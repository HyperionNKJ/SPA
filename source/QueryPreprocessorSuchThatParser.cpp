#include <regex>
#include "Affects.h"
#include "AffectsT.h"
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

constexpr char BRACKET_LEFT = '(';
constexpr char BRACKET_RIGHT = ')';
constexpr char COMMA = ',';

constexpr char STRING_AFFECTS[] = "Affects";
constexpr char STRING_AFFECTS_T[] = "Affects*";
constexpr char STRING_CALLS[] = "Calls";
constexpr char STRING_CALLS_T[] = "Calls*";
constexpr char STRING_FOLLOWS[] = "Follows";
constexpr char STRING_FOLLOWS_T[] = "Follows*";
constexpr char STRING_MODIFIES[] = "Modifies";
constexpr char STRING_NEXT[] = "Next";
constexpr char STRING_NEXT_T[] = "Next*";
constexpr char STRING_PARENT[] = "Parent";
constexpr char STRING_PARENT_T[] = "Parent*";
constexpr char STRING_USES[] = "Uses";

// Initializes a newly created QueryPreprocessorSuchThatParser.
QueryPreprocessorSuchThatParser::QueryPreprocessorSuchThatParser(const string& clause, ProcessedQuery& query)
	: CLAUSE(clause), query(query) {
}

