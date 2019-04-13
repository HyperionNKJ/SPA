#include "NextT.h"

NextT::NextT(const DesignEntity& firstPara, const DesignEntity& secondPara) {
	paraOne = firstPara;
	paraTwo = secondPara;
	type = ClauseType::NEXT_T;
	setSynonyms();
}

Result NextT::evaluate(const PKB& pkb) {
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
	else if (paraOneType == STATEMENT || paraOneType == READ || paraOneType == PRINT || paraOneType == WHILE || paraOneType == IF 
		|| paraOneType == ASSIGN || paraOneType == CALL || paraOneType == PROGLINE || paraOneType == SWITCH) {
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

// case Next*(12, a)
Result NextT::evaluateFixedSynonym(const std::string& previousLineNum, const std::string& nextSynonym, const Type& nextType) {
	Result result;
	std::unordered_set<int> answer;
	if (reducedDomain.count(nextSynonym)) { // domain is reduced if synonym is present in intermediate table. Can improve performance
		std::unordered_set<int> possibleValues = reducedDomain.at(nextSynonym);
		for (auto value : possibleValues) {
			if (pkb.isNextT(stoi(previousLineNum), value)) {
				answer.insert(value);
			}
		}
	}
	else { // otherwise, pkb will have to evaluate every possible value of "nextSynonym" -> slower
		answer = pkb.getNextTOf(stoi(previousLineNum), nextType); 
	}

	if (!answer.empty()) {
		result.setPassed(true);
		result.setAnswer(nextSynonym, answer);
	}
	else {
		result.setPassed(false);
	}
	return result;
}

// case Next*(12, _)
Result NextT::evaluateFixedUnderscore(const std::string& previousLineNum) {
	Result result;
	result.setPassed(pkb.hasNext(stoi(previousLineNum)));
	return result;
}

// case Next*(12, 13)
Result NextT::evaluateFixedFixed(const std::string& previousLineNum, const std::string& nextLineNum) {
	Result result;
	result.setPassed(pkb.isNextT(stoi(previousLineNum), stoi(nextLineNum)));
	return result;
}

// case Next*(s, s1)
Result NextT::evaluateSynonymSynonym(const std::string& previousSynonym, const std::string& nextSynonym, const Type& previousType, const Type& nextType) {
	Result result;
	bool isSameSynonym = nextSynonym == previousSynonym;
	bool hasReducedDomainForPrevSyn = reducedDomain.count(previousSynonym);
	bool hasReducedDomainForNextSyn = reducedDomain.count(nextSynonym);
	std::unordered_set<int> prevSynPossibleValues; 
	std::unordered_set<int> nextSynPossibleValues;

	if (hasReducedDomainForPrevSyn) {
		prevSynPossibleValues = reducedDomain.at(previousSynonym);
	}

	if (hasReducedDomainForNextSyn) {
		nextSynPossibleValues = reducedDomain.at(nextSynonym);
	}

	std::unordered_map<int, std::unordered_set<int>> answer;
	bool keyIsPrevSyn = false; // true if above answer's key = previousSynonym & value = nextSynonym. false otherwise.

	if (hasReducedDomainForPrevSyn && hasReducedDomainForNextSyn) { // if both synonyms exist in intermediate table
		if (prevSynPossibleValues.size() < nextSynPossibleValues.size()) {
			keyIsPrevSyn = evaluateSynSynFromPrev(prevSynPossibleValues, nextType, pkb, answer, isSameSynonym); // work with the smaller domain
		}
		else {
			keyIsPrevSyn = evaluateSynSynFromNext(nextSynPossibleValues, previousType, pkb, answer, isSameSynonym);
		}
	} else if (hasReducedDomainForPrevSyn && !hasReducedDomainForNextSyn) {
		keyIsPrevSyn = evaluateSynSynFromPrev(prevSynPossibleValues, nextType, pkb, answer, isSameSynonym);
	}
	else if (!hasReducedDomainForPrevSyn && hasReducedDomainForNextSyn) {
		keyIsPrevSyn = evaluateSynSynFromNext(nextSynPossibleValues, previousType, pkb, answer, isSameSynonym);
	}
	else {
		if (isSameSynonym) {
			std::unordered_set<int> prevLines = pkb.getPreviousLines(previousType);
			evaluateSameSynonym(prevLines, pkb, answer);
		}
		else {
			answer = pkb.getPreviousNextTPairs(previousType, nextType); // no choice but have to compute all exhaustively -> expensive.
			keyIsPrevSyn = true;
		}
	}
	
	if (!answer.empty()) {
		result.setPassed(true);
		if (keyIsPrevSyn) {
			result.setAnswer(previousSynonym, nextSynonym, answer);
		}
		else {
			result.setAnswer(nextSynonym, previousSynonym, answer);
		}
	}
	else {
		result.setPassed(false);
	}
	return result;
}

// Helper method for evaluateSynonymSynonym()
bool NextT::evaluateSynSynFromPrev(const std::unordered_set<int>& prevSynPossibleValues, const Type& nextType, PKB& pkb, std::unordered_map<int, std::unordered_set<int>>& answer, const bool& isSameSynonym) {
	if (isSameSynonym) {
		evaluateSameSynonym(prevSynPossibleValues, pkb, answer);
		return true; // does not matter since same synonym
	}
	
	for (auto prevValue : prevSynPossibleValues) {
		std::unordered_set<int> nextValues = pkb.getNextTOf(prevValue, nextType);
		if (!nextValues.empty()) {
			answer.insert({ prevValue, nextValues }); 
		}
	}
	return true; // true because answer's key is prev
}

// Helper method for evaluateSynonymSynonym()
bool NextT::evaluateSynSynFromNext(const std::unordered_set<int>& nextSynPossibleValues, const Type& previousType, PKB& pkb, std::unordered_map<int, std::unordered_set<int>>& answer, const bool& isSameSynonym) {
	if (isSameSynonym) {
		evaluateSameSynonym(nextSynPossibleValues, pkb, answer);
		return false; // does not matter since same synonym
	}
	
	for (auto nextValue : nextSynPossibleValues) {
		std::unordered_set<int> prevValues = pkb.getPreviousTOf(nextValue, previousType); 
		if (!prevValues.empty()) {
			answer.insert({ nextValue, prevValues });
		}
	}
	return false; // false because answer's key is next
}

void NextT::evaluateSameSynonym(const std::unordered_set<int>& possibleValues, PKB& pkb, std::unordered_map<int, std::unordered_set<int>>& answer) {
	for (auto value : possibleValues) {
		if (pkb.isNextT(value, value)) {
			answer.insert({ value, {value} });
		}
	}
}

// case Next*(w, _)
Result NextT::evaluateSynonymUnderscore(const std::string& previousSynonym, const Type& previousType) {
	Result result;
	std::unordered_set<int> answer;
	if (reducedDomain.count(previousSynonym)) { 
		std::unordered_set<int> possibleValues = reducedDomain.at(previousSynonym);
		for (auto value : possibleValues) {
			if (pkb.hasNext(value)) { // evaluation of Next*(2,_) = Next(2,_)
				answer.insert(value);
			}
		}
	}
	else { 
		answer = pkb.getPreviousLines(previousType);
	}

	if (!answer.empty()) {
		result.setPassed(true);
		result.setAnswer(previousSynonym, answer);
	}
	else {
		result.setPassed(false);
	}
	return result;
}

// case Next*(s, 14)
Result NextT::evaluateSynonymFixed(const std::string& previousSynonym, const std::string& nextLineNum, const Type& previousType) {
	Result result;
	std::unordered_set<int> answer;
	if (reducedDomain.count(previousSynonym)) {
		std::unordered_set<int> possibleValues = reducedDomain.at(previousSynonym);
		for (auto value : possibleValues) {
			if (pkb.isNextT(value, stoi(nextLineNum))) {
				answer.insert(value);
			}
		}
	}
	else {
		answer = pkb.getPreviousTOf(stoi(nextLineNum), previousType);
	}
	if (!answer.empty()) {
		result.setPassed(true);
		result.setAnswer(previousSynonym, answer);
	}
	else {
		result.setPassed(false);
	}
	return result;
}

// case Next*(_, pl)
Result NextT::evaluateUnderscoreSynonym(const std::string& nextSynonym, const Type& nextType) {
	Result result;
	std::unordered_set<int> answer;
	if (reducedDomain.count(nextSynonym)) {
		std::unordered_set<int> possibleValues = reducedDomain.at(nextSynonym);
		for (auto value : possibleValues) {
			if (pkb.hasPrevious(value)) {
				answer.insert(value);
			}
		}
	}
	else {
		answer = pkb.getNextLines(nextType);
	}

	if (!answer.empty()) {
		result.setPassed(true);
		result.setAnswer(nextSynonym, answer);
	}
	else {
		result.setPassed(false);
	}
	return result;
}

// case Next*(_, _)
Result NextT::evaluateUnderscoreUnderscore() {
	Result result;
	result.setPassed(pkb.hasNextT());
	return result;
}

// case Next*(_, 23)
Result NextT::evaluateUnderscoreFixed(const std::string& nextLineNum) {
	Result result;
	result.setPassed(pkb.hasPrevious(stoi(nextLineNum)));
	return result;
}
