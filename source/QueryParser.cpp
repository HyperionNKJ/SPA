#include <regex>
#include <unordered_set>
#include "DesignEntity.h"
#include "QueryPreprocessorHelper.h"
#include "QueryParser.h"

const std::string QueryParser::REL = "Modifies|Uses|Parent\\*?|Follows\\*?";
const std::string QueryParser::IDENT = "[a-zA-z][a-zA-Z\\d]*";
const std::string QueryParser::INT = "\\d+";
const std::string QueryParser::STMT = "_|" + INT + "|" + IDENT;
const std::string QueryParser::ENT = "_|" + IDENT + "|\"" + IDENT + "\"";
const std::string QueryParser::ENT_STMT = "_|" + INT + "|" + IDENT + "|\"" + IDENT + "\"";

const std::regex QueryParser::SELECT_STMT_REGEX("^Select (" + IDENT + ")$|^Select (" + IDENT + ") (such that|pattern) ([a-zA-Z\\d,\\(\\)\"_\\*]+)$|^Select (" + IDENT + ") (such that|pattern) ([a-zA-Z\\d,\\(\\)\"_\\*]+) (such that|pattern) ([a-zA-Z\\d,\\(\\)\"_\\*]+)$");
const std::regex QueryParser::SUCH_THAT_CLAUSE_REGEX("^(" + REL + ")\\((" + STMT + "),(" + ENT_STMT + ")\\)$");
const std::regex QueryParser::STMT_REGEX("^(_)$|^(" + INT + ")$|^(" + IDENT + ")$");
const std::regex QueryParser::ENT_REGEX("^(_)$|^\"(" + IDENT + ")\"$|^(" + IDENT + ")$");
const std::regex QueryParser::PATTERN_CLAUSE_REGEX("^(" + IDENT + ")\\((" + ENT + "),(_|_\"" + INT + "\"_|_\"" + IDENT + "\"_)\\)$");
const std::regex QueryParser::EXPRESSION_REGEX("^(_)$|^_\"(" + IDENT + ")\"_$|^_\"(" + INT + ")\"_$");

const std::unordered_set<std::string> QueryParser::KEYWORDS = {
	"stmt", "read", "print", "while", "if", "assign", "variable", "constant",
	"procedure", "Select", "pattern", "Modifies", "Uses", "Parent", "Follows",
	"Parent*", "Follows*"
};

QueryParser::QueryParser(const std::vector<Statement>& statements)
	: STATEMENTS(statements) {
}

bool QueryParser::parse() {
	for (Statement& statement : STATEMENTS) {
		bool status;

		if (statement.type == StatementType::DECLARATION) {
			status = parseDeclarationStatement(statement);
		} else {
			status = parseSelectStatement(statement);
		}

		if (!status) {
			return false;
		}
	}

	if (query.declarations.find(query.synonym) == query.declarations.end()) {
		return false;
	}

	if (query.hasSuchThatClause) {
		std::string paramOneValue = query.suchThatClause.paramOne.getValue();
		std::string paramTwoValue = query.suchThatClause.paramTwo.getValue();
		Type paramOneType = query.suchThatClause.paramOne.getType();
		Type paramTwoType = query.suchThatClause.paramTwo.getType();

		if (paramOneType == Type::ASSIGN
			&& query.declarations.find(paramOneValue) == query.declarations.end()) {
			return false;
		}

		if (paramTwoType == Type::ASSIGN
			&& query.declarations.find(paramTwoValue) == query.declarations.end()) {
			return false;
		}
	}

	if (query.hasPatternClause) {
		std::string paramOneValue = query.patternClause.paramOne.getValue();
		std::string paramTwoValue = query.patternClause.paramTwo.getValue();
		Type paramOneType = query.patternClause.paramOne.getType();
		Type paramTwoType = query.patternClause.paramTwo.getType();

		if (paramOneType == Type::ASSIGN 
			&& query.declarations.find(paramOneValue) == query.declarations.end()) {
			return false;
		}

		if (paramTwoType == Type::ASSIGN
			&& query.declarations.find(paramTwoValue) == query.declarations.end()) {
			return false;
		}
	}

	return true;
}

ProcessedQuery QueryParser::getQuery() {
	return query;
}

bool QueryParser::parseDeclarationStatement(const Statement& statement) {
	std::string statementString = statement.VALUE;
	size_t synonymIndex = statementString.find_first_of(' ') + 1;

	// get design entity type of this declaration statement
	std::string typeString = statementString.substr(0, synonymIndex - 1);
	Type type = QueryPreprocessorHelper::getType(typeString);

	// get synonyms of this declaration statement
	std::string synonymsString = statementString.substr(synonymIndex);
	std::vector<std::string> synonyms = QueryPreprocessorHelper::split(synonymsString, ',');
	for (std::string& synonym : synonyms) {
		// check for reserved words
		bool isReservedKeyword = KEYWORDS.find(synonym) != KEYWORDS.end();
		if (isReservedKeyword) {
			return false;
		}

		// check for duplicate entry
		bool status = query.insertDeclaration(synonym, type);
		if (!status) {
			return false;
		}
	}

	return true;
}

