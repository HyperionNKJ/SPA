#include "Parent.h"

Parent::Parent(const DesignEntity& firstPara, const DesignEntity& secondPara) {
	paraOne = firstPara;
	paraTwo = secondPara;
	type = ClauseType::PARENT;
	setSynonyms();
}

Result Parent::evaluate(const PKB& pkb) {
	this->pkb = pkb;
	Type paraOneType = paraOne.getType();
	Type paraTwoType = paraTwo.getType();
	std::string paraOneValue = paraOne.getValue();
	std::string paraTwoValue = paraTwo.getValue();

	Result result;

	if (paraOneType == FIXED) {
		if (paraTwoType == STATEMENT || paraTwoType == READ || paraTwoType == PRINT || paraTwoType == WHILE || paraTwoType == IF 
			|| paraTwoType == ASSIGN || paraTwoType == CALL || paraTwoType == PROGLINE || paraTwoType == SWITCH) {
			result = this->evaluateFixedSynonym(paraOneValue, paraTwoValue, paraTwoType);
		}
		else if (paraTwoType == UNDERSCORE) {
			result = this->evaluateFixedUnderscore(paraOneValue);
		}
		else if (paraTwoType == FIXED) {
			result = this->evaluateFixedFixed(paraOneValue, paraTwoValue);
		}
		else {
			result.setPassed(false);
		}
	}
	else if (paraOneType == STATEMENT || paraOneType == WHILE || paraOneType == IF || paraOneType == PROGLINE || paraOneType == SWITCH) {
		if (paraTwoType == STATEMENT || paraTwoType == READ || paraTwoType == PRINT || paraTwoType == WHILE || paraTwoType == IF 
			|| paraTwoType == ASSIGN || paraTwoType == CALL || paraTwoType == PROGLINE || paraTwoType == SWITCH) {
			result = this->evaluateSynonymSynonym(paraOneValue, paraTwoValue, paraOneType, paraTwoType);
		}
		else if (paraTwoType == UNDERSCORE) {
			result = this->evaluateSynonymUnderscore(paraOneValue, paraOneType);
		}
		else if (paraTwoType == FIXED) {
			result = this->evaluateSynonymFixed(paraOneValue, paraTwoValue, paraOneType);
		}
		else {
			result.setPassed(false);
		}
	}
	else if (paraOneType == UNDERSCORE) {
		if (paraTwoType == STATEMENT || paraTwoType == READ || paraTwoType == PRINT || paraTwoType == WHILE || paraTwoType == IF 
			|| paraTwoType == ASSIGN || paraTwoType == CALL || paraTwoType == PROGLINE || paraTwoType == SWITCH) {
			result = this->evaluateUnderscoreSynonym(paraTwoValue, paraTwoType);
		}
		else if (paraTwoType == UNDERSCORE) {
			result = this->evaluateUnderscoreUnderscore();
		}
		else if (paraTwoType == FIXED) {
			result = this->evaluateUnderscoreFixed(paraTwoValue);
		}
		else {
			result.setPassed(false);
		}
	}
	else {
		result.setPassed(false);
	}
	return result;
}

// case Parent(12, w)
Result Parent::evaluateFixedSynonym(const std::string& parentStmtNum, const std::string& childrenSynonym, const Type& childrenType) {
	Result result;
	std::unordered_set<int> answer = pkb.getChildrenOf(stoi(parentStmtNum), childrenType);
	if (!answer.empty()) {
		result.setPassed(true);
		result.setAnswer(childrenSynonym, answer);
	}
	else {
		result.setPassed(false);
	}
	return result;
}

// case Parent(3, _)
Result Parent::evaluateFixedUnderscore(const std::string& parentStmtNum) {
	Result result;
	result.setPassed(pkb.hasChildren(stoi(parentStmtNum)));
	return result;
}

// case Parent(4, 6)
Result Parent::evaluateFixedFixed(const std::string& parentStmtNum, const std::string& childStmtNum) {
	Result result;
	result.setPassed(pkb.isParent(stoi(parentStmtNum), stoi(childStmtNum)));
	return result;
}

// case Parent(i, a)
Result Parent::evaluateSynonymSynonym(const std::string& parentSynonym, const std::string& childSynonym, const Type& parentType, const Type& childType) {
	Result result;
	if (parentSynonym == childSynonym) {
		result.setPassed(false);
		return result;
	}
	std::unordered_map<int, std::unordered_set<int>> answer = pkb.getParentChildrenPairs(parentType, childType);
	if (!answer.empty()) {
		result.setPassed(true);
		result.setAnswer(parentSynonym, childSynonym, answer);
	}
	else {
		result.setPassed(false);
	}
	return result;
}

// case Parent(w, _)
Result Parent::evaluateSynonymUnderscore(const std::string& parentSynonym, const Type& parentType) {
	Result result;
	std::unordered_set<int> answer = pkb.getParentStmts(parentType);
	if (!answer.empty()) {
		result.setPassed(true);
		result.setAnswer(parentSynonym, answer);
	}
	else {
		result.setPassed(false);
	}
	return result;
}

// case Parent(i, 12)
Result Parent::evaluateSynonymFixed(const std::string& parentSynonym, const std::string& childStmtNum, const Type& parentType) {
	Result result;
	int answer = pkb.getParentOf(stoi(childStmtNum), parentType);
	if (answer != -1) {
		result.setPassed(true);
		result.setAnswer(parentSynonym, answer);
	}
	else {
		result.setPassed(false);
	}
	return result;
}

// case Parent(_, a)
Result Parent::evaluateUnderscoreSynonym(const std::string& childSynonym, const Type& childType) {
	Result result;
	std::unordered_set<int> answer = pkb.getChildrenStmts(childType);
	if (!answer.empty()) {
		result.setPassed(true);
		result.setAnswer(childSynonym, answer);
	}
	else {
		result.setPassed(false);
	}
	return result;
}

// case Parent(_, _)
Result Parent::evaluateUnderscoreUnderscore() {
	Result result;
	std::unordered_set<int> parents = pkb.getParentStmts(Type::STATEMENT);
	result.setPassed(!parents.empty());
	return result;
}

// case Parent(_, 23)
Result Parent::evaluateUnderscoreFixed(const std::string& childStmtNum) {
	Result result;
	result.setPassed(pkb.hasParent(stoi(childStmtNum)));
	return result;
}
