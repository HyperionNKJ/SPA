#include "ModifiesS.h"
#include <unordered_set>
#include <string>

ModifiesS::ModifiesS(DesignEntity paraOne, DesignEntity paraTwo) : Clause(paraOne, paraTwo) {}

Result ModifiesS::evaluate(PKB pkb) {
	this->pkb = pkb;
	Type paraOneType = paraOne.getType();
	Type paraTwoType = paraTwo.getType();
	string paraOneValue = paraOne.getValue();
	string paraTwoValue = paraTwo.getValue();

	Result* result;

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
			result = new Result();
			result->setPassed(false);
		}
	} 
	else if (paraOneType == STATEMENT || paraOneType == READ || paraOneType == WHILE || paraOneType == IF || paraOneType == ASSIGN) {
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

// case Modifies(7, v)
Result* ModifiesS::evaluateFixedVariable(string stmtNum, string variableSynonym) {
	Result* result = new Result();
	unordered_set<string> answer = pkb.getVarModifiedByStmt(stoi(stmtNum));
	if (!answer.empty()) {
		result->setPassed(true);
		result->setAnswer(variableSynonym, answer);
	}
	else {
		result->setPassed(false);
	}
	return result;
}

// case Modifies(7, _)
Result* ModifiesS::evaluateFixedUnderscore(string stmtNum) {
	Result* result = new Result();
	result->setPassed(pkb.doesStmtModifies(stoi(stmtNum)));
	return result;
}

// case Modifies(7, "count")
Result* ModifiesS::evaluateFixedFixed(string stmtNum, string varName) {
	Result* result = new Result();
	result->setPassed(pkb.isModifies(stoi(stmtNum), varName));
	return result;
}

// case Modifies(w, v)
Result* ModifiesS::evaluateSynonymVariable(string stmtSynonym, string variableSynonym, Type stmtType) {
	Result* result = new Result();
	unordered_map<int, unordered_set<string>> answer = pkb.getModifiesStmtVarPairs(stmtType);
	if (!answer.empty()) {
		result->setPassed(true);
		result->setAnswer(stmtSynonym, variableSynonym, answer);
	}
	else {
		result->setPassed(false);
	}
	return result;
}

// case Modifies(a, _)
Result* ModifiesS::evaluateSynonymUnderscore(string stmtSynonym, Type stmtType) {
	Result* result = new Result();
	unordered_set<int> answer = pkb.getStmtsThatModifiesVar(stmtType);
	if (!answer.empty()) {
		result->setPassed(true);
		result->setAnswer(stmtSynonym, answer);
	}
	else {
		result->setPassed(false);
	}
	return result;
}

// case Modifies(w, "count")
Result* ModifiesS::evaluateSynonymFixed(string stmtSynonym, string varName, Type stmtType) {
	Result* result = new Result();
	unordered_set<int> answer = pkb.getStmtsThatModifiesVar(varName, stmtType);
	if (!answer.empty()) {
		result->setPassed(true);
		result->setAnswer(stmtSynonym, answer);
	}
	else {
		result->setPassed(false);
	}
	return result;
}
