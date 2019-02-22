#include "ResultProjector.h"
#include "DesignEntity.h"
#include "../source/PKB.h"
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

list<string> ResultProjector::getResults(DesignEntity selectedSynonym, PKB pkb) {
	list<string> projectedResults;
	Type type = selectedSynonym.getType();

	if (synonymExists(selectedSynonym.getValue())) {
		int tableNum = synonymTable.at(selectedSynonym.getValue());
		list<int> results = synonymResults.at(tableNum).at(selectedSynonym.getValue());

		projectedResults = getSelectedClauseInTable(type, results, pkb);
	}
	else {
		projectedResults = getSelectedClauseNotInTable(type, pkb);
	}
	return projectedResults;
}

list<string> ResultProjector::getSelectedClauseInTable(Type type, list<int> results, PKB pkb) {
	list<string> projectedResults;
	switch (type) {
		case VARIABLE:
			projectedResults = convertVarIndexToVar(results, pkb);
			break;
		case PROCEDURE:
			projectedResults = convertProcIndexToProc(results, pkb);
			break;
		default: // STATEMENTS and CONSTANTS
			projectedResults = convertIndexToString(results, pkb);
			break;
		}
	return projectedResults;
}

list<string> ResultProjector::getSelectedClauseNotInTable(Type type, PKB pkb) {
	unordered_set<int> results;
	list<string> projectedResults;
	//STATEMENT, READ, PRINT, CALL, WHILE, IF, ASSIGN, VARIABLE, CONSTANT, PROCEDURE, UNDERSCORE, FIXED

	switch (type) {
		case STATEMENT:
			results = pkb.getAllStmts();
			projectedResults = convertIndexToString(results, pkb);
			break;
		case READ:
			results = pkb.getReadStmts();
			projectedResults = convertIndexToString(results, pkb);
			break;
		case PRINT:
			results = pkb.getPrintStmts();
			projectedResults = convertIndexToString(results, pkb);
			break;
		/*case CALL:	// call statements
			results = pkb.getAllVariables();
			break;*/
		case WHILE:
			results = pkb.getWhileStmts();
			projectedResults = convertIndexToString(results, pkb);
			break;
		case IF:
			results = pkb.getIfStmts();
			projectedResults = convertIndexToString(results, pkb);
			break;
		case ASSIGN:
			results = pkb.getAssignStmts();
			projectedResults = convertIndexToString(results, pkb);
			break;
		case VARIABLE:
			results = pkb.getAllVariables();
			projectedResults = convertVarIndexToVar(results, pkb);
			break;
		case CONSTANT:
			results = pkb.getAllConstant();
			projectedResults = convertIndexToString(results, pkb);
			break;
		case PROCEDURE:
			results = pkb.getAllProcedures();
			projectedResults = convertProcIndexToProc(results, pkb);
			break;
		}
	return projectedResults;
}

list<string> ResultProjector::convertVarIndexToVar(unordered_set<int> resultSet, PKB pkb) {
	list<string> results;
	for (auto varIndex : resultSet) {
		results.push_back(pkb.getVarAtIdx(varIndex));
	}
	return results;
}

list<string> ResultProjector::convertVarIndexToVar(list<int> resultList, PKB pkb) {
	list<string> results;
	for (auto varIndex : resultList) {
		results.push_back(pkb.getVarAtIdx(varIndex));
	}
	return results;
}

list<string> ResultProjector::convertProcIndexToProc(unordered_set<int> resultSet, PKB pkb) {
	list<string> results;
	for (auto procIndex : resultSet) {
		results.push_back(pkb.getProcAtIdx(procIndex)); // new API from PKB
	}
	return results;
}

list<string> ResultProjector::convertProcIndexToProc(list<int> resultList, PKB pkb) {
	list<string> results;
	for (auto procIndex : resultList) {
		results.push_back(pkb.getProcAtIdx(procIndex)); // new API from PKB
	}
	return results;
}

