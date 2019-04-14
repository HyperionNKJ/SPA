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
	std::string paraOneValue = paraOne.getValue();
	std::string paraTwoValue = paraTwo.getValue();

	Result result;

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
			result.setPassed(false);
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
			result.setPassed(false);
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
			result.setPassed(false);
		}
	}
	else {
		result.setPassed(false);
	}
	return result;
}

// case Affects(5, a)
Result Affects::evaluateFixedSynonym(const std::string& modifierStmtNum, const std::string& userSynonym) {
	Result result;
	std::unordered_set<int> answer;
	if (reducedDomain.count(userSynonym)) { 
		std::unordered_set<int> possibleValues = reducedDomain.at(userSynonym);
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
		result.setPassed(true);
		result.setAnswer(userSynonym, answer);
	}
	else {
		result.setPassed(false);
	}
	return result;
}

// Affects(6, _)
Result Affects::evaluateFixedUnderscore(const std::string& modifierStmtNum) {
	Result result;
	result.setPassed(pkb.hasUser(stoi(modifierStmtNum)));
	return result;
}

// case Affects(11, 15)
Result Affects::evaluateFixedFixed(const std::string& modifierStmtNum, const std::string& userStmtNum) {
	Result result;
	result.setPassed(pkb.isAffects(stoi(modifierStmtNum), stoi(userStmtNum)));
	return result;
}

// case Affects(s, pl)
Result Affects::evaluateSynonymSynonym(const std::string& modifierSynonym, const std::string& userSynonym) {
	Result result;
	bool isSameSynonym = modifierSynonym == userSynonym;
	bool hasDomainForModifierSyn = reducedDomain.count(modifierSynonym);
	bool hasDomainForUserSyn = reducedDomain.count(userSynonym);
	std::unordered_set<int> modifierSynPossibleValues;
	std::unordered_set<int> userSynPossibleValues;

	if (hasDomainForModifierSyn) {
		modifierSynPossibleValues = reducedDomain.at(modifierSynonym);
	}

	if (hasDomainForUserSyn) {
		userSynPossibleValues = reducedDomain.at(userSynonym);
	}

	std::unordered_map<int, std::unordered_set<int>> answer;
	bool keyIsModifierSyn = false; // true if above answer's key represents the modifier while value represents all its users. false otherwise.

	if (hasDomainForModifierSyn && hasDomainForUserSyn) { // if both synonyms exist in intermediate table
		if (modifierSynPossibleValues.size() < userSynPossibleValues.size()) {
			keyIsModifierSyn = evaluateSynSynFromModifier(modifierSynPossibleValues, pkb, answer, isSameSynonym);
		}
		else {
			keyIsModifierSyn = evaluateSynSynFromUser(userSynPossibleValues, pkb, answer, isSameSynonym);
		}
	}
	else if (hasDomainForModifierSyn && !hasDomainForUserSyn) {
		keyIsModifierSyn = evaluateSynSynFromModifier(modifierSynPossibleValues, pkb, answer, isSameSynonym);
	}
	else if (!hasDomainForModifierSyn && hasDomainForUserSyn) {
		keyIsModifierSyn = evaluateSynSynFromUser(userSynPossibleValues, pkb, answer, isSameSynonym);
	}
	else {
		if (isSameSynonym) {
			std::unordered_set<int> modifiers = pkb.getModifierStmts();
			evaluateSameSynonym(modifiers, pkb, answer);
		}
		else {
			answer = pkb.getModifierUserPairs();
			keyIsModifierSyn = true;
		}
	}

	if (!answer.empty()) {
		result.setPassed(true);
		if (keyIsModifierSyn) {
			result.setAnswer(modifierSynonym, userSynonym, answer);
		}
		else {
			result.setAnswer(userSynonym, modifierSynonym, answer);
		}
	}
	else {
		result.setPassed(false);
	}
	return result;
}

// Helper method for evaluateSynonymSynonym()
bool Affects::evaluateSynSynFromModifier(const std::unordered_set<int>& modifierSynPossibleValues, PKB& pkb, std::unordered_map<int, std::unordered_set<int>>& answer, const bool& isSameSynonym) {
	if (isSameSynonym) {
		evaluateSameSynonym(modifierSynPossibleValues, pkb, answer);
		return true; // does not matter since same synonym
	}

	for (const auto& modifier : modifierSynPossibleValues) {
		std::unordered_set<int> users = pkb.getUserOf(modifier);
		if (!users.empty()) {
			answer.insert({ modifier, users });
		}
	}
	return true; // true because answer's key is modifier (as required by caller function)
}

// Helper method for evaluateSynonymSynonym()
bool Affects::evaluateSynSynFromUser(const std::unordered_set<int>& userSynPossibleValues, PKB& pkb, std::unordered_map<int, std::unordered_set<int>>& answer, const bool& isSameSynonym) {
	if (isSameSynonym) {
		evaluateSameSynonym(userSynPossibleValues, pkb, answer);
		return false; // does not matter since same synonym
	}

	for (auto user : userSynPossibleValues) {
		std::unordered_set<int> modifiers = pkb.getModifierOf(user);
		if (!modifiers.empty()) {
			answer.insert({ user, modifiers });
		}
	}
	return false; // false because answer's key is not modifier
}

void Affects::evaluateSameSynonym(const std::unordered_set<int>& possibleValues, PKB& pkb, std::unordered_map<int, std::unordered_set<int>>& answer) {
	for (auto value : possibleValues) {
		if (pkb.isAffects(value, value)) {
			answer.insert({ value, {value} });
		}
	}
}

// case Affects(a, _)
Result Affects::evaluateSynonymUnderscore(const std::string& modifierSyn) {
	Result result;
	std::unordered_set<int> answer;
	if (reducedDomain.count(modifierSyn)) {
		std::unordered_set<int> possibleValues = reducedDomain.at(modifierSyn);
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
		result.setPassed(true);
		result.setAnswer(modifierSyn, answer);
	}
	else {
		result.setPassed(false);
	}
	return result;
}

// case Affects(pl, 14)
Result Affects::evaluateSynonymFixed(const std::string& modifierSyn, const std::string& userStmtNum) {
	Result result;
	std::unordered_set<int> answer;
	if (reducedDomain.count(modifierSyn)) {
		std::unordered_set<int> possibleValues = reducedDomain.at(modifierSyn);
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
		result.setPassed(true);
		result.setAnswer(modifierSyn, answer);
	}
	else {
		result.setPassed(false);
	}
	return result;
}

// case Affects(_, s)
Result Affects::evaluateUnderscoreSynonym(const std::string& userSynonym) {
	Result result;
	std::unordered_set<int> answer;
	if (reducedDomain.count(userSynonym)) {
		std::unordered_set<int> possibleValues = reducedDomain.at(userSynonym);
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
		result.setPassed(true);
		result.setAnswer(userSynonym, answer);
	}
	else {
		result.setPassed(false);
	}
	return result;
}

// case Affects(_, _)
Result Affects::evaluateUnderscoreUnderscore() {
	Result result;
	result.setPassed(pkb.hasAffects());
	return result;
}

// case Affects(_, 8)
Result Affects::evaluateUnderscoreFixed(const std::string& userStmtNum) {
	Result result;
	result.setPassed(pkb.hasModifier(stoi(userStmtNum)));
	return result;
}
