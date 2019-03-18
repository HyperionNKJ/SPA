#include "Calls.h"

Calls::Calls(const DesignEntity& paraOne, const DesignEntity& paraTwo) : Clause(paraOne, paraTwo, ClauseType::CALLS) {}

Result Calls::evaluate(const PKB& pkb) {
	this->pkb = pkb;
	Type paraOneType = paraOne.getType();
	Type paraTwoType = paraTwo.getType();
	string paraOneValue = paraOne.getValue();
	string paraTwoValue = paraTwo.getValue();

	Result* result;

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
			result = new Result();
			result->setPassed(false);
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
			result = new Result();
			result->setPassed(false);
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

// case Calls("main", p)
Result* Calls::evaluateFixedSynonym(const string& callerName, const string& receiverSynonym) {
	Result* result = new Result();
	unordered_set<string> answer = pkb.getReceiverOf(callerName);
	if (!answer.empty()) {
		result->setPassed(true);
		result->setAnswer(receiverSynonym, answer);
	}
	else {
		result->setPassed(false);
	}
	return result;
}

// case Calls("main", _)
Result* Calls::evaluateFixedUnderscore(const string& callerName) {
	Result* result = new Result();
	result->setPassed(pkb.hasReceiver(callerName));
	return result;
}

// case Calls("main", "first")
Result* Calls::evaluateFixedFixed(const string& callerName, const string& receiverName) {
	Result* result = new Result();
	result->setPassed(pkb.isCalls(callerName, receiverName));
	return result;
}

// case Calls(p, q)
Result* Calls::evaluateSynonymSynonym(const string& callerSynonym, const string& receiverSynonym) {
	Result* result = new Result();
	if (callerSynonym == receiverSynonym) { // recursive calls not allowed
		result->setPassed(false);
		return result;
	}
	unordered_map<string, unordered_set<string>> answer = pkb.getCallerReceiverPairs();
	if (!answer.empty()) {
		result->setPassed(true);
		result->setAnswer(callerSynonym, receiverSynonym, answer);
	}
	else {
		result->setPassed(false);
	}
	return result;
}

// case Calls(p, _)
Result* Calls::evaluateSynonymUnderscore(const string& callerSynonym) {
	Result* result = new Result();
	unordered_set<string> answer = pkb.getCallerProcedures();
	if (!answer.empty()) {
		result->setPassed(true);
		result->setAnswer(callerSynonym, answer);
	}
	else {
		result->setPassed(false);
	}
	return result;
}

// case Calls(p, "first")
Result* Calls::evaluateSynonymFixed(const string& callerSynonym, const string& receiverName) {
	Result* result = new Result();
	unordered_set<string> answer = pkb.getCallerOf(receiverName);
	if (!answer.empty()) {
		result->setPassed(true);
		result->setAnswer(callerSynonym, answer);
	}
	else {
		result->setPassed(false);
	}
	return result;
}

// case Calls(_, q)
Result* Calls::evaluateUnderscoreSynonym(const string& receiverSynonym) {
	Result* result = new Result();
	unordered_set<string> answer = pkb.getReceiverProcedures();
	if (!answer.empty()) {
		result->setPassed(true);
		result->setAnswer(receiverSynonym, answer);
	}
	else {
		result->setPassed(false);
	}
	return result;
}

// case Calls(_, _)
Result* Calls::evaluateUnderscoreUnderscore() {
	Result* result = new Result();
	unordered_set<string> callers = pkb.getCallerProcedures();
	result->setPassed(!callers.empty());
	return result;
}

// case Calls(_, "first")
Result* Calls::evaluateUnderscoreFixed(const string& receiverName) {
	Result* result = new Result();
	result->setPassed(pkb.hasCaller(receiverName));
	return result;
}
