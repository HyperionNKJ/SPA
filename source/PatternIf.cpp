#include "PatternIf.h"

PatternIf::PatternIf(const DesignEntity& patternSubject, const DesignEntity& controlVar) {
	paraOne = controlVar;
	paraTwo = DesignEntity("_", Type::UNDERSCORE);
	subject = patternSubject;
	type = ClauseType::PATTERN_IF;
	setSynonyms();
}

Result PatternIf::evaluate(const PKB& pkb) {
	this->pkb = pkb;

	Type subjectType = subject.getType(); // should be a if type
	Type paraOneType = paraOne.getType();
	string subjectValue = subject.getValue();
	string paraOneValue = paraOne.getValue();

	Result result;

	if (subjectType != IF) {
		result.setPassed(false);
	}

	if (paraOneType == VARIABLE) {
		result = this->evaluateVariable(paraOneValue, subjectValue);
	}
	else if (paraOneType == UNDERSCORE) {
		result = this->evaluateUnderscore(subjectValue);
	}
	else if (paraOneType == FIXED) {
		result = this->evaluateFixed(paraOneValue, subjectValue);
	}
	else {
		result.setPassed(false);
	}
	return result;
}

// case i(v, _, _)
Result PatternIf::evaluateVariable(const string& controlVar, const string& ifSynonym) {
	Result result;
	unordered_map<int, unordered_set<string>> answer = pkb.getIfControlVarPair();

	if (!answer.empty()) {
		result.setPassed(true);
		result.setAnswer(ifSynonym, controlVar, answer, pkb.getVarTable());
	}
	else {
		result.setPassed(false);
	}
	return result;
}

// i(_, _, _)
Result PatternIf::evaluateUnderscore(const string& ifSynonym) {
	Result result;
	unordered_set<int> answer = pkb.getIfWithControlVar();

	if (!answer.empty()) {
		result.setPassed(true);
		result.setAnswer(ifSynonym, answer);
	}
	else {
		result.setPassed(false);
	}
	return result;
}

// case i("count", _, _)
Result PatternIf::evaluateFixed(const string& controlVar, const string& ifSynonym) {
	Result result;
	unordered_set<int> answer = pkb.getIfWithControlVar(controlVar);

	if (!answer.empty()) {
		result.setPassed(true);
		result.setAnswer(ifSynonym, answer);
	}
	else {
		result.setPassed(false);
	}
	return result;
}