#include "UsesP.h"

UsesP::UsesP(const DesignEntity& firstPara, const DesignEntity& secondPara) {
	paraOne = firstPara;
	paraTwo = secondPara;
	type = USES_P;
	setSynonyms();
}

Result UsesP::evaluate(const PKB& pkb) {
	this->pkb = pkb;
	Type paraOneType = paraOne.getType();
	Type paraTwoType = paraTwo.getType();
	std::string paraOneValue = paraOne.getValue();
	std::string paraTwoValue = paraTwo.getValue();

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
			result.setPassed(false);
		}
	}
	else {
		result.setPassed(false);
	}
	return result;
}

// case Uses("main", v)
Result UsesP::evaluateFixedVariable(const std::string& procName, const std::string& variableSynonym) {
	Result result;
	std::unordered_set<std::string> answer = pkb.getVarUsedByProc(procName);
	if (!answer.empty()) {
		result.setPassed(true);
		result.setAnswer(variableSynonym, answer, pkb.getVarTable());
	}
	else {
		result.setPassed(false);
	}
	return result;
}

// case Uses("main", _)
Result UsesP::evaluateFixedUnderscore(const std::string& procName) {
	Result result;
	result.setPassed(pkb.doesProcUses(procName));
	return result;
}

// case Uses("main", "count")
Result UsesP::evaluateFixedFixed(const std::string& procName, const std::string& varName) {
	Result result;
	result.setPassed(pkb.isProcUses(procName, varName));
	return result;
}

// case Uses(p, v)
Result UsesP::evaluateSynonymVariable(const std::string& procSynonym, const std::string& variableSynonym) {
	Result result;
	std::unordered_map<std::string, std::unordered_set<std::string>> answer = pkb.getUsesProcVarPairs();
	if (!answer.empty()) {
		result.setPassed(true);
		result.setAnswer(procSynonym, variableSynonym, answer, pkb.getProcTable(), pkb.getVarTable());
	}
	else {
		result.setPassed(false);
	}
	return result;
}

// case Uses(p, _)
Result UsesP::evaluateSynonymUnderscore(const std::string& procSynonym) {
	Result result;
	std::unordered_set<std::string> answer = pkb.getProcThatUsesVar();
	if (!answer.empty()) {
		result.setPassed(true);
		result.setAnswer(procSynonym, answer, pkb.getProcTable());
	}
	else {
		result.setPassed(false);
	}
	return result;
}

// case Uses(p, "count")
Result UsesP::evaluateSynonymFixed(const std::string& procSynonym, const std::string& varName) {
	Result result;
	std::unordered_set<std::string> answer = pkb.getProcThatUsesVar(varName);
	if (!answer.empty()) {
		result.setPassed(true);
		result.setAnswer(procSynonym, answer, pkb.getProcTable());
	}
	else {
		result.setPassed(false);
	}
	return result;
}
