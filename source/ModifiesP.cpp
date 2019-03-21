#include "ModifiesP.h"
#include <unordered_set>
#include <string>

ModifiesP::ModifiesP(const DesignEntity& paraOne, const DesignEntity& paraTwo) : Clause(paraOne, paraTwo, ClauseType::MODIFIES_P) {}

Result ModifiesP::evaluate(const PKB& pkb) {
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
	else if (paraOneType == PROCEDURE) {
		if (paraTwoType == VARIABLE) {
			result = this->evaluateSynonymVariable(paraOneValue, paraTwoValue);
		}
		else if (paraTwoType == UNDERSCORE) {
			result = this->evaluateSynonymUnderscore(paraOneValue);
		}
		else if (paraTwoType == FIXED) {
			result = this->evaluateSynonymFixed(paraOneValue, paraTwoValue);
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

// case Modifies("main", v)
Result* ModifiesP::evaluateFixedVariable(const string& procName, const string& variableSynonym) {
	Result* result = new Result();
	unordered_set<string> answer = pkb.getVarModifiedByProc(procName);
	if (!answer.empty()) {
		result->setPassed(true);
		result->setAnswer(variableSynonym, answer, pkb.getVarTable());
	}
	else {
		result->setPassed(false);
	}
	return result;
}

// case Modifies("main", _)
Result* ModifiesP::evaluateFixedUnderscore(const string& procName) {
	Result* result = new Result();
	result->setPassed(pkb.doesProcModifies(procName));
	return result;
}

// case Modifies("main", "count")
Result* ModifiesP::evaluateFixedFixed(const string& procName, const string& varName) {
	Result* result = new Result();
	result->setPassed(pkb.isProcModifies(procName, varName));
	return result;
}

// case Modifies(p, v)
Result* ModifiesP::evaluateSynonymVariable(const string& procSynonym, const string& variableSynonym) {
	Result* result = new Result();
	unordered_map<string, unordered_set<string>> answer = pkb.getModifiesProcVarPairs();
	if (!answer.empty()) {
		result->setPassed(true);
		result->setAnswer(procSynonym, variableSynonym, answer, pkb.getProcTable(), pkb.getVarTable());
	}
	else {
		result->setPassed(false);
	}
	return result;
}

// case Modifies(p, _)
Result* ModifiesP::evaluateSynonymUnderscore(const string& procSynonym) {
	Result* result = new Result();
	unordered_set<string> answer = pkb.getProcThatModifiesVar();
	if (!answer.empty()) {
		result->setPassed(true);
		result->setAnswer(procSynonym, answer, pkb.getProcTable());
	}
	else {
		result->setPassed(false);
	}
	return result;
}

// case Modifies(p, "count")
Result* ModifiesP::evaluateSynonymFixed(const string& procSynonym, const string& varName) {
	Result* result = new Result();
	unordered_set<string> answer = pkb.getProcThatModifiesVar(varName);
	if (!answer.empty()) {
		result->setPassed(true);
		result->setAnswer(procSynonym, answer, pkb.getProcTable());
	}
	else {
		result->setPassed(false);
	}
	return result;
}
