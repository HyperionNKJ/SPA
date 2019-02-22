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
	"procedure", "Select", "pattern", "Modifies", "Uses", "Parent", "Follows"
};

QueryParser::QueryParser(std::vector<Statement>& statements) {
	for (Statement& statement : statements) {
		if (statement.type == StatementType::DECLARATION) {
			parseDeclarationStatement(statement);
		}
		else {
			parseSelectStatement(statement);
		}
	}
}

ProcessedQuery QueryParser::getQuery() {
	return query;
}

bool QueryParser::parseDeclarationStatement(const Statement& statement) {
	std::string statementString = statement.VALUE;
	size_t synonymIndex = statementString.find_first_of(' ') + 1;

	// get design entity type of this declaration statement
	std::string typeString = statementString.substr(0, synonymIndex - 1);

	Type type;
	
	if (typeString == "stmt") {
		type = Type::STATEMENT;
	}
	else if (typeString == "read") {
		type = Type::READ;
	}
	else if (typeString == "print") {
		type = Type::PRINT;
	}
	else if (typeString == "while") {
		type = Type::WHILE;
	}
	else if (typeString == "if") {
		type = Type::IF;
	}
	else if (typeString == "assign") {
		type = Type::ASSIGN;
	}
	else if (typeString == "variable") {
		type = Type::VARIABLE;
	}
	else if (typeString == "constant") {
		type = Type::CONSTANT;
	}
	else if (typeString == "procedure") {
		type = Type::PROCEDURE;
	}

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

bool QueryParser::parseSelectStatement(const Statement& stmt) {
	std::smatch match;
	std::regex_search(stmt.VALUE, match, SELECT_STMT_REGEX);

	// check if statement is a select statement
	if (match.empty()) {
		return false;
	}

	std::string synonym;
	std::string firstClauseType;
	std::string firstClauseValue;
	std::string secondClauseType;
	std::string secondClauseValue;

	if (match[1].matched) {
		query.addSynonym(match[1].str());
		return true;
	}
	else if (match[2].matched) {
		query.addSynonym(match[2].str());
		firstClauseType = match[3].str();
		firstClauseValue = match[4].str();
	}
	else {
		query.addSynonym(match[5].str());
		firstClauseType = match[6].str();
		firstClauseValue = match[7].str();
		secondClauseType = match[8].str();
		secondClauseValue = match[9].str();
	}

	bool status;
	if (firstClauseType == "such that") {
		status = parseSuchThatClause(firstClauseValue);
	}
	else {
		status = parsePatternClause(firstClauseValue);
	}

	// returns false if there is an error while parsing the first clause
	if (!status) {
		return false;
	}

	// returns if there are only one select clauses and one constraint clause
	if (match.size() == 4) {
		return true;
	}

	if (secondClauseType == "such that") {
		status = parseSuchThatClause(secondClauseValue);
	}
	else {
		status = parsePatternClause(secondClauseValue);
	}

	// returns false if there is an error while parsing the second clause
	return status;
}

bool QueryParser::parseSuchThatClause(const std::string& clause) {
	std::smatch match;
	std::regex_search(clause, match, SUCH_THAT_CLAUSE_REGEX);

	if (match.size() != 4) {
		return false;
	}

	std::string typeString = match[1].str();

	RelationshipType type = getRelationshipType(typeString);

	std::string paramOneValue = match[2].str();
	std::string paramTwoValue = match[3].str();

	Type paramOneType = getParameterType(paramOneValue);
	Type paramTwoType = getParameterType(paramTwoValue);

	return query.addSuchThatClause(type, paramOneType, paramOneValue,
		paramTwoType, paramTwoValue);
}

bool QueryParser::parsePatternClause(const std::string& clause) {
	std::smatch match;
	std::regex_search(clause, match, PATTERN_CLAUSE_REGEX);

	if (match.size() != 4) {
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

Type QueryParser::getParameterType(const std::string value) {
	static std::string test = "^(_)$|^(" + INT + ")$|^(" + IDENT + ")$|^\"(" + IDENT + ")\"$|^(_\"" + IDENT + "\"_)|^(_\"" + INT + "\"_)$";
	static std::regex const A(test);
	std::smatch match;
	std::regex_search(value, match, A);

	if (match[1].matched) {
		return Type::UNDERSCORE;
	}

	if (match[3].matched) {
		return Type::ASSIGN;
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
