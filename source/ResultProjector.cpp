#include "ResultProjector.h"
#include "DesignEntity.h"
#include "PKB.h"
#include <iostream>
#include <string>

using namespace std;

static unordered_map<string, int> synonymTable;
static unordered_map<int, list<unordered_map<string, int>>> synonymResults;
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

unordered_map<int, list<unordered_map<string, int>>> ResultProjector::getSynonymResults() {
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
	// sort and remove duplicates
	projectedResults.sort();
	projectedResults.unique();

	return projectedResults;
}

list<string> ResultProjector::getSelectedClauseInTable(Type type, list<int> results, PKB pkb) {
	list<string> projectedResults;
	switch (type) {
		case Type::VARIABLE:
			projectedResults = convertVarIndexToVar(results, pkb);
			break;
		case Type::PROCEDURE:
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
		case Type::STATEMENT:
			results = pkb.getAllStmts();
			projectedResults = convertIndexToString(results, pkb);
			break;
		case Type::READ:
			results = pkb.getReadStmts();
			projectedResults = convertIndexToString(results, pkb);
			break;
		case Type::PRINT:
			results = pkb.getPrintStmts();
			projectedResults = convertIndexToString(results, pkb);
			break;
		/*case Type::CALL:	// call statements
			results = pkb.getAllVariables();
			break;*/
		case Type::WHILE:
			results = pkb.getWhileStmts();
			projectedResults = convertIndexToString(results, pkb);
			break;
		case Type::IF:
			results = pkb.getIfStmts();
			projectedResults = convertIndexToString(results, pkb);
			break;
		case Type::ASSIGN:
			results = pkb.getAssignStmts();
			projectedResults = convertIndexToString(results, pkb);
			break;
		case Type::VARIABLE:
			projectedResults = convertSetToList(pkb.getAllVariables());
			break;
		case Type::CONSTANT:
			results = pkb.getAllConstant();
			projectedResults = convertIndexToString(results, pkb);
			break;
		case Type::PROCEDURE:
			projectedResults = convertSetToList(pkb.getAllProcedures());
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

list<string> ResultProjector::convertSetToList(unordered_set<string> resultSet) {
	list<string> results;
	for (auto result : resultSet) {
		results.push_back(result);
	}
	return results;
}

bool ResultProjector::combineResults(unordered_set<int> queryResultsOneSynonym, vector<string> synonyms) { // one synonym
	// check if empty result is passed in
	if (queryResultsOneSynonym.empty() || synonyms.size() != 1) {
		return false;
	}

	combineOneSynonym(queryResultsOneSynonym, synonyms);

	if (synonymExists(synonyms.at(0))) {
		return true;
	}
	return false; // no more common combined results
}

bool ResultProjector::combineResults(unordered_map<int, unordered_set<int>> queryResultsTwoSynonyms, vector<string> synonyms) { // two synonyms
	// check if empty result is passed in
	if (queryResultsTwoSynonyms.empty() || synonyms.size() != 2) {
		return false;
	}
	
	combineTwoSynonyms(queryResultsTwoSynonyms, synonyms);

	if (synonymExists(synonyms.at(0)) && synonymExists(synonyms.at(1))) {
		return true;
	}
	return false; // no more common combined results
}

void ResultProjector::combineOneSynonym(unordered_set<int> queryResults, vector<string> synonyms) {
	string key = synonyms.at(0);

	if (synonymExists(key)) { // 1 common synonym
		filterOneSynInTable(key, queryResults);
	}
	else { // no common synonym
		addOneSyn(key, queryResults);
	}
}

void ResultProjector::combineTwoSynonyms(unordered_map<int, unordered_set<int>> queryResults, vector<string> synonyms) {
	string key1 = synonyms.at(0);
	string key2 = synonyms.at(1);
	bool key1Exists = synonymExists(key1);
	bool key2Exists = synonymExists(key2);

	if (key1Exists && key2Exists) { // 2 common synonyms

		if (synonymTable.at(key1) == synonymTable.at(key2)) { // Both synonyms in same table
			filterTwoSynInSameTable(key1, key2, queryResults);
		}
		else { // diff table
			mergeTables(key1, key2, queryResults);
		}
	}
	else if (key1Exists || key2Exists) { // 1 common synonym
		if (key1Exists) {
			mergeOneSyn(key1, key2, queryResults);
		}
		else {
			unordered_map<int, unordered_set<int>> invertedQueryResults = invertResults(queryResults);
			mergeOneSyn(key2, key1, invertedQueryResults);
		}
	}
	else { // no common synonyms
		addTwoSyn(key1, key2, queryResults);
	}
}

bool ResultProjector::synonymExists(string synonym) {
	if (!synonymTable.empty()) {
		if (synonymTable.find(synonym) != synonymTable.end()) {
			return true;
		}
	}
	return false;
}

void ResultProjector::addOneSyn(string key, unordered_set<int> results) {
	synonymTable[key] = index;

	list<unordered_map<string, int>> newSet;
	unordered_map<string, int> newSetEntry;
	for (const auto result : results) {
		newSetEntry[key] = result;
		newSet.push_back(newSetEntry);
	}

	synonymResults[index] = newSet;
	index++;
}

void ResultProjector::addTwoSyn(string key1, string key2, unordered_map<int, unordered_set<int>> results) {
	synonymTable[key1] = index;
	synonymTable[key2] = index;

	list<unordered_map<string, int>> newSet;
	unordered_map<string, int> newSetEntry;
	for (const auto result: results) {
		newSetEntry[key1] = result.first;
		for (const auto key2Results : result.second) {
			newSetEntry[key2] = key2Results;
			newSet.push_back(newSetEntry);
		}
	}

	synonymResults[index] = newSet;
	index++;
}

// Filters the synonym's results such that only overlapped results remains
void ResultProjector::filterOneSynInTable(string key, unordered_set<int> queryResults) {
	int tableNum = synonymTable[key];
	list<unordered_map<string, int>>& prevResults = synonymResults[tableNum];

	for (auto itr = prevResults.begin(); itr != prevResults.end();) {
		int prevKeyValue = (*itr).at(key);
		if (!existInSet(prevKeyValue, queryResults)) { // results do not overlap
			itr = prevResults.erase(itr);
		}
		else {
			itr++;
		}
	}
	cleanUpTables(key);
}

// Filters two synonyms results such that only overlapped results remains
void ResultProjector::filterTwoSynInSameTable(string key1, string key2, unordered_map<int, unordered_set<int>> queryResults) {
	int tableNum = synonymTable[key1];
	list<unordered_map<string, int>>& prevResults = synonymResults[tableNum];

	for (auto itr = prevResults.begin(); itr != prevResults.end();) {
		int prevKey1Value = (*itr).at(key1);
		int prevKey2Value = (*itr).at(key2);

		if (existInMap(prevKey1Value, queryResults)) {
			if (existInSet(prevKey2Value, queryResults[prevKey1Value])) { // results overlap
				itr++;
			}
			else {
				itr = prevResults.erase(itr);
			}
		}
		else {
			itr = prevResults.erase(itr);
		}
	}
	cleanUpTables(key1);
	cleanUpTables(key2);
}

void ResultProjector::mergeOneSyn(string existKey, string newKey, unordered_map<int, unordered_set<int>> queryResults) {
	int tableNum = synonymTable[existKey];
	list<unordered_map<string, int>>& prevResults = synonymResults[tableNum];

	for (auto itr = prevResults.begin(); itr != prevResults.end();) {
		int prevKey1Value = (*itr).at(existKey);

		if (existInMap(prevKey1Value, queryResults)) { // results overlap
			if (!existInMap(newKey, (*itr))) { // dependent result not added before (for when iterating to end of map when adding duplicated results)
				unordered_map<string, int> currRow = (*itr); // get a copy of current row result
				for (auto dependentResult : queryResults.at(prevKey1Value)) { // duplicate rows
					currRow[newKey] = dependentResult;
					prevResults.push_back(currRow);
				}
				itr = prevResults.erase(itr); // delete current row since updated row is added at the back
			}
			else { // ignore if newKey already exists because its the rows we duplicated
				break; // early break because all behind are the new rows just added
			}
		}
		else {
			itr = prevResults.erase(itr);
		}
	}
	cleanUpTables(existKey);

	if (synonymExists(existKey)) { // still have results after merging
		synonymTable[newKey] = tableNum;
	}
}

void ResultProjector::mergeTables(string key1, string key2, unordered_map<int, unordered_set<int>> queryResults) {
	int key1InitialTableNum = synonymTable[key1];
	int key2InitialTableNum = synonymTable[key2];

	list<unordered_map<string, int>> key1PrevResults = synonymResults[key1InitialTableNum];
	list<unordered_map<string, int>> key2PrevResults = synonymResults[key2InitialTableNum];
	list<unordered_map<string, int>> newResultsSet;

	for (auto key1PrevResult : key1PrevResults) {
		int prevKey1Value = key1PrevResult[key1];
		if (existInMap(prevKey1Value, queryResults)) { // results overlap for key1
			for (auto key2PrevResult : key2PrevResults) {
				int prevKey2Value = key2PrevResult[key2];
				if (existInSet(prevKey2Value, queryResults[prevKey1Value])) { // results overlap for key2
					unordered_map<string, int> newResult = key1PrevResult; // merge 2 maps together
					newResult.insert(key2PrevResult.begin(), key2PrevResult.end());
					newResultsSet.push_back(newResult);
				}
			}
		}
	}

	// delete tables
	synonymResults.erase(key1InitialTableNum);
	synonymResults.erase(key2InitialTableNum);

	// update synonymTable indexes
	unordered_map<string, int> synTable = synonymTable; // cannot just iterate using synonymTable, error when erasing
	for (auto synonym : synTable) {
		if (newResultsSet.size() != 0) {
			if (synonym.second == key1InitialTableNum || synonym.second == key2InitialTableNum) { // update to new table
				synonymTable[synonym.first] = index;
			}
		}
		else { // no more results, remove from table
			synonymTable.erase(synonym.first);
		}
	}
	
	if (newResultsSet.size() != 0) { // add in new table
		synonymResults[index] = newResultsSet;
		index++;
	}
}

bool ResultProjector::existInMap(int key, unordered_map<int, unordered_set<int>> umap) {
	if (!umap.empty()) {
		if (umap.find(key) != umap.end()) {
			return true;
		}
	}
	return false;
}

bool ResultProjector::existInMap(string key, unordered_map<string, int> umap) {
	if (!umap.empty()) {
		if (umap.find(key) != umap.end()) {
			return true;
		}
	}
	return false;
}

bool ResultProjector::existInSet(int key, unordered_set<int> uset) {
	if (!uset.empty()) {
		if (uset.find(key) != uset.end()) {
			return true;
		}
	}
	return false;
}

void ResultProjector::cleanUpTables(string key) {
	if (synonymTable.empty() || synonymResults.empty()) {
		return;
	}

	bool tableDeleted = false;
	// clean up synonymTable and synonymResults when there are no more entries in it
	int tableNum = synonymTable[key];
	if (synonymResults.find(tableNum) != synonymResults.end()) { // table exists
		if (synonymResults.at(tableNum).empty()) { // no more entries in table
			synonymResults.erase(tableNum);
			tableDeleted = true;
		}
	}

	// erase all synonyms associated with the table with no entries
	if (tableDeleted) {
		unordered_map<string, int> synTable = synonymTable; // cannot just iterate using synonymTable, error when erasing
		for (auto synonym : synTable) {
			if (synonym.second == tableNum) {
				synonymTable.erase(synonym.first);
			}
		}
	}
}

unordered_map<int, unordered_set<int>> ResultProjector::invertResults(unordered_map<int, unordered_set<int>> queryResults) {
	unordered_map<int, unordered_set<int>> newResults;

	for (auto queryResult : queryResults) {
		int key = queryResult.first;
		unordered_set<int> values = queryResult.second;

		for (auto value : values) {
			newResults[value].insert(key);
		}
	}
	return newResults;
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
		for (auto results : x.second) {
			for (auto result : results) {
				std::cout << result.first << ": " << result.second << ", ";
			}
		}
		cout << "------------------------------ " << endl;
	}
	cout << endl;
}
