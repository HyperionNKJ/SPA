#include <regex>
#include <unordered_set>
#include "Calls.h"
#include "CallsT.h"
#include "DesignEntity.h"
#include "Follows.h"
#include "FollowsT.h"
#include "ModifiesP.h"
#include "ModifiesS.h"
#include "Next.h"
#include "NextT.h"
#include "Parent.h"
#include "ParentT.h"
#include "QueryPreprocessorDeclareParser.h"
#include "QueryPreprocessorHelper.h"
#include "QueryPreprocessorPatternParser.h"
#include "QueryPreprocessorResultParser.h"
#include "QueryPreprocessorSelectParser.h"
#include "QueryPreprocessorWithParser.h"
#include "UsesP.h"
#include "UsesS.h"

constexpr char COMMA = ',';
constexpr auto SPACE = ' ';
constexpr auto SELECT_START = 7;

// Regular Expressions
constexpr char REL_REF[] = "^(Modifies|Uses|Calls|Calls\\*|Parent|Parent\\*|Follows|Follows\\*|Next|Next\\*|Affects|Affects\\*)$";
const regex QueryPreprocessorSelectParser::REL_REF_REGEX(REL_REF);

// Initializes a newly created QueryPreprocessorSelectParser.
QueryPreprocessorSelectParser::QueryPreprocessorSelectParser(const string& statement, ProcessedQuery& query)
				: STATEMENT(statement),
				query(query) {
}

// Parse select clause.
bool QueryPreprocessorSelectParser::parse() {
	// remove "Select "
	std::string selectCl = STATEMENT;
	selectCl.erase(0, 7);
	
	// extract result clause
	size_t resultClSize = selectCl.find_first_of(SPACE);
	if (resultClSize == std::string::npos) {
		// case 0: there is only a result clause
		QueryPreprocessorResultParser parseResultCl(selectCl, query);
		bool status = parseResultCl.parse();
		query = parseResultCl.query;
		return status;
	}

	// case 1: there are other clauses
	std::string resultCl = selectCl.substr(0, resultClSize);
	selectCl.erase(0, resultClSize + 1);

	QueryPreprocessorResultParser parseResultCl(resultCl, query);
	bool status = parseResultCl.parse();
	query = parseResultCl.query;

	if (!status) {
		return false;
	}

	// extract such that, pattern, with clauses
	ClauseType clauseType = ClauseType::RESULT;
	while (selectCl.size() > 0) {
		std::string type;
		size_t typeSize;

		if (selectCl.find("such that ") == 0) {
			type = "such that";
			clauseType = ClauseType::SUCH_THAT;
		} else if (selectCl.find("pattern ") == 0) {
			type = "pattern";
			clauseType = ClauseType::PATTERN;
		} else if (selectCl.find("with ") == 0) {
			type = "with";
			clauseType = ClauseType::WITH;
		} else if (selectCl.find("and ") == 0) {
			type = "and";
		} else {
			return false;
		}

		typeSize = type.size() + 1;

		std::string clause;

		size_t clauseEndPos = selectCl.find(SPACE, typeSize + 6);
		if (clauseEndPos == std::string::npos) {
			clause = selectCl.substr(typeSize);
			selectCl = "";
		} else {
			clause = selectCl.substr(typeSize, clauseEndPos - typeSize);
			selectCl.erase(0, clauseEndPos + 1);
		}

		bool status;
		if (type == "such that") {
			status = parseSuchThatCl(clause);
		} else if (type == "pattern") {
			QueryPreprocessorPatternParser parsePatternCl(clause, query);
			status = parsePatternCl.parse();
			query = parsePatternCl.query;
		} else if (type == "with") {
			QueryPreprocessorWithParser parseWithCl(clause, query);
			status = parseWithCl.parse();
			query = parseWithCl.query;
		} else {
			// Clauses should be in the form of rel(...)
			size_t relSize = clause.find('(');
			if (relSize == std::string::npos) {
				return false;
			}

			// If rel is found in declarations, the clause is a pattern clause
			std::string rel = clause.substr(0, relSize);
			if (query.declarations.find(rel) != query.declarations.end() && clauseType == ClauseType::PATTERN) {
				QueryPreprocessorPatternParser parsePatternCl(clause, query);
				status = parsePatternCl.parse();
				query = parsePatternCl.query;
			} else if (clause.find('=') != std::string::npos && clauseType == ClauseType::WITH) {
				QueryPreprocessorWithParser parseWithCl(clause, query);
				status = parseWithCl.parse();
				query = parseWithCl.query;
			} else if (clauseType == ClauseType::SUCH_THAT) {
				status = parseSuchThatCl(clause);
			} else {
				return false;
			}
		}

		if (!status) {
			return false;
		}
	}

	return true;
}

