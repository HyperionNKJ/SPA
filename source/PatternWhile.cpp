#include "PatternWhile.h"

PatternWhile::PatternWhile(const DesignEntity& patternSubject, const DesignEntity& controlVar) {
	paraOne = controlVar;
	paraTwo = DesignEntity("_", Type::UNDERSCORE);
	subject = patternSubject;
	type = ClauseType::PATTERN_WHILE;
	setSynonyms();
}

Result PatternWhile::evaluate(const PKB& pkb) {
	this->pkb = pkb;

	Type subjectType = subject.getType(); // should be a while type
	Type paraOneType = paraOne.getType();
	string subjectValue = subject.getValue();
	string paraOneValue = paraOne.getValue();

	Result result;

	if (subjectType != WHILE) {
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

// case w(v, _)
Result PatternWhile::evaluateVariable(const string& controlVar, const string& whileSynonym) {
	Result result;
	unordered_map<int, unordered_set<string>> answer = pkb.getWhileControlVarPair();

	if (!answer.empty()) {
		result.setPassed(true);
		result.setAnswer(whileSynonym, controlVar, answer, pkb.getVarTable());
	}
	else {
		result.setPassed(false);
	}
	return result;
}

// case w(_, _)
Result PatternWhile::evaluateUnderscore(const string& whileSynonym) {
	Result result;
	unordered_set<int> answer = pkb.getWhileWithControlVar();

	if (!answer.empty()) {
		result.setPassed(true);
		result.setAnswer(whileSynonym, answer);
	}
	else {
		result.setPassed(false);
	}
	return result;
}

// case w("count", _)
Result PatternWhile::evaluateFixed(const string& controlVar, const string& whileSynonym) {
	Result result;
	unordered_set<int> answer = pkb.getWhileWithControlVar(controlVar);

	if (!answer.empty()) {
		result.setPassed(true);
		result.setAnswer(whileSynonym, answer);
	}
	else {
		result.setPassed(false);
	}
	return result;
}