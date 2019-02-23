#include "Pattern.h"

Pattern::Pattern(DesignEntity subject, DesignEntity paraOne, DesignEntity paraTwo) : Clause(paraOne, paraTwo) {
	this->subject = subject;
}

Result Pattern::evaluate(PKB pkb) {
	this->pkb = pkb;

	Type subjectType = subject.getType(); // should be an assign type
	Type paraOneType = paraOne.getType();
	Type paraTwoType = paraTwo.getType();
	string subjectValue = subject.getValue();
	string paraOneValue = paraOne.getValue();
	string paraTwoValue = paraTwo.getValue();

	Result* result;

	if (paraOneType == VARIABLE) {
		if (paraTwoType == FIXED) {
			result = this->evaluateVariableFixed(paraOneValue, paraTwoValue, subjectValue);
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
		if (paraTwoType == FIXED) {
			result = this->evaluateUnderscoreFixed(paraTwoValue, subjectValue);
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
		if (paraTwoType == FIXED) {
			result = this->evaluateFixedFixed(paraOneValue, paraTwoValue, subjectValue);
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

// case a(v, _"count"_)
Result* Pattern::evaluateVariableFixed(string variableSynonym, string entity, string subjectSynonym) {
	Result* result = new Result();
	unordered_set<int> assignStmtsThatUseEntity = pkb.getAssignStmtsThatUse(entity);
	unordered_map<int, string> answer;

	for (auto assignStmt : assignStmtsThatUseEntity) {
		string variableModified = pkb.getVarModifiedByAssignStmt(assignStmt);
		answer.insert({ { assignStmt,variableModified } });
	}

	if (!answer.empty()) {
		result->setPassed(true);
		result->setAnswer(subjectSynonym, variableSynonym, answer);
	}
	else {
		result->setPassed(false);
	}
	return result;
}

// case a(v, _) = Modifies(a, v)
Result* Pattern::evaluateVariableUnderscore(string variableSynonym, string subjectSynonym) {
	Result* result = new Result();
	unordered_map<int, unordered_set<string>> answer = pkb.getModifiesStmtVarPairs(Type::ASSIGN);
	if (!answer.empty()) {
		result->setPassed(true);
		result->setAnswer(subjectSynonym, variableSynonym, answer);
	}
	else {
		result->setPassed(false);
	}
	return result;
}

// case a(_, _"12"_)
Result* Pattern::evaluateUnderscoreFixed(string entity, string subjectSynonym) {
	Result* result = new Result();
	unordered_set<int> answer = pkb.getAssignStmtsThatUse(entity);
	if (!answer.empty()) {
		result->setPassed(true);
		result->setAnswer(subjectSynonym, answer);
	}
	else {
		result->setPassed(false);
	}
	return result;
}

// case a(_, _) = All assign statements, since they must have a LHS and RHS
Result* Pattern::evaluateUnderscoreUnderscore(string subjectSynonym) {
	Result* result = new Result();
	unordered_set<int> answer = pkb.getAssignStmts(); 
	if (!answer.empty()) {
		result->setPassed(true);
		result->setAnswer(subjectSynonym, answer);
	}
	else {
		result->setPassed(false);
	}
	return result;
}

// case a("count", _"12"_)
Result* Pattern::evaluateFixedFixed(string variable, string entity, string subjectSynonym) {
	Result* result = new Result();
	unordered_set<int> assignStmtsThatModifiesVar = pkb.getAssignStmtsThatModifiesVar(variable);
	unordered_set<int> answer;

	for (auto assignStmt : assignStmtsThatModifiesVar) {
		if (pkb.doesAssignStmtUse(assignStmt, entity)) {
			answer.insert(assignStmt);
		}
	}
	if (!answer.empty()) {
		result->setPassed(true);
		result->setAnswer(subjectSynonym, answer);
	}
	else {
		result->setPassed(false);
	}
	return result;
}

// case a("count", _) = Modifies(a, "count")
Result* Pattern::evaluateFixedUnderscore(string variable, string subjectSynonym) {
	Result* result = new Result();
	unordered_set<int> answer = pkb.getStmtsThatModifiesVar(variable, Type::ASSIGN);
	if (!answer.empty()) {
		result->setPassed(true);
		result->setAnswer(subjectSynonym, answer);
	}
	else {
		result->setPassed(false);
	}
	return result;
}