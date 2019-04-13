#include "PatternAssign.h"

PatternAssign::PatternAssign(const DesignEntity& patternSubject, const DesignEntity& firstPara, const DesignEntity& secondPara) {
	paraOne = firstPara;
	paraTwo = secondPara;
	subject = patternSubject;
	type = ClauseType::PATTERN_ASSIGN;
	setSynonyms();
}

Result PatternAssign::evaluate(const PKB& pkb) {
	this->pkb = pkb;

	Type subjectType = subject.getType(); // should be an assign type
	Type paraOneType = paraOne.getType();
	Type paraTwoType = paraTwo.getType();
	std::string subjectValue = subject.getValue();
	std::string paraOneValue = paraOne.getValue();
	std::string paraTwoValue = paraTwo.getValue();

	Result result;

	if (subjectType != ASSIGN) {
		result.setPassed(false);
	}

	if (paraOneType == VARIABLE) {
		if (paraTwoType == PATTERN_EXACT) {
			result = this->evaluateVariableExact(paraOneValue, paraTwoValue, subjectValue);
		}
		else if (paraTwoType == PATTERN_SUB) {
			result = this->evaluateVariableSub(paraOneValue, paraTwoValue, subjectValue);
		}
		else if (paraTwoType == UNDERSCORE) {
			result = this->evaluateVariableUnderscore(paraOneValue, subjectValue);
		}
		else {
			result.setPassed(false);
		}
	}
	else if (paraOneType == UNDERSCORE) {
		if (paraTwoType == PATTERN_EXACT) {
			result = this->evaluateUnderscoreExact(paraTwoValue, subjectValue);
		}
		else if (paraTwoType == PATTERN_SUB) {
			result = this->evaluateUnderscoreSub(paraTwoValue, subjectValue);
		}
		else if (paraTwoType == UNDERSCORE) {
			result = this->evaluateUnderscoreUnderscore(subjectValue);
		}
		else {
			result.setPassed(false);
		}
	}
	else if (paraOneType == FIXED) {
		if (paraTwoType == PATTERN_EXACT) {
			result = this->evaluateFixedExact(paraOneValue, paraTwoValue, subjectValue);
		}
		else if (paraTwoType == PATTERN_SUB) {
			result = this->evaluateFixedSub(paraOneValue, paraTwoValue, subjectValue);
		}
		else if (paraTwoType == UNDERSCORE) {
			result = this->evaluateFixedUnderscore(paraOneValue, subjectValue);
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

// case a(v, "x+y+10")
Result PatternAssign::evaluateVariableExact(const std::string& variableSynonym, const std::string& exactString, const std::string& assignSynonym) {
	Result result;
	std::unordered_set<int> assignStmtsWithExactMatch = pkb.getAssignStmtsWithExactMatch(exactString);
	std::unordered_map<int, std::string> answer;

	for (auto a : assignStmtsWithExactMatch) {
		std::string variableModified = pkb.getVarModifiedByAssignStmt(a);
		answer.insert({ { a, variableModified } });
	}

	if (!answer.empty()) {
		result.setPassed(true);
		result.setAnswer(assignSynonym, variableSynonym, answer, pkb.getVarTable());
	}
	else {
		result.setPassed(false);
	}
	return result;
}

// case a(v, _"x+y+10"_)
Result PatternAssign::evaluateVariableSub(const std::string& variableSynonym, const std::string& subString, const std::string& assignSynonym) {
	Result result;
	std::unordered_set<int> assignStmtsWithSubMatch = pkb.getAssignStmtsWithSubMatch(subString);
	std::unordered_map<int, std::string> answer;

	for (auto a : assignStmtsWithSubMatch) {
		std::string variableModified = pkb.getVarModifiedByAssignStmt(a);
		answer.insert({ { a, variableModified } });
	}

	if (!answer.empty()) {
		result.setPassed(true);
		result.setAnswer(assignSynonym, variableSynonym, answer, pkb.getVarTable());
	}
	else {
		result.setPassed(false);
	}
	return result;
}

// case a(v, _) = Modifies(a, v)
Result PatternAssign::evaluateVariableUnderscore(const std::string& variableSynonym, const std::string& assignSynonym) {
	Result result;
	std::unordered_map<int, std::unordered_set<std::string>> answer = pkb.getModifiesStmtVarPairs(Type::ASSIGN);
	if (!answer.empty()) {
		result.setPassed(true);
		result.setAnswer(assignSynonym, variableSynonym, answer, pkb.getVarTable());
	}
	else {
		result.setPassed(false);
	}
	return result;
}

// case a(_, "count")
Result PatternAssign::evaluateUnderscoreExact(const std::string& exactString, const std::string& assignSynonym) {
	Result result;
	std::unordered_set<int> answer = pkb.getAssignStmtsWithExactMatch(exactString);

	if (!answer.empty()) {
		result.setPassed(true);
		result.setAnswer(assignSynonym, answer);
	}
	else {
		result.setPassed(false);
	}
	return result;
}

// case a(_, _"count+10"_)
Result PatternAssign::evaluateUnderscoreSub(const std::string& subString, const std::string& assignSynonym) {
	Result result;
	std::unordered_set<int> answer = pkb.getAssignStmtsWithSubMatch(subString);

	if (!answer.empty()) {
		result.setPassed(true);
		result.setAnswer(assignSynonym, answer);
	}
	else {
		result.setPassed(false);
	}
	return result;
}

// case a(_, _) = All assign statements, since they must have a LHS and RHS
Result PatternAssign::evaluateUnderscoreUnderscore(const std::string& assignSynonym) {
	Result result;
	std::unordered_set<int> answer = pkb.getAssignStmts();
	if (!answer.empty()) {
		result.setPassed(true);
		result.setAnswer(assignSynonym, answer);
	}
	else {
		result.setPassed(false);
	}
	return result;
}

// case a("x", "x+1")
Result PatternAssign::evaluateFixedExact(const std::string& varName, const std::string& exactString, const std::string& assignSynonym) {
	Result result;
	std::unordered_set<int> answer;

	std::unordered_set<int> assignStmtsWithExactMatch = pkb.getAssignStmtsWithExactMatch(exactString);
	for (auto a : assignStmtsWithExactMatch) {
		if (pkb.isModifies(a, varName)) {
			answer.insert(a);
		}
	}

	if (!answer.empty()) {
		result.setPassed(true);
		result.setAnswer(assignSynonym, answer);
	}
	else {
		result.setPassed(false);
	}
	return result;
}

// case a("x", _"y*x+z"_)
Result PatternAssign::evaluateFixedSub(const std::string& varName, const std::string& subString, const std::string& assignSynonym) {
	Result result;
	std::unordered_set<int> answer;

	std::unordered_set<int> assignStmtsWithSubMatch = pkb.getAssignStmtsWithSubMatch(subString);
	for (auto a : assignStmtsWithSubMatch) {
		if (pkb.isModifies(a, varName)) {
			answer.insert(a);
		}
	}

	if (!answer.empty()) {
		result.setPassed(true);
		result.setAnswer(assignSynonym, answer);
	}
	else {
		result.setPassed(false);
	}
	return result;
}

// case a("i", _) = Modifies(a, "i")
Result PatternAssign::evaluateFixedUnderscore(const std::string& varName, const std::string& assignSynonym) {
	Result result;
	std::unordered_set<int> answer = pkb.getStmtsThatModifiesVar(varName, Type::ASSIGN);

	if (!answer.empty()) {
		result.setPassed(true);
		result.setAnswer(assignSynonym, answer);
	}
	else {
		result.setPassed(false);
	}
	return result;
}
