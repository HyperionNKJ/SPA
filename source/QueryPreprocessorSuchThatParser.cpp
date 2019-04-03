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

const std::unordered_map<std::string, QueryPreprocessorSuchThatParser::RelRef> QueryPreprocessorSuchThatParser::STRING_TO_REL_REF = {
	{"Affects", RelRef::AFFECTS},
	{"Affects*", RelRef::AFFECTS_T},
	{"Calls", RelRef::CALLS},
	{"Calls*", RelRef::CALLS_T},
	{"Follows", RelRef::FOLLOWS},
	{"Follows*", RelRef::FOLLOWS_T},
	{"Modifies", RelRef::MODIFIES},
	{"Next", RelRef::NEXT},
	{"Next*", RelRef::NEXT_T},
	{"Parent", RelRef::PARENT},
	{"Parent*", RelRef::PARENT_T},
	{"Uses", RelRef::USES}
};

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

	// extract relRef and check that it is legal
	std::string relRefString = CLAUSE.substr(0, relSize);
	std::unordered_map<std::string, RelRef>::const_iterator result = STRING_TO_REL_REF.find(relRefString);
	if (result == STRING_TO_REL_REF.end()) {
		return false;
	}

	RelRef relRef = result->second;

	// extract parameters
	// there should be exactly 2 parameters
	std::string parameters = CLAUSE.substr(relSize + 1, closeBracketPos - relSize - 1);
	std::vector<string> parametersList = QueryPreprocessorHelper::split(parameters, COMMA);

	if (parametersList.size() != 2) {
		return false;
	}

	DesignEntity paramOne = QueryPreprocessorHelper::getParam(parametersList[0], query);
	DesignEntity paramTwo = QueryPreprocessorHelper::getParam(parametersList[1], query);

	return addClause(relRef, paramOne, paramTwo);
}

