#include "NextT.h"

NextT::NextT(const DesignEntity& paraOne, const DesignEntity& paraTwo) : Clause(paraOne, paraTwo, ClauseType::NEXT_T) {}

Result NextT::evaluate(const PKB& pkb) {
	this->pkb = pkb;
	Type paraOneType = paraOne.getType();
	Type paraTwoType = paraTwo.getType();
	string paraOneValue = paraOne.getValue();
	string paraTwoValue = paraTwo.getValue();

	Result* result;

	if (paraOneType == FIXED) {
		if (paraTwoType == STATEMENT || paraTwoType == READ || paraTwoType == PRINT
			|| paraTwoType == WHILE || paraTwoType == IF || paraTwoType == ASSIGN || paraTwoType == CALL || paraTwoType == PROGLINE) {
			result = this->evaluateFixedSynonym(paraOneValue, paraTwoValue, paraTwoType);
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
	else if (paraOneType == STATEMENT || paraOneType == READ || paraOneType == PRINT
		|| paraOneType == WHILE || paraOneType == IF || paraOneType == ASSIGN || paraOneType == CALL || paraOneType == PROGLINE) {
		if (paraTwoType == STATEMENT || paraTwoType == READ || paraTwoType == PRINT
			|| paraTwoType == WHILE || paraTwoType == IF || paraTwoType == ASSIGN || paraTwoType == CALL || paraTwoType == PROGLINE) {
			result = this->evaluateSynonymSynonym(paraOneValue, paraTwoValue, paraOneType, paraTwoType);
		}
		else if (paraTwoType == UNDERSCORE) {
			result = this->evaluateSynonymUnderscore(paraOneValue, paraOneType);
		}
		else if (paraTwoType == FIXED) {
			result = this->evaluateSynonymFixed(paraOneValue, paraTwoValue, paraOneType);
		}
		else {
			result = new Result();
			result->setPassed(false);
		}
	}
	else if (paraOneType == UNDERSCORE) {
		if (paraTwoType == STATEMENT || paraTwoType == READ || paraTwoType == PRINT
			|| paraTwoType == WHILE || paraTwoType == IF || paraTwoType == ASSIGN || paraTwoType == CALL || paraTwoType == PROGLINE) {
			result = this->evaluateUnderscoreSynonym(paraTwoValue, paraTwoType);
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

// case Next*(12, a)
Result* NextT::evaluateFixedSynonym(const string& previousLineNum, const string& nextSynonym, const Type& nextType) {
	Result* result = new Result();
	unordered_set<int> answer;
	if (reducedDomain.count(nextSynonym)) { // domain is reduced if synonym is present in intermediate table. Can improve performance
		unordered_set<int> possibleValues = reducedDomain.at(nextSynonym);
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
		result->setPassed(true);
		result->setAnswer(nextSynonym, answer);
	}
	else {
		result->setPassed(false);
	}
	return result;
}

// case Next*(12, _)
Result* NextT::evaluateFixedUnderscore(const string& previousLineNum) {
	Result* result = new Result();
	result->setPassed(pkb.hasNext(stoi(previousLineNum)));
	return result;
}

// case Next*(12, 13)
Result* NextT::evaluateFixedFixed(const string& previousLineNum, const string& nextLineNum) {
	Result* result = new Result();
	result->setPassed(pkb.isNextT(stoi(previousLineNum), stoi(nextLineNum)));
	return result;
}

// case Next*(s, s1)
Result* NextT::evaluateSynonymSynonym(const string& previousSynonym, const string& nextSynonym, const Type& previousType, const Type& nextType) {
	Result* result = new Result();
	bool hasReducedDomainForPrevSyn = reducedDomain.count(previousSynonym);
	bool hasReducedDomainForNextSyn = reducedDomain.count(nextSynonym);
	unordered_set<int> prevSynPossibleValues; 
	unordered_set<int> nextSynPossibleValues;

	if (hasReducedDomainForPrevSyn) {
		prevSynPossibleValues = reducedDomain.at(previousSynonym);
	}

	if (hasReducedDomainForNextSyn) {
		nextSynPossibleValues = reducedDomain.at(nextSynonym);
	}

	unordered_map<int, unordered_set<int>> answer;
	bool keyIsPrevSyn; // true if above answer's key = previousSynonym & value = nextSynonym. false otherwise.

	if (hasReducedDomainForPrevSyn && hasReducedDomainForNextSyn) { // if both synonyms exist in intermediate table
		if (prevSynPossibleValues.size < nextSynPossibleValues.size) {
			keyIsPrevSyn = evaluateSynSynFromPrev(prevSynPossibleValues, nextType, pkb, answer);
		}
		else {
			keyIsPrevSyn = evaluateSynSynFromNext(nextSynPossibleValues, previousType, pkb, answer);
		}
	} else if (hasReducedDomainForPrevSyn && !hasReducedDomainForNextSyn) {
		keyIsPrevSyn = evaluateSynSynFromPrev(prevSynPossibleValues, nextType, pkb, answer);
	}
	else if (!hasReducedDomainForPrevSyn && hasReducedDomainForNextSyn) {
		keyIsPrevSyn = evaluateSynSynFromNext(nextSynPossibleValues, previousType, pkb, answer);
	}
	else {
		answer = pkb.getPreviousNextTPairs(previousType, nextType); // no choice but have to compute all exhaustively -> expensive.
		keyIsPrevSyn = true;
	}
	
	if (!answer.empty()) {
		result->setPassed(true);
		if (keyIsPrevSyn) {
			result->setAnswer(previousSynonym, nextSynonym, answer);
		}
		else {
			result->setAnswer(nextSynonym, previousSynonym, answer);
		}
	}
	else {
		result->setPassed(false);
	}
	return result;
}

// Helper method for evaluateSynonymSynonym()
bool NextT::evaluateSynSynFromPrev(const unordered_set<int>& prevSynPossibleValues, const Type& nextType, const PKB& pkb, unordered_map<int, unordered_set<int>>& answer) {
	for (auto prevValue : prevSynPossibleValues) {
		unordered_set<int> nextValues = pkb.getNextTOf(prevValue, nextType);
		if (!nextValues.empty) {
			answer.insert({ prevValue, nextValues }); 
		}
	}
	return true; // true because answer's key is prev
}

// Helper method for evaluateSynonymSynonym()
bool NextT::evaluateSynSynFromNext(const unordered_set<int>& nextSynPossibleValues, const Type& previousType, const PKB& pkb, unordered_map<int, unordered_set<int>>& answer) {
	for (auto nextValue : nextSynPossibleValues) {
		unordered_set<int> prevValues = pkb.getPreviousTOf(nextValue, previousType); 
		if (!prevValues.empty) {
			answer.insert({ nextValue, prevValues });
		}
	}
	return false; // false because answer's key is next
}

// case Next*(w, _)
Result* NextT::evaluateSynonymUnderscore(const string& previousSynonym, const Type& previousType) {
	Result* result = new Result();
	unordered_set<int> answer;
	if (reducedDomain.count(previousSynonym)) { 
		unordered_set<int> possibleValues = reducedDomain.at(previousSynonym);
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
		result->setPassed(true);
		result->setAnswer(previousSynonym, answer);
	}
	else {
		result->setPassed(false);
	}
	return result;
}

// case Next*(s, 14)
Result* NextT::evaluateSynonymFixed(const string& previousSynonym, const string& nextLineNum, const Type& previousType) {
	Result* result = new Result();
	unordered_set<int> answer;
	if (reducedDomain.count(previousSynonym)) {
		unordered_set<int> possibleValues = reducedDomain.at(previousSynonym);
		for (auto value : possibleValues) {
			if (pkb.isNextT(value, nextLineNum)) {
				answer.insert(value);
			}
		}
	}
	else {
		answer = pkb.getPreviousTOf(stoi(nextLineNum), previousType);
	}
	if (!answer.empty()) {
		result->setPassed(true);
		result->setAnswer(previousSynonym, answer);
	}
	else {
		result->setPassed(false);
	}
	return result;
}

// case Next*(_, pl)
Result* NextT::evaluateUnderscoreSynonym(const string& nextSynonym, const Type& nextType) {
	Result* result = new Result();
	unordered_set<int> answer;
	if (reducedDomain.count(nextSynonym)) {
		unordered_set<int> possibleValues = reducedDomain.at(nextSynonym);
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
		result->setPassed(true);
		result->setAnswer(nextSynonym, answer);
	}
	else {
		result->setPassed(false);
	}
	return result;
}

// case Next*(_, _)
Result* NextT::evaluateUnderscoreUnderscore() {
	Result* result = new Result();
	unordered_set<int> previousLines = pkb.getPreviousLines(Type::PROGLINE);
	result->setPassed(!previousLines.empty());
	return result;
}

// case Next*(_, 23)
Result* NextT::evaluateUnderscoreFixed(const string& nextLineNum) {
	Result* result = new Result();
	result->setPassed(pkb.hasPrevious(stoi(nextLineNum)));
	return result;
}
