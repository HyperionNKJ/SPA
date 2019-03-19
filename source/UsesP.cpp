#include "UsesP.h"
#include <unordered_set>
#include <string>

UsesP::UsesP(const DesignEntity& paraOne, const DesignEntity& paraTwo) : Clause(paraOne, paraTwo, ClauseType::USES_P) {}

Result UsesP::evaluate(const PKB& pkb) {
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

// case Uses("main", v)
Result* UsesP::evaluateFixedVariable(const string& procName, const string& variableSynonym) {
	Result* result = new Result();
	unordered_set<string> answer = pkb.getVarUsedByProc(procName);
	if (!answer.empty()) {
		result->setPassed(true);
		result->setAnswer(variableSynonym, answer, pkb.getProcTable());
	}
	else {
		result->setPassed(false);
	}
	return result;
}

// case Uses("main", _)
Result* UsesP::evaluateFixedUnderscore(const string& procName) {
	Result* result = new Result();
	result->setPassed(pkb.doesProcUses(procName));
	return result;
}

// case Uses("main", "count")
Result* UsesP::evaluateFixedFixed(const string& procName, const string& varName) {
	Result* result = new Result();
	result->setPassed(pkb.isProcUses(procName, varName));
	return result;
}

// case Uses(p, v)
Result* UsesP::evaluateSynonymVariable(const string& procSynonym, const string& variableSynonym) {
	Result* result = new Result();
	unordered_map<string, unordered_set<string>> answer = pkb.getUsesProcVarPairs();
	if (!answer.empty()) {
		result->setPassed(true);
		result->setAnswer(procSynonym, variableSynonym, answer, pkb.getProcTable(), pkb.getVarTable());
	}
	else {
		result->setPassed(false);
	}
	return result;
}

// case Uses(p, _)
Result* UsesP::evaluateSynonymUnderscore(const string& procSynonym) {
	Result* result = new Result();
	unordered_set<string> answer = pkb.getProcThatUsesVar();
	if (!answer.empty()) {
		result->setPassed(true);
		result->setAnswer(procSynonym, answer, pkb.getProcTable());
	}
	else {
		result->setPassed(false);
	}
	return result;
}

// case Uses(p, "count")
Result* UsesP::evaluateSynonymFixed(const string& procSynonym, const string& varName) {
	Result* result = new Result();
	unordered_set<string> answer = pkb.getProcThatUsesVar(varName);
	if (!answer.empty()) {
		result->setPassed(true);
		result->setAnswer(procSynonym, answer, pkb.getProcTable());
	}
	else {
		result->setPassed(false);
	}
	return result;
}