bool QueryPreprocessorSuchThatParser::addClause(const RelRef& relRef, const DesignEntity& paramOne, const DesignEntity& paramTwo) {
	if (relRef == RelRef::AFFECTS) {
		bool status = isValidAffectsParam(paramOne, paramTwo);
		if (!status) {
			return false;
		}

		//Affects* suchThatClause = new Affects(paramOne, paramTwo);
		//query.addClause(suchThatClause, CLAUSE);
	}
	else if (relRef == RelRef::AFFECTS_T) {
		bool status = isValidAffectsParam(paramOne, paramTwo);
		if (!status) {
			return false;
		}

		//AffectsT* suchThatClause = new AffectsT(paramOne, paramTwo);
		//query.addClause(suchThatClause, CLAUSE);
	}
	else if (relRef == RelRef::CALLS) {
		bool status = isValidCallsParam(paramOne, paramTwo);
		if (!status) {
			return false;
		}

		Calls* suchThatClause = new Calls(paramOne, paramTwo);
		query.addClause(suchThatClause, CLAUSE);
	}
	else if (relRef == RelRef::CALLS_T) {
		bool status = isValidCallsParam(paramOne, paramTwo);
		if (!status) {
			return false;
		}

		CallsT* suchThatClause = new CallsT(paramOne, paramTwo);
		query.addClause(suchThatClause, CLAUSE);
	}
	else if (relRef == RelRef::FOLLOWS) {
		bool status = isValidFollowsParam(paramOne, paramTwo);
		if (!status) {
			return false;
		}

		Follows* suchThatClause = new Follows(paramOne, paramTwo);
		query.addClause(suchThatClause, CLAUSE);
	}
	else if (relRef == RelRef::FOLLOWS_T) {
		bool status = isValidFollowsParam(paramOne, paramTwo);
		if (!status) {
			return false;
		}

		FollowsT* suchThatClause = new FollowsT(paramOne, paramTwo);
		query.addClause(suchThatClause, CLAUSE);
	}
	else if (relRef == RelRef::MODIFIES) {
		bool status = isValidModifiesParam(paramOne, paramTwo);
		if (!status) {
			return false;
		}

		if (paramOne.isType(Type::PROCEDURE)) {
			ModifiesP* suchThatClause = new ModifiesP(paramOne, paramTwo);
			query.addClause(suchThatClause, CLAUSE);
		}
		else if (paramOne.isVar()) {
			ModifiesP* suchThatClause = new ModifiesP(paramOne, paramTwo);
			query.addClause(suchThatClause, CLAUSE);
		}
		else {
			ModifiesS* suchThatClause = new ModifiesS(paramOne, paramTwo);
			query.addClause(suchThatClause, CLAUSE);
		}
	}
	else if (relRef == RelRef::NEXT) {
		bool status = isValidNextParam(paramOne, paramTwo);
		if (!status) {
			return false;
		}

		Next* suchThatClause = new Next(paramOne, paramTwo);
		query.addClause(suchThatClause, CLAUSE);
	}
	else if (relRef == RelRef::NEXT_T) {
		bool status = isValidNextParam(paramOne, paramTwo);
		if (!status) {
			return false;
		}

		NextT* suchThatClause = new NextT(paramOne, paramTwo);
		query.addClause(suchThatClause, CLAUSE);
	}
	else if (relRef == RelRef::PARENT) {
		bool status = isValidParentParam(paramOne, paramTwo);
		if (!status) {
			return false;
		}

		Parent* suchThatClause = new Parent(paramOne, paramTwo);
		query.addClause(suchThatClause, CLAUSE);
	}
	else if (relRef == RelRef::PARENT_T) {
		bool status = isValidParentParam(paramOne, paramTwo);
		if (!status) {
			return false;
		}

		ParentT* suchThatClause = new ParentT(paramOne, paramTwo);
		query.addClause(suchThatClause, CLAUSE);
	}
	else if (relRef == RelRef::USES) {
		bool status = isValidUsesParam(paramOne, paramTwo);
		if (!status) {
			return false;
		}

		if (paramOne.isType(Type::PROCEDURE)) {
			UsesP* suchThatClause = new UsesP(paramOne, paramTwo);
			query.addClause(suchThatClause, CLAUSE);
		}
		else if (paramOne.isVar()) {
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

bool QueryPreprocessorSuchThatParser::isValidAffectsParam(const DesignEntity& paramOne, const DesignEntity& paramTwo) {
	std::vector<Type> legalTypes = { Type::ASSIGN, Type::PROGLINE, Type::UNDERSCORE };
	
	bool isValidLHS = paramOne.isAnyType(legalTypes) || paramOne.isStmtNo();
	bool isValidRHS = paramTwo.isAnyType(legalTypes) || paramTwo.isStmtNo();

	return isValidLHS && isValidRHS;
}

bool QueryPreprocessorSuchThatParser::isValidCallsParam(const DesignEntity& paramOne, const DesignEntity& paramTwo) {
	std::vector<Type> legalTypes = { Type::PROCEDURE, Type::UNDERSCORE };

	bool isValidLHS = paramOne.isAnyType(legalTypes) || paramOne.isVar();
	bool isValidRHS = paramTwo.isAnyType(legalTypes) || paramTwo.isVar();

	return isValidLHS && isValidRHS;
}

bool QueryPreprocessorSuchThatParser::isValidFollowsParam(const DesignEntity& paramOne, const DesignEntity& paramTwo) {
	std::vector<Type> legalTypes = { 
		Type::ASSIGN, 
		Type::CALL, 
		Type::IF, 
		Type::PRINT, 
		Type::PROCEDURE, 
		Type::PROGLINE, 
		Type::READ, 
		Type::STATEMENT, 
		Type::WHILE, 
		Type::UNDERSCORE
	};

	bool isValidLHS = paramOne.isAnyType(legalTypes) || paramOne.isStmtNo();
	bool isValidRHS = paramTwo.isAnyType(legalTypes) || paramTwo.isStmtNo();

	return isValidLHS && isValidRHS;
}

bool QueryPreprocessorSuchThatParser::isValidModifiesParam(const DesignEntity& paramOne, const DesignEntity& paramTwo) {
	std::vector<Type> legalLHSTypes = { 
		Type::ASSIGN, 
		Type::CALL, 
		Type::IF, 
		Type::PRINT, 
		Type::PROCEDURE,
		Type::PROGLINE, 
		Type::READ, 
		Type::STATEMENT, 
		Type::WHILE, 
		Type::FIXED
	};

	std::vector<Type> legalRHSTypes = { Type::VARIABLE, Type::UNDERSCORE };
	
	bool isValidLHS = paramOne.isAnyType(legalLHSTypes);
	bool isValidRHS = paramTwo.isAnyType(legalRHSTypes) || paramTwo.isVar();

	return isValidLHS && isValidRHS;
}

bool QueryPreprocessorSuchThatParser::isValidNextParam(const DesignEntity& paramOne, const DesignEntity& paramTwo) {
	return isValidFollowsParam(paramOne, paramTwo);
}

bool QueryPreprocessorSuchThatParser::isValidParentParam(const DesignEntity& paramOne, const  DesignEntity& paramTwo) {
	std::vector<Type> legalLHSTypes = {
		Type::STATEMENT,
		Type::WHILE,
		Type::IF,
		Type::PROGLINE,
		Type::UNDERSCORE
	};

	std::vector<Type> legalRHSTypes = {
		Type::ASSIGN,
		Type::CALL,
		Type::IF,
		Type::PRINT,
		Type::PROCEDURE,
		Type::PROGLINE,
		Type::READ,
		Type::STATEMENT,
		Type::WHILE,
		Type::UNDERSCORE
	};

	bool isValidLHS = paramOne.isAnyType(legalLHSTypes) || paramOne.isStmtNo();
	bool isValidRHS = paramTwo.isAnyType(legalRHSTypes) || paramTwo.isStmtNo();

	return isValidLHS && isValidRHS;
}

bool QueryPreprocessorSuchThatParser::isValidUsesParam(const DesignEntity& paramOne, const DesignEntity& paramTwo) {
	return isValidModifiesParam(paramOne, paramTwo);
}