list<string> ResultProjector::convertIndexToString(unordered_set<int> resultSet, PKB pkb) {
	list<string> results;
	for (auto resultIndex : resultSet) {
		results.push_back(to_string(resultIndex));
	}
	return results;
}

list<string> ResultProjector::convertIndexToString(list<int> resultList, PKB pkb) {
	list<string> results;
	for (auto resultIndex : resultList) {
		results.push_back(to_string(resultIndex));
	}
	return results;
}

bool ResultProjector::combineResults(unordered_map<string, list<int>> queryResults) {
	// check if empty result is passed in
	if (queryResults.empty()) {
		return false;
	}
	for (auto queryResult : queryResults) {
		if (queryResult.second.empty()) {
			return false;
		}
	}

	// main logic
	vector<string> synonyms;

	for (auto synonym : queryResults)
		synonyms.push_back(synonym.first);

	if (synonyms.size() == 1) { // one synonym in a clause
		string key = synonyms.at(0);
		if (synonymExists(key)) { // 1 common synonym
			filterSynInTable(key, queryResults.at(key));
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
			filterSynInTable(key1, queryResults.at(key1));
			filterSynInTable(key2, queryResults.at(key2));

			if (synonymTable.at(key1) == synonymTable.at(key2)) { // Both synonyms in same table
				combineDependentResults(key1, key2, queryResults);
			}
			else { // diff table
				// key1 has larger existing table. Merge key2 table with key1 table
				if (synonymResults.at(synonymTable.at(key1)).size() > synonymResults.at(synonymTable.at(key2)).size()) {
					int key2InitialTableNum = synonymTable.at(key2);

					mergeOneSyn(key1, key2, queryResults);
					filterSynInTable(key2, synonymResults.at(key2InitialTableNum).at(key2));
					filterOldTable(key2InitialTableNum, key2);
					mergeTables(key2InitialTableNum, key2, synonymResults.at(synonymTable.at(key1)).at(key1));
				}
				else {
					int key1InitialTableNum = synonymTable.at(key1);

					mergeOneSyn(key2, key1, queryResults);
					filterSynInTable(key1, synonymResults.at(key1InitialTableNum).at(key1));
					filterOldTable(key1InitialTableNum, key1);
					mergeTables(key1InitialTableNum, key1, synonymResults.at(synonymTable.at(key2)).at(key2));
				}
			}
		}
		else if (key1Exists || key2Exists) { // 1 common synonym
			if (key1Exists) {
				filterSynInTable(key1, queryResults.at(key1));
				if (synonymExists(key1)) {
					mergeOneSyn(key1, key2, queryResults);
				}
			}
			else {
				filterSynInTable(key2, queryResults.at(key2));
				if (synonymExists(key2)) {
					mergeOneSyn(key2, key1, queryResults);
				}
			}
		}
		else { // no common synonyms
			addTwoSyn(key1, key2, queryResults);
		}
	}

	if (synonymExists(synonyms.at(0))) {
		return true;
	}
	return false; // no more common combined results
}

bool ResultProjector::synonymExists(string synonym) {
	if (!synonymTable.empty()) {
		if (synonymTable.find(synonym) != synonymTable.end()) {
			return true;
		}
	}
	return false;
}

void ResultProjector::addOneSyn(string key, unordered_map<string, list<int>> results) {
	synonymTable[key] = index;
	synonymResults[index] = results;
	index++;
}

void ResultProjector::addTwoSyn(string key1, string key2, unordered_map<string, list<int>> results) {
	synonymTable[key1] = index;
	synonymTable[key2] = index;
	synonymResults[index] = results;
	index++;
}

// Filters the synonym's results such that only overlapped results remains
void ResultProjector::filterSynInTable(string key, list<int> queryResults) {
	int tableNum = synonymTable.at(key);
	list<int>& prevResults = synonymResults.at(tableNum).at(key);
	int rowIndex = 0;

	for (auto itr = prevResults.begin(); itr != prevResults.end();) {
		if (!resultExists(*itr, queryResults)) { // results do not overlap
			eraseTableRow(rowIndex, synonymResults.at(tableNum), key);
			itr = prevResults.erase(itr);
			rowIndex--;
		}
		else {
			itr++;
		}
		rowIndex++;
	}
	cleanUpTables(key);
}

