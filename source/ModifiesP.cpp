#include "ModifiesP.h"

ModifiesP::ModifiesP(const DesignEntity& firstPara, const DesignEntity& secondPara) {
	paraOne = firstPara;
	paraTwo = secondPara;
	type = ClauseType::MODIFIES_P;
	setSynonyms();
}

Result ModifiesP::evaluate(const PKB& pkb) {
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

// case Modifies("main", v)
Result ModifiesP::evaluateFixedVariable(const std::string& procName, const std::string& variableSynonym) {
	Result result;
	std::unordered_set<std::string> answer = pkb.getVarModifiedByProc(procName);
	if (!answer.empty()) {
		result.setPassed(true);
		result.setAnswer(variableSynonym, answer, pkb.getVarTable());
	}
	else {
		result.setPassed(false);
	}
	return result;
}

// case Modifies("main", _)
Result ModifiesP::evaluateFixedUnderscore(const std::string& procName) {
	Result result;
	result.setPassed(pkb.doesProcModifies(procName));
	return result;
}

// case Modifies("main", "count")
Result ModifiesP::evaluateFixedFixed(const std::string& procName, const std::string& varName) {
	Result result;
	result.setPassed(pkb.isProcModifies(procName, varName));
	return result;
}

// case Modifies(p, v)
Result ModifiesP::evaluateSynonymVariable(const std::string& procSynonym, const std::string& variableSynonym) {
	Result result;
	std::unordered_map<std::string, std::unordered_set<std::string>> answer = pkb.getModifiesProcVarPairs();
	if (!answer.empty()) {
		result.setPassed(true);
		result.setAnswer(procSynonym, variableSynonym, answer, pkb.getProcTable(), pkb.getVarTable());
	}
	else {
		result.setPassed(false);
	}
	return result;
}

// case Modifies(p, _)
Result ModifiesP::evaluateSynonymUnderscore(const std::string& procSynonym) {
	Result result;
	std::unordered_set<std::string> answer = pkb.getProcThatModifiesVar();
	if (!answer.empty()) {
		result.setPassed(true);
		result.setAnswer(procSynonym, answer, pkb.getProcTable());
	}
	else {
		result.setPassed(false);
	}
	return result;
}

// case Modifies(p, "count")
Result ModifiesP::evaluateSynonymFixed(const std::string& procSynonym, const std::string& varName) {
	Result result;
	std::unordered_set<std::string> answer = pkb.getProcThatModifiesVar(varName);
	if (!answer.empty()) {
		result.setPassed(true);
		result.setAnswer(procSynonym, answer, pkb.getProcTable());
	}
	else {
		result.setPassed(false);
	}
	return result;
}
