#include "Next.h"

Next::Next(const DesignEntity& firstPara, const DesignEntity& secondPara) {
	paraOne = firstPara;
	paraTwo = secondPara;
	type = ClauseType::NEXT;
	setSynonyms();
}

/**
 * Definitions:
 * 1. Previous is a program line which can be executed before another program line. This another program line is said to be its Next.
 * e.g. Next (1,2) -> 1 is the Previous; 2 is the Next
 * 2. Next is a program line which can be executed after another program line. This another program line is said to be its Previous.
*/
Result Next::evaluate(const PKB& pkb) {
	this->pkb = pkb;
	Type paraOneType = paraOne.getType();
	Type paraTwoType = paraTwo.getType();
	std::string paraOneValue = paraOne.getValue();
	std::string paraTwoValue = paraTwo.getValue();

	Result result;

	if (paraOneType == FIXED) {
		if (paraTwoType == STATEMENT || paraTwoType == READ || paraTwoType == PRINT || paraTwoType == WHILE || paraTwoType == IF 
			|| paraTwoType == ASSIGN || paraTwoType == CALL || paraTwoType == PROGLINE || paraTwoType == SWITCH) {
			result = this->evaluateFixedSynonym(paraOneValue, paraTwoValue, paraTwoType);
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
	else if (paraOneType == STATEMENT || paraOneType == READ || paraOneType == PRINT || paraOneType == WHILE || paraOneType == IF || 
		paraOneType == ASSIGN || paraOneType == CALL || paraOneType == PROGLINE || paraOneType == SWITCH) {
		if (paraTwoType == STATEMENT || paraTwoType == READ || paraTwoType == PRINT || paraTwoType == WHILE || paraTwoType == IF 
			|| paraTwoType == ASSIGN || paraTwoType == CALL || paraTwoType == PROGLINE || paraTwoType == SWITCH) {
			result = this->evaluateSynonymSynonym(paraOneValue, paraTwoValue, paraOneType, paraTwoType);
		}
		else if (paraTwoType == UNDERSCORE) {
			result = this->evaluateSynonymUnderscore(paraOneValue, paraOneType);
		}
		else if (paraTwoType == FIXED) {
			result = this->evaluateSynonymFixed(paraOneValue, paraTwoValue, paraOneType);
		}
		else {
			result.setPassed(false);
		}
	}
	else if (paraOneType == UNDERSCORE) {
		if (paraTwoType == STATEMENT || paraTwoType == READ || paraTwoType == PRINT || paraTwoType == WHILE || paraTwoType == IF 
			|| paraTwoType == ASSIGN || paraTwoType == CALL || paraTwoType == PROGLINE || paraTwoType == SWITCH) {
			result = this->evaluateUnderscoreSynonym(paraTwoValue, paraTwoType);
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

// case Next(12, a)
Result Next::evaluateFixedSynonym(const std::string& previousLineNum, const std::string& nextSynonym, const Type& nextType) {
	Result result;
	std::unordered_set<int> answer = pkb.getNextOf(stoi(previousLineNum), nextType);
	if (!answer.empty()) {
		result.setPassed(true);
		result.setAnswer(nextSynonym, answer);
	}
	else {
		result.setPassed(false);
	}
	return result;
}

// case Next(12, _)
Result Next::evaluateFixedUnderscore(const std::string& previousLineNum) {
	Result result;
	result.setPassed(pkb.hasNext(stoi(previousLineNum)));
	return result;
}

// case Next(12, 13)
Result Next::evaluateFixedFixed(const std::string& previousLineNum, const std::string& nextLineNum) {
	Result result;
	result.setPassed(pkb.isNext(stoi(previousLineNum), stoi(nextLineNum)));
	return result;
}

// case Next(s, s1)
Result Next::evaluateSynonymSynonym(const std::string& previousSynonym, const std::string& nextSynonym, const Type& previousType, const Type& nextType) {
	Result result;
	if (previousSynonym == nextSynonym) {
		result.setPassed(false);
		return result;
	}
	std::unordered_map<int, std::unordered_set<int>> answer = pkb.getPreviousNextPairs(previousType, nextType);
	if (!answer.empty()) {
		result.setPassed(true);
		result.setAnswer(previousSynonym, nextSynonym, answer);
	}
	else {
		result.setPassed(false);
	}
	return result;
}

// case Next(w, _)
Result Next::evaluateSynonymUnderscore(const std::string& previousSynonym, const Type& previousType) {
	Result result;
	std::unordered_set<int> answer = pkb.getPreviousLines(previousType);
	if (!answer.empty()) {
		result.setPassed(true);
		result.setAnswer(previousSynonym, answer);
	}
	else {
		result.setPassed(false);
	}
	return result;
}

// case Next(s, 14)
Result Next::evaluateSynonymFixed(const std::string& previousSynonym, const std::string& nextLineNum, const Type& previousType) {
	Result result;
	std::unordered_set<int> answer = pkb.getPreviousOf(stoi(nextLineNum), previousType);
	if (!answer.empty()) {
		result.setPassed(true);
		result.setAnswer(previousSynonym, answer);
	}
	else {
		result.setPassed(false);
	}
	return result;
}

// case Next(_, pl)
Result Next::evaluateUnderscoreSynonym(const std::string& nextSynonym, const Type& nextType) {
	Result result;
	std::unordered_set<int> answer = pkb.getNextLines(nextType);
	if (!answer.empty()) {
		result.setPassed(true);
		result.setAnswer(nextSynonym, answer);
	}
	else {
		result.setPassed(false);
	}
	return result;
}

// case Next(_, _)
Result Next::evaluateUnderscoreUnderscore() {
	Result result;
	std::unordered_set<int> previousLines = pkb.getPreviousLines(Type::PROGLINE);
	result.setPassed(!previousLines.empty());
	return result;
}

// case Next(_, 23)
Result Next::evaluateUnderscoreFixed(const std::string& nextLineNum) {
	Result result;
	result.setPassed(pkb.hasPrevious(stoi(nextLineNum)));
	return result;
}
