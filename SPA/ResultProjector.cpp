#include "ResultProjector.h"
#include <iostream>
#include <string>

static unordered_map<string, int> synonymTable;
static unordered_map<int, unordered_map<string, list<int>>> synonymResults;
static int index;

// Reset tables for new query
void ResultProjector::resetResults()
{
	synonymTable.clear();
	synonymResults.clear();
	index = 0;
}

unordered_map<string, int> ResultProjector::getSynonymTable() {
	return synonymTable;
}

unordered_map<int, unordered_map<string, list<int>>> ResultProjector::getSynonymResults() {
	return synonymResults;
}

list<string> ResultProjector::getResults(string selectedSynonym) {
	int tableNum = synonymTable.at(selectedSynonym);
	list<int> results = synonymResults.at(tableNum).at(selectedSynonym);

	list<string> projectedResults;
	// TODO: convert int to var from varTable

	return projectedResults;
}

bool ResultProjector::combineResults(unordered_map<string, list<int>> queryResults) {
	vector<string> synonyms;

	for (auto synonym : queryResults)
		synonyms.push_back(synonym.first);

	if (synonyms.size() == 1) { // one synonym in a clause
		string key = synonyms.at(0);
		if (synonymExists(key)) { // 1 common synonym
			filterSyn(key, queryResults.at(key));
		}
		else { // no common synonym
			addOneSyn(key, queryResults);
		}
	}
	else if (synonyms.size() == 2) { // two synonyms in a clause
		string key1 = synonyms.at(0);
		string key2 = synonyms.at(1);
		bool key1Exists = synonymExists(key1);
		bool key2Exists = synonymExists(key2);

		if (key1Exists && key2Exists) { // 2 common synonyms
			filterSyn(key1, queryResults.at(key1));
			filterSyn(key2, queryResults.at(key2));

			// TODO: Check if same table
		}
		else if (key1Exists || key2Exists) { // 1 common synonym
			if (key1Exists) {
				filterSyn(key1, queryResults.at(key1));
				mergeOneSyn(key1, key2);
			}
			else {
				filterSyn(key2, queryResults.at(key2));
				mergeOneSyn(key2, key1);
			}
		}
		else { // no common synonyms
			addTwoSyn(key1, key2);
		}
	}
	return true; // To be edited
}

bool ResultProjector::synonymExists(string synonym) {
	if (synonymTable.find(synonym) != synonymTable.end()) {
		return true;
	}
	return false;
}

void ResultProjector::addOneSyn(string key, unordered_map<string, list<int>> results) {
	synonymTable[key] = index;
	synonymResults[index] = results;
	index++;
}

void ResultProjector::addTwoSyn(string key1, string key2) {

}

// Filters the synonym's results such that only overlapped results remains
void ResultProjector::filterSyn(string key, list<int> queryResults) {
	int tableNum = synonymTable.at(key);
	list<int>& prevResults = synonymResults.at(tableNum).at(key);
	int index = 0;

	for (auto itr = prevResults.begin(); itr != prevResults.end();) {
		if (indexDoesNotExist(*itr, queryResults)) { // results do not overlap
			itr = prevResults.erase(itr);
			eraseTableRow(index, synonymResults.at(tableNum), key);
		}
		else {
			itr++;
		}
		index++;
	}

	// clean up synonymTable and synonymResults when there are no more entries in it
	for (auto synonym : synonymResults.at(tableNum)) {
		if (synonym.second.empty()) { // remove synonym entry from synonym results if no more entries
			synonymResults.at(tableNum).erase(synonym.first);
			synonymTable.erase(key);

			if (synonymResults.at(tableNum).size() == 0)
				synonymResults.erase(tableNum);
			break;
		}
	}
}

bool ResultProjector::indexDoesNotExist(int index, list<int> resultsList) {
	return find(resultsList.begin(), resultsList.end(), index) == resultsList.end();
}

// Delete dependent variables' index in the same table as the deleted item
void ResultProjector::eraseTableRow(int index, unordered_map<string, list<int>>& synResTable, string key) {
	list<int>::iterator itr;

	for (auto synonym : synResTable) {
		if (synonym.first.compare(key) != 0) { // list containing cur key has already been deleted
			itr = synonym.second.begin();
			advance(itr, index);
			synonym.second.erase(itr);
		}
	}
}

void ResultProjector::mergeOneSyn(string existKey, string newKey) {

}

void ResultProjector::mergeDiffTable(string key1, string key2) {

}

// For debugging purposes
void ResultProjector::printTables() {
	cout << "SynonymTable: " << endl;
	for (auto x : synonymTable) {
		cout << x.first << " " << std::to_string(x.second) << endl;
	}
	cout << "SynonymResults: " << endl;
	for (auto x : synonymResults) {
		cout << "Table " << x.first << "-- ";
		for (auto v : x.second) {
			std::cout << v.first << ": ";
			for (auto z : v.second)
				cout << z << " ";
		}
	}
	cout << endl;
}
