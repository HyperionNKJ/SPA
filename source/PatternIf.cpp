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
	std::string subjectValue = subject.getValue();
	std::string paraOneValue = paraOne.getValue();

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
Result PatternIf::evaluateVariable(const std::string& controlVar, const std::string& ifSynonym) {
	Result result;
	std::unordered_map<int, std::unordered_set<std::string>> answer = pkb.getIfControlVarPair();

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
Result PatternIf::evaluateUnderscore(const std::string& ifSynonym) {
	Result result;
	std::unordered_set<int> answer = pkb.getIfWithControlVar();

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
Result PatternIf::evaluateFixed(const std::string& controlVar, const std::string& ifSynonym) {
	Result result;
	std::unordered_set<int> answer = pkb.getIfWithControlVar(controlVar);

	if (!answer.empty()) {
		result.setPassed(true);
		result.setAnswer(ifSynonym, answer);
	}
	else {
		result.setPassed(false);
	}
	return result;
}