// Parse such that clause.
bool QueryPreprocessorSelectParser::parseSuchThatCl(std::string& suchThatCl) {
	size_t relSize = suchThatCl.find('(');
	size_t closeBracketPos = suchThatCl.find(')');

	// close bracket should be the last character
	if (closeBracketPos + 1 != suchThatCl.size()) {
		return false;
	}

	// open bracket should exist
	if (relSize == std::string::npos) {
		return false;
	}

	// extract rel and check that it is legal
	std::string rel = suchThatCl.substr(0, relSize);
	if (!regex_match(rel, REL_REF_REGEX)) {
		return false;
	}

	// remove rel and the brackets
	suchThatCl = suchThatCl.substr(relSize + 1, closeBracketPos - relSize - 1);

	std::vector<string> parameters = QueryPreprocessorHelper::split(suchThatCl, COMMA);

	if (parameters.size() != 2) {
		return false;
	}

	DesignEntity paramOne = parseParameter(parameters[0]);
	DesignEntity paramTwo = parseParameter(parameters[1]);

	std::string designEntityOne = QueryPreprocessorHelper::TYPE_TO_STRING.find(paramOne.getType())->second;
	std::string designEntityTwo = QueryPreprocessorHelper::TYPE_TO_STRING.find(paramTwo.getType())->second;
	std::string clauseString = paramOne.getValue() + designEntityOne + paramTwo.getValue() + designEntityTwo;

	if (rel == "Calls") {
		clauseString = "Calls" + clauseString;
		if (query.clausesString.find(clauseString) == query.clausesString.end()) {
			Calls* suchThatClause = new Calls(paramOne, paramTwo);
			query.addClause(suchThatClause);
			query.clausesString.insert(clauseString);
		}
	} else if (rel == "Calls*") {
		clauseString = "Calls*" + clauseString;
		if (query.clausesString.find(clauseString) == query.clausesString.end()) {
			CallsT* suchThatClause = new CallsT(paramOne, paramTwo);
			query.addClause(suchThatClause);
			query.clausesString.insert(clauseString);
		}
	} else if (rel == "Follows") {
		clauseString = "Follows" + clauseString;
		if (query.clausesString.find(clauseString) == query.clausesString.end()) {
			Follows* suchThatClause = new Follows(paramOne, paramTwo);
			query.addClause(suchThatClause);
			query.clausesString.insert(clauseString);
		}
	} else if (rel == "Follows*") {
		clauseString = "Follows*" + clauseString;
		if (query.clausesString.find(clauseString) == query.clausesString.end()) {
			FollowsT* suchThatClause = new FollowsT(paramOne, paramTwo);
			query.addClause(suchThatClause);
			query.clausesString.insert(clauseString);
		}
	} else if (rel == "Modifies") {
		clauseString = "Modifies" + clauseString;
		if (paramOne.getType() == Type::PROCEDURE) {
			if (query.clausesString.find(clauseString) == query.clausesString.end()) {
				ModifiesP* suchThatClause = new ModifiesP(paramOne, paramTwo);
				query.addClause(suchThatClause);
				query.clausesString.insert(clauseString);
			}
		} else if (paramOne.getType() == Type::FIXED && QueryPreprocessorHelper::isVar(paramOne.getValue())) {
			if (query.clausesString.find(clauseString) == query.clausesString.end()) {
				ModifiesP* suchThatClause = new ModifiesP(paramOne, paramTwo);
				query.addClause(suchThatClause);
				query.clausesString.insert(clauseString);
			}
		} else {
			if (query.clausesString.find(clauseString) == query.clausesString.end()) {
				ModifiesS* suchThatClause = new ModifiesS(paramOne, paramTwo);
				query.addClause(suchThatClause);
				query.clausesString.insert(clauseString);
			}
		}
	} else if (rel == "Next") {
		clauseString = "Next" + clauseString;
		if (query.clausesString.find(clauseString) == query.clausesString.end()) {
			Next* suchThatClause = new Next(paramOne, paramTwo);
			query.addClause(suchThatClause);
			query.clausesString.insert(clauseString);
		}
	} else if (rel == "Next*") {
		clauseString = "Next*" + clauseString;
		if (query.clausesString.find(clauseString) == query.clausesString.end()) {
			NextT* suchThatClause = new NextT(paramOne, paramTwo);
			query.addClause(suchThatClause);
			query.clausesString.insert(clauseString);
		}
	} else if (rel == "Parent") {
		clauseString = "Parent" + clauseString;
		if (query.clausesString.find(clauseString) == query.clausesString.end()) {
			Parent* suchThatClause = new Parent(paramOne, paramTwo);
			query.addClause(suchThatClause);
		}
	} else if (rel == "Uses") {
		clauseString = "Uses" + clauseString;
		if (paramOne.getType() == Type::PROCEDURE) {
			if (query.clausesString.find(clauseString) == query.clausesString.end()) {
				UsesP* suchThatClause = new UsesP(paramOne, paramTwo);
				query.addClause(suchThatClause);
				query.clausesString.insert(clauseString);
			}
		} else if (paramOne.getType() == Type::FIXED && QueryPreprocessorHelper::isVar(paramOne.getValue())) {
			if (query.clausesString.find(clauseString) == query.clausesString.end()) {
				UsesP* suchThatClause = new UsesP(paramOne, paramTwo);
				query.addClause(suchThatClause);
				query.clausesString.insert(clauseString);
			}
		} else {
			if (query.clausesString.find(clauseString) == query.clausesString.end()) {
				UsesS* suchThatClause = new UsesS(paramOne, paramTwo);
				query.addClause(suchThatClause);
				query.clausesString.insert(clauseString);
			}
		}
	}

	return true;
}

DesignEntity QueryPreprocessorSelectParser::parseParameter(std::string& parameter) {
	if (parameter == "_") {
		// underscore
		return DesignEntity("", Type::UNDERSCORE);
	} else if (QueryPreprocessorHelper::isInt(parameter)) {
		// statement number
		return DesignEntity(parameter, Type::FIXED);
	} else if (parameter.front() == '"' && parameter.back() == '"') {
		// constant
		parameter = parameter.substr(1, parameter.size() - 2);

		if (QueryPreprocessorHelper::isVar(parameter)) {
			return DesignEntity(parameter, Type::FIXED);
		}

		return DesignEntity("", Type::INVALID);
	} else if (QueryPreprocessorHelper::isVar(parameter)
			&& query.declarations.find(parameter) != query.declarations.end()) {
		Type designEntity = query.declarations.find(parameter)->second;
		return DesignEntity(parameter, designEntity);
	}

	return DesignEntity("", Type::INVALID);
}