// Parses the such that clause.
// Returns true if parsing is successful and false if unsucessful.
bool QueryPreprocessorSuchThatParser::parse() {
	size_t relSize = CLAUSE.find(BRACKET_LEFT);
	size_t closeBracketPos = CLAUSE.find(BRACKET_RIGHT);

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

	if (rel == STRING_AFFECTS) {
		bool status = isValidAffectsParam(paramOne, paramTwo);
		if (!status) {
			return false;
		}

		//Affects* suchThatClause = new Affects(paramOne, paramTwo);
		//query.addClause(suchThatClause, CLAUSE);
	}
	else if (rel == STRING_AFFECTS_T) {
		bool status = isValidAffectsParam(paramOne, paramTwo);
		if (!status) {
			return false;
		}

		//AffectsT* suchThatClause = new AffectsT(paramOne, paramTwo);
		//query.addClause(suchThatClause, CLAUSE);
	}
	else if (rel == STRING_CALLS) {
		bool status = isValidCallsParam(paramOne, paramTwo);
		if (!status) {
			return false;
		}

		Calls* suchThatClause = new Calls(paramOne, paramTwo);
		query.addClause(suchThatClause, CLAUSE);
	}
	else if (rel == STRING_CALLS_T) {
		bool status = isValidCallsParam(paramOne, paramTwo);
		if (!status) {
			return false;
		}

		CallsT* suchThatClause = new CallsT(paramOne, paramTwo);
		query.addClause(suchThatClause, CLAUSE);
	}
	else if (rel == STRING_FOLLOWS) {
		bool status = isValidFollowsParam(paramOne, paramTwo);
		if (!status) {
			return false;
		}

		Follows* suchThatClause = new Follows(paramOne, paramTwo);
		query.addClause(suchThatClause, CLAUSE);
	}
	else if (rel == STRING_FOLLOWS_T) {
		bool status = isValidFollowsParam(paramOne, paramTwo);
		if (!status) {
			return false;
		}

		FollowsT* suchThatClause = new FollowsT(paramOne, paramTwo);
		query.addClause(suchThatClause, CLAUSE);
	}
	else if (rel == STRING_MODIFIES) {
		bool status = isValidModifiesParam(paramOne, paramTwo);
		if (!status) {
			return false;
		}

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
	else if (rel == STRING_NEXT) {
		bool status = isValidNextParam(paramOne, paramTwo);
		if (!status) {
			return false;
		}

		Next* suchThatClause = new Next(paramOne, paramTwo);
		query.addClause(suchThatClause, CLAUSE);
	}
	else if (rel == STRING_NEXT_T) {
		bool status = isValidNextParam(paramOne, paramTwo);
		if (!status) {
			return false;
		}

		NextT* suchThatClause = new NextT(paramOne, paramTwo);
		query.addClause(suchThatClause, CLAUSE);
	}
	else if (rel == STRING_PARENT) {
		bool status = isValidParentParam(paramOne, paramTwo);
		if (!status) {
			return false;
		}

		Parent* suchThatClause = new Parent(paramOne, paramTwo);
		query.addClause(suchThatClause, CLAUSE);
	}
	else if (rel == STRING_PARENT_T) {
		bool status = isValidParentParam(paramOne, paramTwo);
		if (!status) {
			return false;
		}

		ParentT* suchThatClause = new ParentT(paramOne, paramTwo);
		query.addClause(suchThatClause, CLAUSE);
	}
	else if (rel == STRING_USES) {
		bool status = isValidUsesParam(paramOne, paramTwo);
		if (!status) {
			return false;
		}

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
	return relRef == STRING_AFFECTS
		|| relRef == STRING_AFFECTS_T
		|| relRef == STRING_CALLS
		|| relRef == STRING_CALLS_T
		|| relRef == STRING_FOLLOWS
		|| relRef == STRING_FOLLOWS_T
		|| relRef == STRING_MODIFIES
		|| relRef == STRING_NEXT
		|| relRef == STRING_NEXT_T
		|| relRef == STRING_PARENT
		|| relRef == STRING_PARENT_T
		|| relRef == STRING_USES;
}

bool QueryPreprocessorSuchThatParser::isValidAffectsParam(DesignEntity& paramOne, DesignEntity& paramTwo) {
	Type paramOneType = paramOne.getType();
	Type paramTwoType = paramTwo.getType();

	bool isValidLHS = paramOneType == Type::ASSIGN
		|| paramOneType == Type::UNDERSCORE
		|| (paramOneType == Type::FIXED && QueryPreprocessorHelper::isInt(paramOne.getValue));
	
	bool isValidRHS = paramTwoType == Type::ASSIGN
		|| paramTwoType == Type::UNDERSCORE
		|| (paramTwoType == Type::FIXED && QueryPreprocessorHelper::isInt(paramTwo.getValue));

	return isValidLHS && isValidRHS;
}

bool QueryPreprocessorSuchThatParser::isValidCallsParam(DesignEntity& paramOne, DesignEntity& paramTwo) {
	Type paramOneType = paramOne.getType();
	Type paramTwoType = paramTwo.getType();

	bool isValidLHS = paramOneType == Type::PROCEDURE
		|| paramOneType == Type::UNDERSCORE
		|| (paramOneType == Type::FIXED && QueryPreprocessorHelper::isVar(paramOne.getValue()));

	bool isValidRHS = paramTwoType == Type::PROCEDURE
		|| paramTwoType == Type::UNDERSCORE
		|| (paramTwoType == Type::FIXED && QueryPreprocessorHelper::isVar(paramTwo.getValue()));

	return isValidLHS && isValidRHS;
}

bool QueryPreprocessorSuchThatParser::isValidFollowsParam(DesignEntity& paramOne, DesignEntity& paramTwo) {
	Type paramOneType = paramOne.getType();
	Type paramTwoType = paramTwo.getType();

	bool isValidLHS = paramOneType == Type::ASSIGN
		|| paramOneType == Type::CALL
		|| paramOneType == Type::IF
		|| paramOneType == Type::PRINT
		|| paramOneType == Type::PROCEDURE
		|| paramOneType == Type::PROGLINE
		|| paramOneType == Type::READ
		|| paramOneType == Type::STATEMENT
		|| paramOneType == Type::WHILE
		|| paramOneType == Type::UNDERSCORE
		|| (paramOneType == Type::FIXED && QueryPreprocessorHelper::isInt(paramOne.getValue()));

	bool isValidRHS = paramTwoType == Type::ASSIGN
		|| paramTwoType == Type::CALL
		|| paramTwoType == Type::IF
		|| paramTwoType == Type::PRINT
		|| paramTwoType == Type::PROCEDURE
		|| paramTwoType == Type::PROGLINE
		|| paramTwoType == Type::READ
		|| paramTwoType == Type::STATEMENT
		|| paramTwoType == Type::WHILE
		|| paramTwoType == Type::UNDERSCORE
		|| (paramTwoType == Type::FIXED && QueryPreprocessorHelper::isInt(paramTwo.getValue()));

	return isValidLHS && isValidRHS;
}

bool QueryPreprocessorSuchThatParser::isValidModifiesParam(DesignEntity& paramOne, DesignEntity& paramTwo) {
	Type paramOneType = paramOne.getType();
	Type paramTwoType = paramTwo.getType();
	
	bool isValidLHS = paramOneType == Type::ASSIGN
		|| paramOneType == Type::CALL
		|| paramOneType == Type::IF
		|| paramOneType == Type::PRINT
		|| paramOneType == Type::PROCEDURE
		|| paramOneType == Type::PROGLINE
		|| paramOneType == Type::READ
		|| paramOneType == Type::STATEMENT
		|| paramOneType == Type::WHILE
		|| paramOneType == Type::FIXED;

	bool isValidRHS = paramTwoType == Type::VARIABLE
		|| paramTwoType == Type::UNDERSCORE
		|| (paramTwoType == Type::FIXED && QueryPreprocessorHelper::isVar(paramTwo.getValue()));

	return isValidLHS && isValidRHS;
}

bool QueryPreprocessorSuchThatParser::isValidNextParam(DesignEntity& paramOne, DesignEntity& paramTwo) {
	return isValidFollowsParam(paramOne, paramTwo);
}

bool QueryPreprocessorSuchThatParser::isValidParentParam(DesignEntity& paramOne, DesignEntity& paramTwo) {
	Type paramOneType = paramOne.getType();
	Type paramTwoType = paramTwo.getType();

	bool isValidLHS = paramOneType == Type::STATEMENT
		|| paramOneType == Type::WHILE
		|| paramOneType == Type::IF
		|| paramOneType == Type::PROGLINE
		|| paramOneType == Type::UNDERSCORE
		|| (paramOneType == Type::FIXED && QueryPreprocessorHelper::isInt(paramOne.getValue()));

	bool isValidRHS = paramTwoType == Type::ASSIGN
		|| paramTwoType == Type::CALL
		|| paramTwoType == Type::IF
		|| paramTwoType == Type::PRINT
		|| paramTwoType == Type::PROCEDURE
		|| paramTwoType == Type::PROGLINE
		|| paramTwoType == Type::READ
		|| paramTwoType == Type::STATEMENT
		|| paramTwoType == Type::WHILE
		|| paramTwoType == Type::UNDERSCORE
		|| (paramTwoType == Type::FIXED && QueryPreprocessorHelper::isInt(paramTwo.getValue()));

	return isValidLHS && isValidRHS;
}

bool QueryPreprocessorSuchThatParser::isValidUsesParam(DesignEntity& paramOne, DesignEntity& paramTwo) {
	return isValidModifiesParam(paramOne, paramTwo);
}