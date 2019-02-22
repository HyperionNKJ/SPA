#include "ParentT.h"

ParentT::ParentT(DesignEntity paraOne, DesignEntity paraTwo) : Clause(paraOne, paraTwo) {}

Result ParentT::evaluate(PKB pkb) {
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
Result* ParentT::evaluateFixedSynonym(string parentStmtNum, string childrenSynonym, Type childrenType) {
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
Result* ParentT::evaluateFixedUnderscore(string parentStmtNum) {
	Result* result = new Result();
	result->setPassed(pkb.hasChildren(stoi(parentStmtNum)));
	return result;
}

// case Parent*(4, 6)
Result* ParentT::evaluateFixedFixed(string parentStmtNum, string childStmtNum) {
	Result* result = new Result();
	result->setPassed(pkb.isParentT(stoi(parentStmtNum), stoi(childStmtNum)));
	return result;
}

// case Parent*(i, a)
Result* ParentT::evaluateSynonymSynonym(string parentSynonym, string childSynonym, Type parentType, Type childType) {
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
Result* ParentT::evaluateSynonymUnderscore(string parentSynonym, Type parentType) {
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
Result* ParentT::evaluateSynonymFixed(string parentSynonym, string childStmtNum, Type parentType) {
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
Result* ParentT::evaluateUnderscoreSynonym(string childSynonym, Type childType) {
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
Result* ParentT::evaluateUnderscoreFixed(string childStmtNum) {
	Result* result = new Result();
	result->setPassed(pkb.hasParent(stoi(childStmtNum)));
	return result;
}
