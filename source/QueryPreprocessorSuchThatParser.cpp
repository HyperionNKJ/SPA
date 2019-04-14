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
#include "QueryPreprocessorError.h"
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
	: CLAUSE(clause), query(query) {}

// Parses the such that clause.
void QueryPreprocessorSuchThatParser::parse() {
	size_t relSize = CLAUSE.find(BRACKET_LEFT);
	size_t closeBracketPos = CLAUSE.find(BRACKET_RIGHT);

	// close bracket should be the last character
	if (closeBracketPos + 1 != CLAUSE.size()) {
		throw QueryPreprocessorError(ErrorType::SYNTACTIC);
	}

	// open bracket should exist
	if (relSize == std::string::npos) {
		throw QueryPreprocessorError(ErrorType::SYNTACTIC);
	}

	// extract relRef and check that it is legal
	std::string relRefString = CLAUSE.substr(0, relSize);
	std::unordered_map<std::string, RelRef>::const_iterator result = STRING_TO_REL_REF.find(relRefString);
	if (result == STRING_TO_REL_REF.end()) {
		throw QueryPreprocessorError(ErrorType::SYNTACTIC);
	}

	RelRef relRef = result->second;

	// extract parameters
	// there should be exactly 2 parameters
	std::string parameters = CLAUSE.substr(relSize + 1, closeBracketPos - relSize - 1);
	size_t paramOneSize = parameters.find_first_of(COMMA);

	if (paramOneSize == std::string::npos) {
		throw QueryPreprocessorError(ErrorType::SYNTACTIC);
	}
	std::string paramOneString = parameters.substr(0, paramOneSize);
	std::string paramTwoString = parameters.substr(paramOneSize + 1);

	DesignEntity paramOne = QueryPreprocessorHelper::getParam(paramOneString, query);
	DesignEntity paramTwo = QueryPreprocessorHelper::getParam(paramTwoString, query);

	addClause(relRef, paramOne, paramTwo);
}

void QueryPreprocessorSuchThatParser::addClause(const RelRef& relRef, const DesignEntity& paramOne, const DesignEntity& paramTwo) {
	if (relRef == RelRef::AFFECTS) {
		isValidAffectsParam(paramOne, paramTwo);

		Affects* suchThatClause = new Affects(paramOne, paramTwo);
		query.addClause(suchThatClause, CLAUSE);
	}
	else if (relRef == RelRef::AFFECTS_T) {
		isValidAffectsParam(paramOne, paramTwo);

		AffectsT* suchThatClause = new AffectsT(paramOne, paramTwo);
		query.addClause(suchThatClause, CLAUSE);
	}
	else if (relRef == RelRef::CALLS) {
		isValidCallsParam(paramOne, paramTwo);

		Calls* suchThatClause = new Calls(paramOne, paramTwo);
		query.addClause(suchThatClause, CLAUSE);
	}
	else if (relRef == RelRef::CALLS_T) {
		isValidCallsParam(paramOne, paramTwo);

		CallsT* suchThatClause = new CallsT(paramOne, paramTwo);
		query.addClause(suchThatClause, CLAUSE);
	}
	else if (relRef == RelRef::FOLLOWS) {
		isValidFollowsParam(paramOne, paramTwo);

		Follows* suchThatClause = new Follows(paramOne, paramTwo);
		query.addClause(suchThatClause, CLAUSE);
	}
	else if (relRef == RelRef::FOLLOWS_T) {
		isValidFollowsParam(paramOne, paramTwo);

		FollowsT* suchThatClause = new FollowsT(paramOne, paramTwo);
		query.addClause(suchThatClause, CLAUSE);
	}
	else if (relRef == RelRef::MODIFIES) {
		isValidModifiesParam(paramOne, paramTwo);

		if (paramOne.isType(Type::PROCEDURE) || paramOne.isVar()) {
			ModifiesP* suchThatClause = new ModifiesP(paramOne, paramTwo);
			query.addClause(suchThatClause, CLAUSE);
		}
		else {
			ModifiesS* suchThatClause = new ModifiesS(paramOne, paramTwo);
			query.addClause(suchThatClause, CLAUSE);
		}
	}
	else if (relRef == RelRef::NEXT) {
		isValidNextParam(paramOne, paramTwo);

		Next* suchThatClause = new Next(paramOne, paramTwo);
		query.addClause(suchThatClause, CLAUSE);
	}
	else if (relRef == RelRef::NEXT_T) {
		isValidNextParam(paramOne, paramTwo);

		NextT* suchThatClause = new NextT(paramOne, paramTwo);
		query.addClause(suchThatClause, CLAUSE);
	}
	else if (relRef == RelRef::PARENT) {
		isValidParentParam(paramOne, paramTwo);

		Parent* suchThatClause = new Parent(paramOne, paramTwo);
		query.addClause(suchThatClause, CLAUSE);
	}
	else if (relRef == RelRef::PARENT_T) {
		isValidParentParam(paramOne, paramTwo);

		ParentT* suchThatClause = new ParentT(paramOne, paramTwo);
		query.addClause(suchThatClause, CLAUSE);
	}
	else if (relRef == RelRef::USES) {
		isValidUsesParam(paramOne, paramTwo);

		if (paramOne.isType(Type::PROCEDURE) || paramOne.isVar()) {
			UsesP* suchThatClause = new UsesP(paramOne, paramTwo);
			query.addClause(suchThatClause, CLAUSE);
		}
		else {
			UsesS* suchThatClause = new UsesS(paramOne, paramTwo);
			query.addClause(suchThatClause, CLAUSE);
		}
	}
}

