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
	std::string subjectValue = subject.getValue();
	std::string paraOneValue = paraOne.getValue();

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
Result PatternSwitch::evaluateVariable(const std::string& controlVar, const std::string& switchSynonym) {
	Result result;
	std::unordered_map<int, std::unordered_set<std::string>> answer = pkb.getSwitchControlVarPair();

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
Result PatternSwitch::evaluateUnderscore(const std::string& switchSynonym) {
	Result result;
	std::unordered_set<int> answer = pkb.getSwitchStmts(); // all switch statement must have control variable

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
Result PatternSwitch::evaluateFixed(const std::string& controlVar, const std::string& switchSynonym) {
	Result result;
	std::unordered_set<int> answer = pkb.getSwitchWithControlVar(controlVar);

	if (!answer.empty()) {
		result.setPassed(true);
		result.setAnswer(switchSynonym, answer);
	}
	else {
		result.setPassed(false);
	}
	return result;
}
