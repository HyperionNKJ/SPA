#include "CallsT.h"

CallsT::CallsT(const DesignEntity& paraOne, const DesignEntity& paraTwo) : Clause(paraOne, paraTwo, ClauseType::CALLS_T) {}

Result CallsT::evaluate(const PKB& pkb) {
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

// case Calls*("main", p)
Result* CallsT::evaluateFixedSynonym(const string& callerName, const string& receiverSynonym) {
	Result* result = new Result();
	unordered_set<string> answer = pkb.getReceiverTOf(callerName);
	if (!answer.empty()) {
		result->setPassed(true);
		result->setAnswer(receiverSynonym, answer, pkb.getProcTable());
	}
	else {
		result->setPassed(false);
	}
	return result;
}

// case Calls*("main", _)
Result* CallsT::evaluateFixedUnderscore(const string& callerName) {
	Result* result = new Result();
	result->setPassed(pkb.hasReceiver(callerName));
	return result;
}

// case Calls*("main", "first")
Result* CallsT::evaluateFixedFixed(const string& callerName, const string& receiverName) {
	Result* result = new Result();
	result->setPassed(pkb.isCallsT(callerName, receiverName));
	return result;
}

// case Calls*(p, q)
Result* CallsT::evaluateSynonymSynonym(const string& callerSynonym, const string& receiverSynonym) {
	Result* result = new Result();
	if (callerSynonym == receiverSynonym) { // cyclic calls not allowed
		result->setPassed(false);
		return result;
	}
	unordered_map<string, unordered_set<string>> answer = pkb.getCallerReceiverTPairs();
	if (!answer.empty()) {
		result->setPassed(true);
		result->setAnswer(callerSynonym, receiverSynonym, answer, pkb.getProcTable());
	}
	else {
		result->setPassed(false);
	}
	return result;
}

// case Calls*(p, _)
Result* CallsT::evaluateSynonymUnderscore(const string& callerSynonym) {
	Result* result = new Result();
	unordered_set<string> answer = pkb.getCallerProcedures();
	if (!answer.empty()) {
		result->setPassed(true);
		result->setAnswer(callerSynonym, answer, pkb.getProcTable());
	}
	else {
		result->setPassed(false);
	}
	return result;
}

// case Calls*(p, "first")
Result* CallsT::evaluateSynonymFixed(const string& callerSynonym, const string& receiverName) {
	Result* result = new Result();
	unordered_set<string> answer = pkb.getCallerTOf(receiverName);
	if (!answer.empty()) {
		result->setPassed(true);
		result->setAnswer(callerSynonym, answer, pkb.getProcTable());
	}
	else {
		result->setPassed(false);
	}
	return result;
}

// case Calls*(_, q)
Result* CallsT::evaluateUnderscoreSynonym(const string& receiverSynonym) {
	Result* result = new Result();
	unordered_set<string> answer = pkb.getReceiverProcedures();
	if (!answer.empty()) {
		result->setPassed(true);
		result->setAnswer(receiverSynonym, answer, pkb.getProcTable());
	}
	else {
		result->setPassed(false);
	}
	return result;
}

// case Calls*(_, _)
Result* CallsT::evaluateUnderscoreUnderscore() {
	Result* result = new Result();
	unordered_set<string> callers = pkb.getCallerProcedures();
	result->setPassed(!callers.empty());
	return result;
}

// case Calls*(_, "first")
Result* CallsT::evaluateUnderscoreFixed(const string& receiverName) {
	Result* result = new Result();
	result->setPassed(pkb.hasCaller(receiverName));
	return result;
}