void QueryPreprocessorSuchThatParser::isValidAffectsParam(const DesignEntity& paramOne, const DesignEntity& paramTwo) {
	std::vector<Type> legalTypes = { Type::ASSIGN, Type::PROGLINE, Type::STATEMENT, Type::UNDERSCORE };
	
	bool isValidLHS = paramOne.isAnyType(legalTypes) || paramOne.isStmtNo();
	bool isValidRHS = paramTwo.isAnyType(legalTypes) || paramTwo.isStmtNo();

	bool isValid = isValidLHS && isValidRHS;
	if (!isValid) {
		throw QueryPreprocessorError(query, ErrorType::SEMANTIC);
	}
}

void QueryPreprocessorSuchThatParser::isValidCallsParam(const DesignEntity& paramOne, const DesignEntity& paramTwo) {
	std::vector<Type> legalTypes = { Type::PROCEDURE, Type::UNDERSCORE };

	bool isValidLHS = paramOne.isAnyType(legalTypes) || paramOne.isVar();
	bool isValidRHS = paramTwo.isAnyType(legalTypes) || paramTwo.isVar();

	bool isValid = isValidLHS && isValidRHS;
	if (!isValid) {
		throw QueryPreprocessorError(query, ErrorType::SEMANTIC);
	}
}

void QueryPreprocessorSuchThatParser::isValidFollowsParam(const DesignEntity& paramOne, const DesignEntity& paramTwo) {
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
		Type::SWITCH,
		Type::UNDERSCORE
	};

	bool isValidLHS = paramOne.isAnyType(legalTypes) || paramOne.isStmtNo();
	bool isValidRHS = paramTwo.isAnyType(legalTypes) || paramTwo.isStmtNo();

	bool isValid = isValidLHS && isValidRHS;
	if (!isValid) {
		throw QueryPreprocessorError(query, ErrorType::SEMANTIC);
	}
}

void QueryPreprocessorSuchThatParser::isValidModifiesParam(const DesignEntity& paramOne, const DesignEntity& paramTwo) {
	std::vector<Type> legalLHSTypes = { 
		Type::ASSIGN, 
		Type::CALL, 
		Type::IF, 
		Type::PRINT, 
		Type::PROCEDURE,
		Type::PROGLINE, 
		Type::READ, 
		Type::SWITCH,
		Type::STATEMENT, 
		Type::WHILE, 
		Type::FIXED
	};

	std::vector<Type> legalRHSTypes = { Type::VARIABLE, Type::UNDERSCORE };
	
	bool isValidLHS = paramOne.isAnyType(legalLHSTypes);
	bool isValidRHS = paramTwo.isAnyType(legalRHSTypes) || paramTwo.isVar();

	bool isValid = isValidLHS && isValidRHS;
	if (!isValid) {
		throw QueryPreprocessorError(query, ErrorType::SEMANTIC);
	}
}

void QueryPreprocessorSuchThatParser::isValidNextParam(const DesignEntity& paramOne, const DesignEntity& paramTwo) {
	isValidFollowsParam(paramOne, paramTwo);
}

void QueryPreprocessorSuchThatParser::isValidParentParam(const DesignEntity& paramOne, const  DesignEntity& paramTwo) {
	std::vector<Type> legalLHSTypes = {
		Type::STATEMENT,
		Type::WHILE,
		Type::IF,
		Type::SWITCH,
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
		Type::SWITCH,
		Type::STATEMENT,
		Type::WHILE,
		Type::UNDERSCORE
	};

	bool isValidLHS = paramOne.isAnyType(legalLHSTypes) || paramOne.isStmtNo();
	bool isValidRHS = paramTwo.isAnyType(legalRHSTypes) || paramTwo.isStmtNo();

	bool isValid = isValidLHS && isValidRHS;
	if (!isValid) {
		throw QueryPreprocessorError(query, ErrorType::SEMANTIC);
	}
}

void QueryPreprocessorSuchThatParser::isValidUsesParam(const DesignEntity& paramOne, const DesignEntity& paramTwo) {
	isValidModifiesParam(paramOne, paramTwo);
}