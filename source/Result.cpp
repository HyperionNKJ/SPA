#include "Result.h"

bool Result::hasPassed() {
	return this->isPassed;
}

void Result::setPassed(bool isPassed) {
	this->isPassed = isPassed;
}

std::vector<std::string> Result::getSynonyms() {
	return this->synonyms;
}

int Result::getNumOfSyn() {
	return this->synonyms.size();
}

std::unordered_set<int> Result::getOneSynAnswer() {
	return this->oneSynonymAnswer;
}

std::unordered_map<int, std::unordered_set<int>> Result::getTwoSynAnswer() {
	return this->twoSynonymAnswer;
}

void Result::setAnswer(const std::string& synonym, const int& answer) {
	this->synonyms.push_back(synonym);
	this->oneSynonymAnswer.insert(answer);
}

void Result::setAnswer(const std::string& synonym, const std::unordered_set<int>& answer) {
	this->synonyms.push_back(synonym);
	this->oneSynonymAnswer = answer;
}

void Result::setAnswer(const std::string& synonym, const std::unordered_set<std::string>& answer, const std::unordered_map<std::string, int>& indexTable) {
	this->synonyms.push_back(synonym);
	for (auto stringEntity : answer) { // convert all strings to their corresponding index as per the given variable/procedure table
		oneSynonymAnswer.insert(indexTable.at(stringEntity));
	}
}

void Result::setAnswer(const std::string& keySynonym, const std::string& valueSynonym, const std::unordered_map<int, int>& answer) {
	this->synonyms.push_back(keySynonym);
	this->synonyms.push_back(valueSynonym);
	for (auto pair : answer) {
		this->twoSynonymAnswer.insert({ pair.first, {pair.second} });
	}
}

void Result::setAnswer(const std::string& keySynonym, const std::string& valueSynonym, const std::unordered_map<int, std::string>& answer, const std::unordered_map<std::string, int>& indexTable) {
	this->synonyms.push_back(keySynonym);
	this->synonyms.push_back(valueSynonym);
	for (auto pair : answer) {
		this->twoSynonymAnswer.insert({ pair.first, {indexTable.at(pair.second)} });
	}
}

void Result::setAnswer(const std::string& keySynonym, const std::string& valueSynonym, const std::unordered_map<int, std::unordered_set<int>>& answer) {
	this->synonyms.push_back(keySynonym);
	this->synonyms.push_back(valueSynonym);
	this->twoSynonymAnswer = answer;
}

void Result::setAnswer(const std::string& keySynonym, const std::string& valueSynonym, const std::unordered_map<int, std::unordered_set<std::string>>& answer, const std::unordered_map<std::string, int>& indexTable) {
	this->synonyms.push_back(keySynonym);
	this->synonyms.push_back(valueSynonym);
	for (auto pair : answer) {
		std::unordered_set<int> valueIntEntities = this->convertStringsToIndices(pair.second, indexTable);
		this->twoSynonymAnswer.insert({ pair.first, valueIntEntities });
	}
}

void Result::setAnswer(const std::string& keySynonym, const std::string& valueSynonym, const std::unordered_map<std::string, std::unordered_set<int>>& answer, const std::unordered_map<std::string, int>& indexTable) {
	this->synonyms.push_back(keySynonym);
	this->synonyms.push_back(valueSynonym);
	for (auto pair : answer) {
		this->twoSynonymAnswer.insert({ indexTable.at(pair.first), pair.second });
	}
}

void Result::setAnswer(const std::string& keySynonym, const std::string& valueSynonym, const std::unordered_map<std::string, std::unordered_set<std::string>>& answer, const std::unordered_map<std::string, int>& indexTable) {
	this->setAnswer(keySynonym, valueSynonym, answer, indexTable, indexTable);
}

void Result::setAnswer(const std::string& keySynonym, const std::string& valueSynonym, const std::unordered_map<std::string, std::unordered_set<std::string>>& answer, const std::unordered_map<std::string, int>& keyIndexTable, const std::unordered_map<std::string, int>& valueIndexTable) {
	this->synonyms.push_back(keySynonym);
	this->synonyms.push_back(valueSynonym);
	for (auto pair : answer) {
		int keyIntEntity = keyIndexTable.at(pair.first);
		std::unordered_set<int> valueIntEntities = this->convertStringsToIndices(pair.second, valueIndexTable);
		this->twoSynonymAnswer.insert({ keyIntEntity, valueIntEntities });
	}
}

// Function converts a given string set to int set based on the given indexTable
std::unordered_set<int> Result::convertStringsToIndices(const std::unordered_set<std::string>& stringSet, const std::unordered_map<std::string, int>& indexTable) {
	std::unordered_set<int> intSet;
	for (auto string : stringSet) {
		intSet.insert(indexTable.at(string));
	}
	return intSet;
}
