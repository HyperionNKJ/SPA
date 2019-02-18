#include "UsesS.h"
#include <unordered_set>
#include <string>

UsesS::UsesS(DesignEntity paraOne, DesignEntity paraTwo) : Clause(paraOne, paraTwo) {}

Result UsesS::evaluate() {
	Type paraOneType = paraOne.getType();
	Type paraTwoType = paraTwo.getType();
	string paraOneValue = paraOne.getValue();
	string paraTwoValue = paraTwo.getValue();

	Result* result;

	if (paraOneType == FIXED) {
		if (paraTwoType == VARIABLE) {
			result = evaluateFixedVariable(paraOneValue, paraTwoValue);
		}
		else if (paraTwoType == UNDERSCORE) {
			result = evaluateFixedUnderscore(paraOneValue);
		}
		else if (paraTwoType == FIXED) {
			result = evaluateFixedFixed(paraOneValue, paraTwoValue);
		}
		else {
			result = new Result();
			result->setPassed(false);
		}
	}
	else if (paraOneType == STATEMENT || paraOneType == PRINT || paraOneType == WHILE || paraOneType == IF || paraOneType == ASSIGN) {
		if (paraTwoType == VARIABLE) {
			result = evaluateSynonymVariable(paraOneValue, paraTwoValue, paraOneType);
		}
		else if (paraTwoType == UNDERSCORE) {
			result = evaluateSynonymUnderscore(paraOneValue, paraOneType);
		}
		else if (paraTwoType == FIXED) {
			result = evaluateSynonymFixed(paraOneValue, paraTwoValue, paraOneType);
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

// case Uses(7, v)
Result* UsesS::evaluateFixedVariable(string stmtNum, string variableSynonym) {
	Result* result = new Result();
	unordered_set<string> answer = getVariablesUsedByStatement(stoi(stmtNum));
	if (!answer.empty()) {
		result->setPassed(true);
		result->setAnswer(variableSynonym, answer);
	}
	else {
		result->setPassed(false);
	}
	return result;
}

// case Uses(7, _)
Result* UsesS::evaluateFixedUnderscore(string stmtNum) {
	Result* result = new Result();
	result->setPassed(doesStatementUses(stoi(stmtNum)));
	return result;
}

// case Uses(7, "count")
Result* UsesS::evaluateFixedFixed(string stmtNum, string varName) {
	Result* result = new Result();
	result->setPassed(isUses(stoi(stmtNum), varName));
	return result;
}

// case Uses(w, v)
Result* UsesS::evaluateSynonymVariable(string stmtSynonym, string variableSynonym, Type stmtType) {
	Result* result = new Result();
	unordered_map<int, unordered_set<string>> answer = getUsesStatementVariablesPairs(stmtType);
	if (!answer.empty()) {
		result->setPassed(true);
		result->setAnswer(stmtSynonym, variableSynonym, answer);
	}
	else {
		result->setPassed(false);
	}
	return result;
}

// case Uses(a, _)
Result* UsesS::evaluateSynonymUnderscore(string stmtSynonym, Type stmtType) {
	Result* result = new Result();
	unordered_set<int> answer = getStatementsThatUses(stmtType);
	if (!answer.empty()) {
		result->setPassed(true);
		result->setAnswer(stmtSynonym, answer);
	}
	else {
		result->setPassed(false);
	}
	return result;
}

// e.g. Uses(w, "count")
Result* UsesS::evaluateSynonymFixed(string stmtSynonym, string varName, Type stmtType) {
	Result* result = new Result();
	unordered_set<int> answer = getStatementsThatUsesVariable(varName, stmtType);
	if (!answer.empty()) {
		result->setPassed(true);
		result->setAnswer(stmtSynonym, answer);
	}
	else {
		result->setPassed(false);
	}
	return result;
}