bool QueryParser::parseSelectStatement(const Statement& statement) {
	std::smatch match;
	std::regex_search(statement.VALUE, match, SELECT_STMT_REGEX);

	// check if statement is a select statement
	if (match.empty()) {
		return false;
	}

	std::string clauseOneType;
	std::string clauseTwoType;

	std::string clauseOneValue;
	std::string clauseTwoValue;

	size_t numberOfClauses;

	if (match[1].matched) {
		numberOfClauses = 0;
		query.addSynonym(match[1].str());
		return true;
	}
	else if (match[2].matched) {
		numberOfClauses = 1;
		query.addSynonym(match[2].str());

		clauseOneType = match[3].str();
		clauseOneValue = match[4].str();
	}
	else {
		numberOfClauses = 2;
		query.addSynonym(match[5].str());

		clauseOneType = match[6].str();
		clauseOneValue = match[7].str();

		clauseTwoType = match[8].str();
		clauseTwoValue = match[9].str();
	}

	bool status;
	if (clauseOneType == "such that") {
		status = parseSuchThatClause(clauseOneValue);
	}
	else {
		status = parsePatternClause(clauseOneValue);
	}

	// returns false if there is an error while parsing the first clause
	if (!status) {
		return false;
	}

	if (numberOfClauses == 1) {
		return true;
	}

	if (clauseTwoType == "such that") {
		status = parseSuchThatClause(clauseTwoValue);
	}
	else {
		status = parsePatternClause(clauseTwoValue);
	}

	// returns false if there is an error while parsing the second clause
	return status;
}

bool QueryParser::parseSuchThatClause(const std::string& clause) {
	std::smatch match;
	std::regex_search(clause, match, SUCH_THAT_CLAUSE_REGEX);

	if (match.empty()) {
		return false;
	}

	std::string typeValue = match[1].str();
	std::string paramOneValue = match[2].str();
	std::string paramTwoValue = match[3].str();

	Type paramOneType = getParameterType(paramOneValue);
	Type paramTwoType = getParameterType(paramTwoValue);

	RelationshipType type = getRelationshipType(typeValue);
	DesignEntity paramOne(paramOneValue, paramOneType);
	DesignEntity paramTwo(paramTwoValue, paramTwoType);

	bool a = query.addSuchThatClause(type, paramOne, paramTwo);
	return a;
}

bool QueryParser::parsePatternClause(const std::string& clause) {
	std::smatch match;
	std::regex_search(clause, match, PATTERN_CLAUSE_REGEX);

	if (match.empty()) {
		return false;
	}

	std::string synonym = match[1].str();

	std::string paramOneValue = match[2].str();
	std::string paramTwoValue = match[3].str();

	Type paramOneType = getParameterType(paramOneValue);
	Type paramTwoType = getParameterType(paramTwoValue);

	DesignEntity assign(synonym, Type::ASSIGN);
	DesignEntity paramOne(paramOneValue, paramOneType);
	DesignEntity paramTwo(paramTwoValue, paramTwoType);

	return query.addPatternClause(assign, paramOne, paramTwo);
}

Type QueryParser::getParameterType(std::string& value) {
	static std::string test = "^(_)$|^(" + INT + ")$|^(" + IDENT + ")$|^\"(" + IDENT + ")\"$|^_\"(" + IDENT + ")\"_|^_\"(" + INT + ")\"_$";
	static std::regex const A(test);
	std::smatch match;
	std::regex_search(value, match, A);

	if (match[1].matched) {
		return Type::UNDERSCORE;
	}

	if (match[3].matched) {
		return Type::ASSIGN;
	}

	if (match[4].matched) {
		value = match[4].str();
	}

	if (match[5].matched) {
		value = match[5].str();
	}

	if (match[6].matched) {
		value = match[6].str();
	}

	return Type::FIXED;
}

RelationshipType QueryParser::getRelationshipType(const std::string& relationshipTypeString) {
	if (relationshipTypeString == "Modifies") {
		return RelationshipType::MODIFIES_S;
	}

	if (relationshipTypeString == "Uses") {
		return RelationshipType::USES_S;
	}

	if (relationshipTypeString == "Parent") {
		return RelationshipType::PARENT;
	}

	if (relationshipTypeString == "Parent*") {
		return RelationshipType::PARENT_T;
	}

	if (relationshipTypeString == "Follows") {
		return RelationshipType::FOLLOWS;
	}

	return RelationshipType::FOLLOWS_T;
}
