#include "Result.h"

bool Result::hasPassed() {
	return this->isPassed;
}

void Result::setPassed(bool isPassed) {
	this->isPassed = isPassed;
}

vector<string> Result::getSynonyms() {
	return this->synonyms;
}

int Result::getNumOfSyn() {
	return this->synonyms.size();
}

unordered_set<int> Result::getOneSynAnswer() {
	return this->oneSynonymAnswer;
}

unordered_map<int, unordered_set<int>> Result::getTwoSynAnswer() {
	return this->twoSynonymAnswer;
}

void Result::setAnswer(const string& synonym, const int& answer) {
	this->synonyms.push_back(synonym);
	this->oneSynonymAnswer.insert(answer);
}

void Result::setAnswer(const string& synonym, const unordered_set<int>& answer) {
	this->synonyms.push_back(synonym);
	this->oneSynonymAnswer = answer;
}

void Result::setAnswer(const string& synonym, const unordered_set<string>& answer, const unordered_map<string, int>& indexTable) {
	this->synonyms.push_back(synonym);
	for (auto stringEntity : answer) { // convert all strings to their corresponding index as per the given variable/procedure table
		oneSynonymAnswer.insert(indexTable.at(stringEntity));
	}
}

void Result::setAnswer(const string& keySynonym, const string& valueSynonym, const unordered_map<int, int>& answer) {
	this->synonyms.push_back(keySynonym);
	this->synonyms.push_back(valueSynonym);
	for (auto pair : answer) {
		this->twoSynonymAnswer.insert({ pair.first, {pair.second} });
	}
}

void Result::setAnswer(const string& keySynonym, const string& valueSynonym, const unordered_map<int, string>& answer, const unordered_map<string, int>& indexTable) {
	this->synonyms.push_back(keySynonym);
	this->synonyms.push_back(valueSynonym);
	for (auto pair : answer) {
		this->twoSynonymAnswer.insert({ pair.first, {indexTable.at(pair.second)} });
	}
}

void Result::setAnswer(const string& keySynonym, const string& valueSynonym, const unordered_map<int, unordered_set<int>>& answer) {
	this->synonyms.push_back(keySynonym);
	this->synonyms.push_back(valueSynonym);
	this->twoSynonymAnswer = answer;
}

void Result::setAnswer(const string& keySynonym, const string& valueSynonym, const unordered_map<int, unordered_set<string>>& answer, const unordered_map<string, int>& indexTable) {
	this->synonyms.push_back(keySynonym);
	this->synonyms.push_back(valueSynonym);
	for (auto pair : answer) {
		unordered_set<int> valueIntEntities = this->convertStringsToIndices(pair.second, indexTable);
		this->twoSynonymAnswer.insert({ pair.first, valueIntEntities });
	}
}

void Result::setAnswer(const string& keySynonym, const string& valueSynonym, const unordered_map<string, unordered_set<int>>& answer, const unordered_map<string, int>& indexTable) {
	this->synonyms.push_back(keySynonym);
	this->synonyms.push_back(valueSynonym);
	for (auto pair : answer) {
		this->twoSynonymAnswer.insert({ indexTable.at(pair.first), pair.second });
	}
}

void Result::setAnswer(const string& keySynonym, const string& valueSynonym, const unordered_map<string, unordered_set<string>>& answer, const unordered_map<string, int>& indexTable) {
	this->setAnswer(keySynonym, valueSynonym, answer, indexTable, indexTable);
}

void Result::setAnswer(const string& keySynonym, const string& valueSynonym, const unordered_map<string, unordered_set<string>>& answer, const unordered_map<string, int>& keyIndexTable, const unordered_map<string, int>& valueIndexTable) {
	this->synonyms.push_back(keySynonym);
	this->synonyms.push_back(valueSynonym);
	for (auto pair : answer) {
		int keyIntEntity = keyIndexTable.at(pair.first);
		unordered_set<int> valueIntEntities = this->convertStringsToIndices(pair.second, valueIndexTable);
		this->twoSynonymAnswer.insert({ keyIntEntity, valueIntEntities });
	}
}

// Function converts a given string set to int set based on the given indexTable
unordered_set<int> Result::convertStringsToIndices(const unordered_set<string>& stringSet, const unordered_map<string, int>& indexTable) {
	unordered_set<int> intSet;
	for (auto string : stringSet) {
		intSet.insert(indexTable.at(string));
	}
	return intSet;
}
