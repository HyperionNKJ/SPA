#include "ParentT.h"

ParentT::ParentT(const DesignEntity& paraOne, const DesignEntity& paraTwo) : Clause(paraOne, paraTwo) {}

Result ParentT::evaluate(const PKB& pkb) {
	this->pkb = pkb;
	Type paraOneType = paraOne.getType();
	Type paraTwoType = paraTwo.getType();
	string paraOneValue = paraOne.getValue();
	string paraTwoValue = paraTwo.getValue();

	Result* result;

	if (paraOneType == FIXED) {
		if (paraTwoType == STATEMENT || paraTwoType == READ || paraTwoType == PRINT
			|| paraTwoType == WHILE || paraTwoType == IF || paraTwoType == ASSIGN) {
			result = this->evaluateFixedSynonym(paraOneValue, paraTwoValue, paraTwoType);
		}
		else if (paraTwoType == UNDERSCORE) {
			result = this->evaluateFixedUnderscore(paraOneValue);
		}
		else if (paraTwoType == FIXED) {
			result = this->evaluateFixedFixed(paraOneValue, paraTwoValue);
		}
		else {
			result = new Result();
			result->setPassed(false);
		}
	}
	else if (paraOneType == STATEMENT || paraOneType == WHILE || paraOneType == IF) {
		if (paraTwoType == STATEMENT || paraTwoType == READ || paraTwoType == PRINT
			|| paraTwoType == WHILE || paraTwoType == IF || paraTwoType == ASSIGN) {
			result = this->evaluateSynonymSynonym(paraOneValue, paraTwoValue, paraOneType, paraTwoType);
		}
		else if (paraTwoType == UNDERSCORE) {
			result = this->evaluateSynonymUnderscore(paraOneValue, paraOneType);
		}
		else if (paraTwoType == FIXED) {
			result = this->evaluateSynonymFixed(paraOneValue, paraTwoValue, paraOneType);
		}
		else {
			result = new Result();
			result->setPassed(false);
		}
	}
	else if (paraOneType == UNDERSCORE) {
		if (paraTwoType == STATEMENT || paraTwoType == READ || paraTwoType == PRINT
			|| paraTwoType == WHILE || paraTwoType == IF || paraTwoType == ASSIGN) {
			result = this->evaluateUnderscoreSynonym(paraTwoValue, paraTwoType);
		}
		else if (paraTwoType == UNDERSCORE) {
			result = this->evaluateUnderscoreUnderscore();
		}
		else if (paraTwoType == FIXED) {
			result = this->evaluateUnderscoreFixed(paraTwoValue);
		}
		else {
			result = new Result();
			result->setPassed(false);
		}
	}
	else {
		result = new Result();
		result->setPassed(false);
	}
	return *result;
}

// case Parent*(12, w)
Result* ParentT::evaluateFixedSynonym(const string& parentStmtNum, const string& childrenSynonym, const Type& childrenType) {
	Result* result = new Result();
	unordered_set<int> answer = pkb.getChildrenTOf(stoi(parentStmtNum), childrenType);
	if (!answer.empty()) {
		result->setPassed(true);
		result->setAnswer(childrenSynonym, answer);
	}
	else {
		result->setPassed(false);
	}
	return result;
}

// case Parent*(3, _)
Result* ParentT::evaluateFixedUnderscore(const string& parentStmtNum) {
	Result* result = new Result();
	result->setPassed(pkb.hasChildren(stoi(parentStmtNum)));
	return result;
}

// case Parent*(4, 6)
Result* ParentT::evaluateFixedFixed(const string& parentStmtNum, const string& childStmtNum) {
	Result* result = new Result();
	result->setPassed(pkb.isParentT(stoi(parentStmtNum), stoi(childStmtNum)));
	return result;
}

// case Parent*(i, a)
Result* ParentT::evaluateSynonymSynonym(const string& parentSynonym, const string& childSynonym, const Type& parentType, const Type& childType) {
	Result* result = new Result();
	unordered_map<int, unordered_set<int>> answer = pkb.getParentChildrenTPairs(parentType, childType);
	if (!answer.empty()) {
		result->setPassed(true);
		result->setAnswer(parentSynonym, childSynonym, answer);
	}
	else {
		result->setPassed(false);
	}
	return result;
}

// case Parent*(w, _)
Result* ParentT::evaluateSynonymUnderscore(const string& parentSynonym, const Type& parentType) {
	Result* result = new Result();
	unordered_set<int> answer = pkb.getParentStmts(parentType);
	if (!answer.empty()) {
		result->setPassed(true);
		result->setAnswer(parentSynonym, answer);
	}
	else {
		result->setPassed(false);
	}
	return result;
}

// case Parent*(i, 12)
Result* ParentT::evaluateSynonymFixed(const string& parentSynonym, const string& childStmtNum, const Type& parentType) {
	Result* result = new Result();
	unordered_set<int> answer = pkb.getParentTOf(stoi(childStmtNum), parentType);
	if (!answer.empty()) {
		result->setPassed(true);
		result->setAnswer(parentSynonym, answer);
	}
	else {
		result->setPassed(false);
	}
	return result;
}

// case Parent*(_, a)
Result* ParentT::evaluateUnderscoreSynonym(const string& childSynonym, const Type& childType) {
	Result* result = new Result();
	unordered_set<int> answer = pkb.getChildrenStmts(childType);
	if (!answer.empty()) {
		result->setPassed(true);
		result->setAnswer(childSynonym, answer);
	}
	else {
		result->setPassed(false);
	}
	return result;
}

// case Parent*(_, _)
Result* ParentT::evaluateUnderscoreUnderscore() {
	Result* result = new Result();
	unordered_set<int> parents = pkb.getParentStmts(Type::STATEMENT);
	result->setPassed(!parents.empty());
	return result;
}

// case Parent*(_, 23)
Result* ParentT::evaluateUnderscoreFixed(const string& childStmtNum) {
	Result* result = new Result();
	result->setPassed(pkb.hasParent(stoi(childStmtNum)));
	return result;
}