void ResultProjector::cleanUpTables(string key) {
	if (synonymTable.empty()) {
		return;
	}

	// clean up synonymTable and synonymResults when there are no more entries in it
	int tableNum = synonymTable.at(key);

	unordered_map<string, list<int>> table = synonymResults.at(tableNum); // cannot just iterate using synonymResults.at(tableNum), error when erasing
	for (auto synonym : table) {
		if (synonym.second.empty()) { // remove synonym entry from synonym results if no more entries
			string synonymToDelete = synonym.first;
			synonymResults.at(tableNum).erase(synonymToDelete);
			synonymTable.erase(synonymToDelete);
		}
	}

	if (synonymResults.at(tableNum).empty()) { // no more entries in table
		synonymResults.erase(tableNum);
	}
}

void ResultProjector::filterOldTable(int initialTableNum, string key) {
	list<int>& prevResults = synonymResults.at(initialTableNum).at(key);
	list<int> updatedResults = synonymResults.at(synonymTable.at(key)).at(key);
	int rowIndex = 0;

	for (auto itr = prevResults.begin(); itr != prevResults.end();) {
		if (!resultExists(*itr, updatedResults)) { // results do not overlap
			eraseTableRow(rowIndex, synonymResults.at(initialTableNum), key);
			itr = prevResults.erase(itr);
			rowIndex--;
		}
		else {
			itr++;
		}
		rowIndex++;
	}
	cleanUpTables(key);
}

void ResultProjector::mergeTables(int initialTableNum, string mergingKey, list<int> updatedResults) {
	unordered_map<string, list<int>> prevTable = synonymResults.at(initialTableNum);

	int newTableIndex = synonymTable.at(mergingKey);
	unordered_map<string, list<int>> curTable = synonymResults.at(synonymTable.at(mergingKey));
	list<int> curResults = curTable.at(mergingKey);

	for (auto synonym : prevTable) {
		if (synonym.first != mergingKey) {
			list<int> newKeyResultsSorted;
			for (auto curResult : curResults) {
				int listIndex = findIndex(curResult, prevTable.at(mergingKey));
				list<int>::iterator itr;
				itr = prevTable.at(synonym.first).begin();
				advance(itr, listIndex);
				newKeyResultsSorted.push_back(*itr);
			}
			synonymTable[synonym.first] = newTableIndex;
			curTable[synonym.first] = newKeyResultsSorted;
		}
	}
	synonymResults.erase(initialTableNum);
}

bool ResultProjector::resultExists(int target, list<int> resultsList) {
	return find(resultsList.begin(), resultsList.end(), target) != resultsList.end();
}

// Delete dependent variables' index in the same table as the deleted item
void ResultProjector::eraseTableRow(int rowIndex, unordered_map<string, list<int>>& synResTable, string key) {
	list<int>::iterator itr;

	for (auto& synonym : synResTable) {
		if (synonym.first.compare(key) != 0) { // list containing cur key has already been deleted
			itr = synonym.second.begin();
			advance(itr, rowIndex);
			synonym.second.erase(itr);
		}
	}
}

