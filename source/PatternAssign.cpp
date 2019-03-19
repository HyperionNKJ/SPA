#include "PatternAssign.h"

PatternAssign::PatternAssign(const DesignEntity& subject, const DesignEntity& paraOne, const DesignEntity& paraTwo) : Clause(paraOne, paraTwo, ClauseType::PATTERN_ASSIGN) {
	this->subject = subject;
}

Result PatternAssign::evaluate(const PKB& pkb) {
	this->pkb = pkb;

	Type subjectType = subject.getType(); // should be an assign type
	Type paraOneType = paraOne.getType();
	Type paraTwoType = paraTwo.getType();
	string subjectValue = subject.getValue();
	string paraOneValue = paraOne.getValue();
	string paraTwoValue = paraTwo.getValue();

	Result* result;

	if (subjectType != ASSIGN) {
		result = new Result();
		result->setPassed(false);
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
			result = new Result();
			result->setPassed(false);
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
			result = new Result();
			result->setPassed(false);
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

// case a(v, "x+y+10")
Result* PatternAssign::evaluateVariableExact(const string& variableSynonym, const string& exactString, const string& assignSynonym) {
	Result* result = new Result();
	unordered_set<int> assignStmtsWithExactMatch = pkb.getAssignStmtWithExactMatch(exactString); 
	unordered_map<int, string> answer;

	for (auto a : assignStmtsWithExactMatch) {
		string variableModified = pkb.getVarModifiedByAssignStmt(a);
		answer.insert({ { a, variableModified } });
	}

	if (!answer.empty()) {
		result->setPassed(true);
		result->setAnswer(assignSynonym, variableSynonym, answer, pkb.getVarTable());
	}
	else {
		result->setPassed(false);
	}
	return result;
}

// case a(v, _"x+y+10"_)
Result* PatternAssign::evaluateVariableSub(const string& variableSynonym, const string& subString, const string& assignSynonym) {
	Result* result = new Result();
	unordered_set<int> assignStmtsWithSubMatch = pkb.getAssignStmtWithSubMatch(subString);
	unordered_map<int, string> answer;

	for (auto a : assignStmtsWithSubMatch) {
		string variableModified = pkb.getVarModifiedByAssignStmt(a);
		answer.insert({ { a, variableModified } });
	}

	if (!answer.empty()) {
		result->setPassed(true);
		result->setAnswer(assignSynonym, variableSynonym, answer, pkb.getVarTable());
	}
	else {
		result->setPassed(false);
	}
	return result;
}

// case a(v, _) = Modifies(a, v)
Result* PatternAssign::evaluateVariableUnderscore(const string& variableSynonym, const string& assignSynonym) {
	Result* result = new Result();
	unordered_map<int, unordered_set<string>> answer = pkb.getModifiesStmtVarPairs(Type::ASSIGN);
	if (!answer.empty()) {
		result->setPassed(true);
		result->setAnswer(assignSynonym, variableSynonym, answer, pkb.getVarTable());
	}
	else {
		result->setPassed(false);
	}
	return result;
}

// case a(_, "count")
Result* PatternAssign::evaluateUnderscoreExact(const string& exactString, const string& assignSynonym) {
	Result* result = new Result();
	unordered_set<int> answer = pkb.getAssignStmtWithExactMatch(exactString);

	if (!answer.empty()) {
		result->setPassed(true);
		result->setAnswer(assignSynonym, answer);
	}
	else {
		result->setPassed(false);
	}
	return result;
}

// case a(_, _"count+10"_)
Result* PatternAssign::evaluateUnderscoreSub(const string& subString, const string& assignSynonym) {
	Result* result = new Result();
	unordered_set<int> answer = pkb.getAssignStmtWithSubMatch(subString);

	if (!answer.empty()) {
		result->setPassed(true);
		result->setAnswer(assignSynonym, answer);
	}
	else {
		result->setPassed(false);
	}
	return result;
}

// case a(_, _) = All assign statements, since they must have a LHS and RHS
Result* PatternAssign::evaluateUnderscoreUnderscore(const string& assignSynonym) {
	Result* result = new Result();
	unordered_set<int> answer = pkb.getAssignStmts();
	if (!answer.empty()) {
		result->setPassed(true);
		result->setAnswer(assignSynonym, answer);
	}
	else {
		result->setPassed(false);
	}
	return result;
}

// case a("x", "x+1")
Result* PatternAssign::evaluateFixedExact(const string& varName, const string& exactString, const string& assignSynonym) {
	Result* result = new Result();
	unordered_set<int> answer;

	unordered_set<int> assignStmtsWithExactMatch = pkb.getAssignStmtWithExactMatch(exactString);
	for (auto a : assignStmtsWithExactMatch) {
		if (pkb.isModifies(a, varName)) {
			answer.insert(a);
		}
	}

	if (!answer.empty()) {
		result->setPassed(true);
		result->setAnswer(assignSynonym, answer);
	}
	else {
		result->setPassed(false);
	}
	return result;
}

// case a("x", _"y*x+z"_)
Result* PatternAssign::evaluateFixedSub(const string& varName, const string& subString, const string& assignSynonym) {
	Result* result = new Result();
	unordered_set<int> answer;

	unordered_set<int> assignStmtsWithSubMatch = pkb.getAssignStmtWithSubMatch(subString);
	for (auto a : assignStmtsWithSubMatch) {
		if (pkb.isModifies(a, varName)) {
			answer.insert(a);
		}
	}

	if (!answer.empty()) {
		result->setPassed(true);
		result->setAnswer(assignSynonym, answer);
	}
	else {
		result->setPassed(false);
	}
	return result;
}

// case a("i", _) = Modifies(a, "i")
Result* PatternAssign::evaluateFixedUnderscore(const string& varName, const string& assignSynonym) {
	Result* result = new Result();
	unordered_set<int> answer = pkb.getStmtsThatModifiesVar(varName, Type::ASSIGN);

	if (!answer.empty()) {
		result->setPassed(true);
		result->setAnswer(assignSynonym, answer);
	}
	else {
		result->setPassed(false);
	}
	return result;
}
