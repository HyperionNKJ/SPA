#include <regex>
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
#include "QueryPreprocessorHelper.h"
#include "QueryPreprocessorSuchThatParser.h"
#include "UsesP.h"
#include "UsesS.h"

constexpr char COMMA = ',';

// Initializes a newly created QueryPreprocessorSuchThatParser.
QueryPreprocessorSuchThatParser::QueryPreprocessorSuchThatParser(const string& clause, ProcessedQuery& query)
	: CLAUSE(clause), query(query) {
}

// Parses the such that clause.
// Returns true if parsing is successful and false if unsucessful.
bool QueryPreprocessorSuchThatParser::parse() {
	size_t relSize = CLAUSE.find('(');
	size_t closeBracketPos = CLAUSE.find(')');

	// close bracket should be the last character
	if (closeBracketPos + 1 != CLAUSE.size()) {
		return false;
	}

	// open bracket should exist
	if (relSize == std::string::npos) {
		return false;
	}

	// extract rel and check that it is legal
	std::string rel = CLAUSE.substr(0, relSize);
	if (!isRelRef(rel)) {
		return false;
	}

	// extract parameters
	// there should be exactly 2 parameters
	std::string parameters = CLAUSE.substr(relSize + 1, closeBracketPos - relSize - 1);
	std::vector<string> parametersList = QueryPreprocessorHelper::split(parameters, COMMA);

	if (parametersList.size() != 2) {
		return false;
	}

	DesignEntity paramOne = QueryPreprocessorHelper::getParam(parametersList[0], query);
	DesignEntity paramTwo = QueryPreprocessorHelper::getParam(parametersList[1], query);

	if (rel == "Calls") {
		Calls* suchThatClause = new Calls(paramOne, paramTwo);
		query.addClause(suchThatClause, CLAUSE);
	}
	else if (rel == "Calls*") {
		CallsT* suchThatClause = new CallsT(paramOne, paramTwo);
		query.addClause(suchThatClause, CLAUSE);
	}
	else if (rel == "Follows") {
		Follows* suchThatClause = new Follows(paramOne, paramTwo);
		query.addClause(suchThatClause, CLAUSE);
	}
	else if (rel == "Follows*") {
		FollowsT* suchThatClause = new FollowsT(paramOne, paramTwo);
		query.addClause(suchThatClause, CLAUSE);
	}
	else if (rel == "Modifies") {
		if (paramOne.getType() == Type::PROCEDURE) {
			ModifiesP* suchThatClause = new ModifiesP(paramOne, paramTwo);
			query.addClause(suchThatClause, CLAUSE);
		}
		else if (paramOne.getType() == Type::FIXED && QueryPreprocessorHelper::isVar(paramOne.getValue())) {
			ModifiesP* suchThatClause = new ModifiesP(paramOne, paramTwo);
			query.addClause(suchThatClause, CLAUSE);
		}
		else {
			ModifiesS* suchThatClause = new ModifiesS(paramOne, paramTwo);
			query.addClause(suchThatClause, CLAUSE);
		}
	}
	else if (rel == "Next") {
		Next* suchThatClause = new Next(paramOne, paramTwo);
		query.addClause(suchThatClause, CLAUSE);
	}
	else if (rel == "Next*") {
		NextT* suchThatClause = new NextT(paramOne, paramTwo);
		query.addClause(suchThatClause, CLAUSE);
	}
	else if (rel == "Parent") {
		Parent* suchThatClause = new Parent(paramOne, paramTwo);
		query.addClause(suchThatClause, CLAUSE);
	}
	else if (rel == "Uses") {
		if (paramOne.getType() == Type::PROCEDURE) {
			UsesP* suchThatClause = new UsesP(paramOne, paramTwo);
			query.addClause(suchThatClause, CLAUSE);
		}
		else if (paramOne.getType() == Type::FIXED && QueryPreprocessorHelper::isVar(paramOne.getValue())) {
			UsesP* suchThatClause = new UsesP(paramOne, paramTwo);
			query.addClause(suchThatClause, CLAUSE);
		}
		else {
			UsesS* suchThatClause = new UsesS(paramOne, paramTwo);
			query.addClause(suchThatClause, CLAUSE);
		}
	}

	return true;
}

bool QueryPreprocessorSuchThatParser::isRelRef(std::string& relRef) {
	return relRef == "Affects"
		|| relRef == "Affects*"
		|| relRef == "Calls"
		|| relRef == "Calls*"
		|| relRef == "Follows"
		|| relRef == "Follows*"
		|| relRef == "Modifies"
		|| relRef == "Next"
		|| relRef == "Next*"
		|| relRef == "Parent"
		|| relRef == "Parent*"
		|| relRef == "Uses";
}
