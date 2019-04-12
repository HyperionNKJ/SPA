#include "ModifiesS.h"
#include <unordered_set>
#include <string>

ModifiesS::ModifiesS(const DesignEntity& firstPara, const DesignEntity& secondPara) {
	paraOne = firstPara;
	paraTwo = secondPara;
	type = ClauseType::MODIFIES_S;
	setSynonyms();
}

Result ModifiesS::evaluate(const PKB& pkb) {
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
	else if (paraOneType == STATEMENT || paraOneType == READ || paraOneType == WHILE || paraOneType == IF 
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

// case Modifies(7, v)
Result ModifiesS::evaluateFixedVariable(const string& stmtNum, const string& variableSynonym) {
	Result result;
	unordered_set<string> answer = pkb.getVarModifiedByStmt(stoi(stmtNum));
	if (!answer.empty()) {
		result.setPassed(true);
		result.setAnswer(variableSynonym, answer, pkb.getVarTable());
	}
	else {
		result.setPassed(false);
	}
	return result;
}

// case Modifies(7, _)
Result ModifiesS::evaluateFixedUnderscore(const string& stmtNum) {
	Result result;
	result.setPassed(pkb.doesStmtModifies(stoi(stmtNum)));
	return result;
}

// case Modifies(7, "count")
Result ModifiesS::evaluateFixedFixed(const string& stmtNum, const string& varName) {
	Result result;
	result.setPassed(pkb.isModifies(stoi(stmtNum), varName));
	return result;
}

// case Modifies(w, v)
Result ModifiesS::evaluateSynonymVariable(const string& stmtSynonym, const string& variableSynonym, const Type& stmtType) {
	Result result;
	unordered_map<int, unordered_set<string>> answer = pkb.getModifiesStmtVarPairs(stmtType);
	if (!answer.empty()) {
		result.setPassed(true);
		result.setAnswer(stmtSynonym, variableSynonym, answer, pkb.getVarTable());
	}
	else {
		result.setPassed(false);
	}
	return result;
}

// case Modifies(a, _)
Result ModifiesS::evaluateSynonymUnderscore(const string& stmtSynonym, const Type& stmtType) {
	Result result;
	unordered_set<int> answer = pkb.getStmtsThatModifiesVar(stmtType);
	if (!answer.empty()) {
		result.setPassed(true);
		result.setAnswer(stmtSynonym, answer);
	}
	else {
		result.setPassed(false);
	}
	return result;
}

// case Modifies(w, "count")
Result ModifiesS::evaluateSynonymFixed(const string& stmtSynonym, const string& varName, const Type& stmtType) {
	Result result;
	unordered_set<int> answer = pkb.getStmtsThatModifiesVar(varName, stmtType);
	if (!answer.empty()) {
		result.setPassed(true);
		result.setAnswer(stmtSynonym, answer);
	}
	else {
		result.setPassed(false);
	}
	return result;
}
