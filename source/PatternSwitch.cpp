#include "PatternSwitch.h"

PatternSwitch::PatternSwitch(const DesignEntity& patternSubject, const DesignEntity& controlVar) {
	paraOne = controlVar;
	paraTwo = DesignEntity("_", Type::UNDERSCORE);
	subject = patternSubject;
	type = ClauseType::PATTERN_SWITCH;
	setSynonyms();
}

Result PatternSwitch::evaluate(const PKB& pkb) {
	this->pkb = pkb;

	Type subjectType = subject.getType(); // should be a switch type
	Type paraOneType = paraOne.getType();
	string subjectValue = subject.getValue();
	string paraOneValue = paraOne.getValue();

	Result result;

	if (subjectType != SWITCH) {
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

// case sw(v, _)
Result PatternSwitch::evaluateVariable(const string& controlVar, const string& switchSynonym) {
	Result result;
	unordered_map<int, string> answer = pkb.getSwitchControlVarPair();

	if (!answer.empty()) {
		result.setPassed(true);
		result.setAnswer(switchSynonym, controlVar, answer, pkb.getVarTable());
	}
	else {
		result.setPassed(false);
	}
	return result;
}

// case sw(_, _)
Result PatternSwitch::evaluateUnderscore(const string& switchSynonym) {
	Result result;
	unordered_set<int> answer = pkb.getSwitchWithControlVar();

	if (!answer.empty()) {
		result.setPassed(true);
		result.setAnswer(switchSynonym, answer);
	}
	else {
		result.setPassed(false);
	}
	return result;
}

// case sw("count", _)
Result PatternSwitch::evaluateFixed(const string& controlVar, const string& switchSynonym) {
	Result result;
	unordered_set<int> answer = pkb.getSwitchWithControlVar(controlVar);

	if (!answer.empty()) {
		result.setPassed(true);
		result.setAnswer(switchSynonym, answer);
	}
	else {
		result.setPassed(false);
	}
	return result;
}