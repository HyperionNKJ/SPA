#include "UsesS.h"
#include <unordered_set>
#include <string>

UsesS::UsesS(const DesignEntity& firstPara, const DesignEntity& secondPara) {
	paraOne = firstPara;
	paraTwo = secondPara;
	type = USES_S;
	setSynonyms();
}

Result UsesS::evaluate(const PKB& pkb) {
	this->pkb = pkb;
	Type paraOneType = paraOne.getType();
	Type paraTwoType = paraTwo.getType();
	string paraOneValue = paraOne.getValue();
	string paraTwoValue = paraTwo.getValue();

	Result result;

	if (paraOneType == FIXED) {
		if (paraTwoType == VARIABLE) {
			result = this->evaluateFixedVariable(paraOneValue, paraTwoValue);
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
	else if (paraOneType == STATEMENT || paraOneType == PRINT || paraOneType == WHILE || paraOneType == IF 
		|| paraOneType == ASSIGN || paraOneType == CALL || paraOneType == PROGLINE || paraOneType == SWITCH) {
		if (paraTwoType == VARIABLE) {
			result = this->evaluateSynonymVariable(paraOneValue, paraTwoValue, paraOneType);
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
	else {
		result.setPassed(false);
	}
	return result;
}

// case Uses(7, v)
Result UsesS::evaluateFixedVariable(const string& stmtNum, const string& variableSynonym) {
	Result result;
	unordered_set<string> answer = pkb.getVarUsedByStmt(stoi(stmtNum));
	if (!answer.empty()) {
		result.setPassed(true);
		result.setAnswer(variableSynonym, answer, pkb.getVarTable());
	}
	else {
		result.setPassed(false);
	}
	return result;
}

// case Uses(7, _)
Result UsesS::evaluateFixedUnderscore(const string& stmtNum) {
	Result result;
	result.setPassed(pkb.doesStmtUses(stoi(stmtNum)));
	return result;
}

// case Uses(7, "count")
Result UsesS::evaluateFixedFixed(const string& stmtNum, const string& varName) {
	Result result;
	result.setPassed(pkb.isUses(stoi(stmtNum), varName));
	return result;
}

// case Uses(w, v)
Result UsesS::evaluateSynonymVariable(const string& stmtSynonym, const string& variableSynonym, const Type& stmtType) {
	Result result;
	unordered_map<int, unordered_set<string>> answer = pkb.getUsesStmtVarPairs(stmtType);
	if (!answer.empty()) {
		result.setPassed(true);
		result.setAnswer(stmtSynonym, variableSynonym, answer, pkb.getVarTable());
	}
	else {
		result.setPassed(false);
	}
	return result;
}

// case Uses(a, _)
Result UsesS::evaluateSynonymUnderscore(const string& stmtSynonym, const Type& stmtType) {
	Result result;
	unordered_set<int> answer = pkb.getStmtsThatUsesVar(stmtType);
	if (!answer.empty()) {
		result.setPassed(true);
		result.setAnswer(stmtSynonym, answer);
	}
	else {
		result.setPassed(false);
	}
	return result;
}

// e.g. Uses(w, "count")
Result UsesS::evaluateSynonymFixed(const string& stmtSynonym, const string& varName, const Type& stmtType) {
	Result result;
	unordered_set<int> answer = pkb.getStmtsThatUsesVar(varName, stmtType);
	if (!answer.empty()) {
		result.setPassed(true);
		result.setAnswer(stmtSynonym, answer);
	}
	else {
		result.setPassed(false);
	}
	return result;
}