void ResultProjector::mergeOneSyn(string existKey, string newKey, unordered_map<string, list<int>> results) {
	int tableIndex = synonymTable.at(existKey);
	unordered_map<string, list<int>>& resultTable = synonymResults.at(tableIndex);
	list<int> prevResults = resultTable.at(existKey);

	list<int> newKeyResultsSorted;
	list<int>::iterator itr;

	// merging with current results
	for (auto prevResult : prevResults) {
		if (resultExists(prevResult, results.at(existKey))) {
			newKeyResultsSorted.push_back(getAndDeleteCorrespondingResult(prevResult, existKey, newKey, results));
		}
	}

	// repeated results in new results
	if (!results.at(existKey).empty()) {
		// for dependent pair
		list<int> existKeyResults = results.at(existKey);
		list<int> newKeyResults = results.at(newKey);
		int listIndex = 0;

		for (auto existKeyResult : existKeyResults) {
			if (resultExists(existKeyResult, prevResults)) { // result overlaps with existing results
				duplicateResultsForRestOfTable(findIndex(existKeyResult, prevResults), resultTable);
				newKeyResultsSorted.push_back(getCorrespondingResult(listIndex, newKeyResults));
			}
			listIndex++;
		}
	}

	// repeated results in old results
	if (prevResults.size() > newKeyResultsSorted.size()) {
		int startIndex = newKeyResultsSorted.size();
		list<int>::iterator itr;
		for (int i = startIndex; i < prevResults.size(); i++) {
			itr = prevResults.begin();
			advance(itr, i);
			int listIndex = 0;
			for (auto prevResult : prevResults) {
				if (prevResult == *itr) {
					newKeyResultsSorted.push_back(getCorrespondingResult(listIndex, newKeyResultsSorted));
					break;
				}
				listIndex++;
			}
		}
	}

	synonymTable[newKey] = tableIndex;
	resultTable[newKey] = newKeyResultsSorted;
}

void ResultProjector::duplicateResultsForRestOfTable(int listIndex, unordered_map<string, list<int>>& table) {
	list<int>::iterator itr;

	for (auto& synonym : table) {
		itr = synonym.second.begin();
		advance(itr, listIndex);
		synonym.second.push_back(*itr);
		continue;
	}
}

int ResultProjector::findIndex(int target, list<int> targetList) {
	int listIndex = 0;

	for (auto result : targetList) {
		if (result != target) {
			listIndex++;
		}
		else {
			break;
		}
	}
	return listIndex;
}

int ResultProjector::getCorrespondingResult(int listIndex, list<int> resultList) {
	list<int>::iterator itr;
	itr = resultList.begin();
	advance(itr, listIndex);

	return *itr;
}

int ResultProjector::getAndDeleteCorrespondingResult(int prevResult, string existKey, string newKey, unordered_map<string, list<int>>& results) {
	list<int>& existKeyResults = results.at(existKey);
	list<int>& newKeyResults = results.at(newKey);
	list<int>::iterator itr;
	int listIndex = 0;

	for (itr = existKeyResults.begin(); itr != existKeyResults.end(); ++itr) {
		if (*itr == prevResult) { // overlapping results
			break;
		}
		else {
			listIndex++;
		}
	}
	// delete result from result list so will not add in again subsequently
	existKeyResults.erase(itr);

	itr = newKeyResults.begin();
	advance(itr, listIndex);
	int correspondingResult = *itr;
	newKeyResults.erase(itr);

	return correspondingResult;
}

void ResultProjector::combineDependentResults(string key1, string key2, unordered_map<string, list<int>> results) {
	list<int> prevResultsKey1 = synonymResults.at(synonymTable.at(key1)).at(key1);
	list<int> prevResultsKey2 = synonymResults.at(synonymTable.at(key2)).at(key2);

	int loopIndex = 0;
	int listIndex = 0;

	for (auto prevResultKey1 : prevResultsKey1) {
		int dependentResult = getCorrespondingResult(loopIndex, prevResultsKey2);
		if (!dependentResultExists(key1, key2, prevResultKey1, dependentResult, results)) {
			eraseTableRow(listIndex, synonymResults.at(synonymTable.at(key1)), ""); // empty string passed because erase entire row
			listIndex--;
		}
		listIndex++;
		loopIndex++;
	}
	cleanUpTables(key1);
	cleanUpTables(key2);
}

bool ResultProjector::dependentResultExists(string key1, string key2, int prevResultKey1, int dependentResult, unordered_map<string, list<int>> results) {
	int listIndex = 0;

	for (auto result : results.at(key1)) {
		if (result == prevResultKey1) {
			if (getCorrespondingResult(listIndex, results.at(key2)) == dependentResult) {
				return true;
			}
		}
		listIndex++;
	}
	return false;
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
