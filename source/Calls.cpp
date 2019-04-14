#include "Calls.h"

Calls::Calls(const DesignEntity& firstPara, const DesignEntity& secondPara) {
	paraOne = firstPara;
	paraTwo = secondPara;
	type = ClauseType::CALLS;
	setSynonyms();
}

Result Calls::evaluate(const PKB& pkb) {
	this->pkb = pkb;
	Type paraOneType = paraOne.getType();
	Type paraTwoType = paraTwo.getType();
	std::string paraOneValue = paraOne.getValue();
	std::string paraTwoValue = paraTwo.getValue();

	Result result;

	if (paraOneType == FIXED) {
		if (paraTwoType == PROCEDURE) {
			result = this->evaluateFixedSynonym(paraOneValue, paraTwoValue);
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
		if (paraTwoType == PROCEDURE) {
			result = this->evaluateSynonymSynonym(paraOneValue, paraTwoValue);
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
	else if (paraOneType == UNDERSCORE) {
		if (paraTwoType == PROCEDURE) {
			result = this->evaluateUnderscoreSynonym(paraTwoValue);
		}
		else if (paraTwoType == UNDERSCORE) {
			result = this->evaluateUnderscoreUnderscore();
		}
		else if (paraTwoType == FIXED) {
			result = this->evaluateUnderscoreFixed(paraTwoValue);
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

// case Calls("main", p)
Result Calls::evaluateFixedSynonym(const std::string& callerName, const std::string& receiverSynonym) {
	Result result;
	std::unordered_set<std::string> answer = pkb.getReceiverOf(callerName);
	if (!answer.empty()) {
		result.setPassed(true);
		result.setAnswer(receiverSynonym, answer, pkb.getProcTable());
	}
	else {
		result.setPassed(false);
	}
	return result;
}

// case Calls("main", _)
Result Calls::evaluateFixedUnderscore(const std::string& callerName) {
	Result result;
	result.setPassed(pkb.hasReceiver(callerName));
	return result;
}

// case Calls("main", "first")
Result Calls::evaluateFixedFixed(const std::string& callerName, const std::string& receiverName) {
	Result result;
	result.setPassed(pkb.isCalls(callerName, receiverName));
	return result;
}

// case Calls(p, q)
Result Calls::evaluateSynonymSynonym(const std::string& callerSynonym, const std::string& receiverSynonym) {
	Result result;
	if (callerSynonym == receiverSynonym) { // recursive calls not allowed
		result.setPassed(false);
		return result;
	}
	std::unordered_map<std::string, std::unordered_set<std::string>> answer = pkb.getCallerReceiverPairs();
	if (!answer.empty()) {
		result.setPassed(true);
		result.setAnswer(callerSynonym, receiverSynonym, answer, pkb.getProcTable());
	}
	else {
		result.setPassed(false);
	}
	return result;
}

// case Calls(p, _)
Result Calls::evaluateSynonymUnderscore(const std::string& callerSynonym) {
	Result result;
	std::unordered_set<std::string> answer = pkb.getCallerProcedures();
	if (!answer.empty()) {
		result.setPassed(true);
		result.setAnswer(callerSynonym, answer, pkb.getProcTable());
	}
	else {
		result.setPassed(false);
	}
	return result;
}

// case Calls(p, "first")
Result Calls::evaluateSynonymFixed(const std::string& callerSynonym, const std::string& receiverName) {
	Result result;
	std::unordered_set<std::string> answer = pkb.getCallerOf(receiverName);
	if (!answer.empty()) {
		result.setPassed(true);
		result.setAnswer(callerSynonym, answer, pkb.getProcTable());
	}
	else {
		result.setPassed(false);
	}
	return result;
}

// case Calls(_, q)
Result Calls::evaluateUnderscoreSynonym(const std::string& receiverSynonym) {
	Result result;
	std::unordered_set<std::string> answer = pkb.getReceiverProcedures();
	if (!answer.empty()) {
		result.setPassed(true);
		result.setAnswer(receiverSynonym, answer, pkb.getProcTable());
	}
	else {
		result.setPassed(false);
	}
	return result;
}

// case Calls(_, _)
Result Calls::evaluateUnderscoreUnderscore() {
	Result result;
	std::unordered_set<std::string> callers = pkb.getCallerProcedures();
	result.setPassed(!callers.empty());
	return result;
}

// case Calls(_, "first")
Result Calls::evaluateUnderscoreFixed(const std::string& receiverName) {
	Result result;
	result.setPassed(pkb.hasCaller(receiverName));
	return result;
}
