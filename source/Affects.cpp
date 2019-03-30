#include "Affects.h"

Affects::Affects(const DesignEntity& firstPara, const DesignEntity& secondPara) {
	paraOne = firstPara;
	paraTwo = secondPara;
	type = ClauseType::AFFECTS;
	setSynonyms();
}

Result Affects::evaluate(const PKB& pkb) {
	this->pkb = pkb;
	Type paraOneType = paraOne.getType();
	Type paraTwoType = paraTwo.getType();
	string paraOneValue = paraOne.getValue();
	string paraTwoValue = paraTwo.getValue();

	Result* result;

	if (paraOneType == FIXED) {
		if (paraTwoType == STATEMENT || paraTwoType == ASSIGN || paraTwoType == PROGLINE) {
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
	else if (paraOneType == STATEMENT || paraOneType == ASSIGN || paraOneType == PROGLINE) {
		if (paraTwoType == STATEMENT || paraTwoType == ASSIGN || paraTwoType == PROGLINE) {
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
		if (paraTwoType == STATEMENT || paraTwoType == ASSIGN || paraTwoType == PROGLINE) {
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

// case Affects(5, a)
Result* Affects::evaluateFixedSynonym(const string& modifierStmtNum, const string& userSynonym) {
	Result* result = new Result();
	unordered_set<int> answer;
	if (reducedDomain.count(userSynonym)) { 
		unordered_set<int> possibleValues = reducedDomain.at(userSynonym);
		for (auto value : possibleValues) {
			if (pkb.isAffects(stoi(modifierStmtNum), value)) {
				answer.insert(value);
			}
		}
	}
	else {
		answer = pkb.getUserOf(stoi(modifierStmtNum));
	}

	if (!answer.empty()) {
		result->setPassed(true);
		result->setAnswer(userSynonym, answer);
	}
	else {
		result->setPassed(false);
	}
	return result;
}

// Affects(6, _)
Result* Affects::evaluateFixedUnderscore(const string& modifierStmtNum) {
	Result* result = new Result();
	result->setPassed(pkb.hasUser(stoi(modifierStmtNum)));
	return result;
}

// case Affects(11, 15)
Result* Affects::evaluateFixedFixed(const string& modifierStmtNum, const string& userStmtNum) {
	Result* result = new Result();
	result->setPassed(pkb.isAffects(stoi(modifierStmtNum), stoi(userStmtNum)));
	return result;
}

// case Affects(s, pl)
Result* Affects::evaluateSynonymSynonym(const string& modifierSynonym, const string& userSynonym) {
	Result* result = new Result();
	bool hasDomainForModifierSyn = reducedDomain.count(modifierSynonym);
	bool hasDomainForUserSyn = reducedDomain.count(userSynonym);
	unordered_set<int> modifierSynPossibleValues;
	unordered_set<int> userSynPossibleValues;

	if (hasDomainForModifierSyn) {
		modifierSynPossibleValues = reducedDomain.at(modifierSynonym);
	}

	if (hasDomainForUserSyn) {
		userSynPossibleValues = reducedDomain.at(userSynonym);
	}

	unordered_map<int, unordered_set<int>> answer;
	bool keyIsModifierSyn; // true if above answer's key represents the modifier while value represents all its users. false otherwise.

	if (hasDomainForModifierSyn && hasDomainForUserSyn) { // if both synonyms exist in intermediate table
		if (modifierSynPossibleValues.size() < userSynPossibleValues.size()) {
			keyIsModifierSyn = evaluateSynSynFromModifier(modifierSynPossibleValues, pkb, answer);
		}
		else {
			keyIsModifierSyn = evaluateSynSynFromUser(userSynPossibleValues, pkb, answer);
		}
	}
	else if (hasDomainForModifierSyn && !hasDomainForUserSyn) {
		keyIsModifierSyn = evaluateSynSynFromModifier(modifierSynPossibleValues, pkb, answer);
	}
	else if (!hasDomainForModifierSyn && hasDomainForUserSyn) {
		keyIsModifierSyn = evaluateSynSynFromUser(userSynPossibleValues, pkb, answer);
	}
	else {
		answer = pkb.getModifierUserPairs();
		keyIsModifierSyn = true;
	}

	if (!answer.empty()) {
		result->setPassed(true);
		if (keyIsModifierSyn) {
			result->setAnswer(modifierSynonym, userSynonym, answer);
		}
		else {
			result->setAnswer(userSynonym, modifierSynonym, answer);
		}
	}
	else {
		result->setPassed(false);
	}
	return result;
}

// Helper method for evaluateSynonymSynonym()
bool Affects::evaluateSynSynFromModifier(const unordered_set<int>& modifierSynPossibleValues, PKB& pkb, unordered_map<int, unordered_set<int>>& answer) {
	for (const auto& modifier : modifierSynPossibleValues) {
		unordered_set<int> users = pkb.getUserOf(modifier);
		if (!users.empty()) {
			answer.insert({ modifier, users });
		}
	}
	return true; // true because answer's key is modifier (as required by caller function)
}

// Helper method for evaluateSynonymSynonym()
bool Affects::evaluateSynSynFromUser(const unordered_set<int>& userSynPossibleValues, PKB& pkb, unordered_map<int, unordered_set<int>>& answer) {
	for (auto user : userSynPossibleValues) {
		unordered_set<int> modifiers = pkb.getModifierOf(user);
		if (!modifiers.empty()) {
			answer.insert({ user, modifiers });
		}
	}
	return false; // false because answer's key is not modifier
}

// case Affects(a, _)
Result* Affects::evaluateSynonymUnderscore(const string& modifierSyn) {
	Result* result = new Result();
	unordered_set<int> answer;
	if (reducedDomain.count(modifierSyn)) {
		unordered_set<int> possibleValues = reducedDomain.at(modifierSyn);
		for (auto value : possibleValues) {
			if (pkb.hasUser(value)) {
				answer.insert(value);
			}
		}
	}
	else {
		answer = pkb.getModifierStmts();
	}

	if (!answer.empty()) {
		result->setPassed(true);
		result->setAnswer(modifierSyn, answer);
	}
	else {
		result->setPassed(false);
	}
	return result;
}

// case Affects(pl, 14)
Result* Affects::evaluateSynonymFixed(const string& modifierSyn, const string& userStmtNum) {
	Result* result = new Result();
	unordered_set<int> answer;
	if (reducedDomain.count(modifierSyn)) {
		unordered_set<int> possibleValues = reducedDomain.at(modifierSyn);
		for (auto value : possibleValues) {
			if (pkb.isAffects(value, stoi(userStmtNum))) {
				answer.insert(value);
			}
		}
	}
	else {
		answer = pkb.getModifierOf(stoi(userStmtNum));
	}
	if (!answer.empty()) {
		result->setPassed(true);
		result->setAnswer(modifierSyn, answer);
	}
	else {
		result->setPassed(false);
	}
	return result;
}

// case Affects(_, s)
Result* Affects::evaluateUnderscoreSynonym(const string& userSynonym) {
	Result* result = new Result();
	unordered_set<int> answer;
	if (reducedDomain.count(userSynonym)) {
		unordered_set<int> possibleValues = reducedDomain.at(userSynonym);
		for (auto value : possibleValues) {
			if (pkb.hasModifier(value)) {
				answer.insert(value);
			}
		}
	}
	else {
		answer = pkb.getUserStmts();
	}

	if (!answer.empty()) {
		result->setPassed(true);
		result->setAnswer(userSynonym, answer);
	}
	else {
		result->setPassed(false);
	}
	return result;
}

// case Affects(_, _)
Result* Affects::evaluateUnderscoreUnderscore() {
	Result* result = new Result();
	result->setPassed(pkb.hasAffects());
	return result;
}

// case Affects(_, 8)
Result* Affects::evaluateUnderscoreFixed(const string& userStmtNum) {
	Result* result = new Result();
	result->setPassed(pkb.hasModifier(stoi(userStmtNum)));
	return result;
}
