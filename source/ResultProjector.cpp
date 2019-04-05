#include "ResultProjector.h"

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

void ResultProjector::setSynonymTable(unordered_map<string, int> synTable) {
	synonymTable = synTable;
}

unordered_map<int, list<unordered_map<string, int>>> ResultProjector::getSynonymResults() {
	return synonymResults;
}

void ResultProjector::setSynonymResults(unordered_map<int, list<unordered_map<string, int>>> synResults) {
	synonymResults = synResults;
}

list<string> ResultProjector::getResults(vector<DesignEntity> selectedSynonyms, PKB pkb) {

	list<string> projectedResults;

	// early return if it's BOOLEAN. Just put in case.
	// actually if no common results, should be return false and break out of loop in QueryEvaluator and return FALSE already.
	// No need to wait till getResults()
	if (selectedSynonyms.size() == 1) {
		if (selectedSynonyms.at(0).getType() == Type::BOOLEAN) { // FALSE boolean handled in QueryEvaluator during early return
			projectedResults.push_back("TRUE");
			return projectedResults;
		}
	}

	unordered_map<int, list<DesignEntity>> selectedSynonymTableMap;

	// 1. Loop through all selected synonyms and get their table numbers
	for (auto selectedSynonym : selectedSynonyms) {
		string synonym = selectedSynonym.getValue();
		int tableNum;
		if (synonymExists(synonym)) {
			tableNum = synonymTable[selectedSynonym.getValue()];
		}
		else {
			tableNum = -1;
		}
		selectedSynonymTableMap[tableNum].push_back(selectedSynonym);
	}

	// 2. For each tableNum, put selected results in an unordered_map
	vector<list<unordered_map<string, string>>> allTableResults;
	for (auto tableMap : selectedSynonymTableMap) {
		list<unordered_map<string, string>> selectedResults;
		if (tableMap.first == -1) { // synonym does not exist in table
			for (auto selectedSyn : tableMap.second) {
				selectedResults = getSelectedClauseNotInTable(selectedSyn, pkb);
				allTableResults.push_back(selectedResults);
			}
		}
		else {
			list<unordered_map<string, int>> results = synonymResults[tableMap.first];
			for (auto result : results) {
				unordered_map<string, string> rowResult;
				for (auto selectedSyn : tableMap.second) {
					string convertedResult = convertSynonymResultToRequired(selectedSyn.getType(), result.at(selectedSyn.getValue()), selectedSyn.getAttrRef(), pkb);
					if (selectedSyn.getType() == READ || selectedSyn.getType() == Type::PRINT || selectedSyn.getType() == Type::CALL) {
						string key = selectedSyn.getValue() + to_string(selectedSyn.getAttrRef());
						rowResult[key] = convertedResult;
					}
					else {
						rowResult[selectedSyn.getValue()] = convertedResult;
					}
				}
				selectedResults.push_back(rowResult);
			}
			allTableResults.push_back(selectedResults);
		}
	}

	// 3. Cross product all maps
	list<unordered_map<string, string>> finalMaps;
	if (!allTableResults.empty()) {
		finalMaps = allTableResults.at(0);
		for (size_t i = 1; i < allTableResults.size(); i++) {
			list<unordered_map<string, string>> tableResults = allTableResults.at(i);

			int size = finalMaps.size();
			int count = 0; // for early break, no need loop through all that just added

			for (auto& itr = finalMaps.begin(); itr != finalMaps.end();) {
				if (count == size) {
					break;
				}
				for (auto tableResult : tableResults) {
					unordered_map<string, string> newResult = (*itr); // merge 2 maps together
					newResult.insert(tableResult.begin(), tableResult.end());
					finalMaps.push_back(newResult);
				}
				itr = finalMaps.erase(itr);
				count++;
			}
		}
	}

	// 4. Convert to required format
	if (!selectedSynonyms.empty()) {
		for (auto finalMap : finalMaps) {
			string resultString;

			DesignEntity firstSelectedSyn = selectedSynonyms.at(0);
			if (firstSelectedSyn.getType() == Type::READ || firstSelectedSyn.getType() == Type::PRINT || firstSelectedSyn.getType() == Type::CALL) {
				string key = firstSelectedSyn.getValue() + to_string(firstSelectedSyn.getAttrRef());
				resultString = finalMap[key];
			}
			else {
				resultString = finalMap[firstSelectedSyn.getValue()];
			}
			
			for (size_t i = 1; i < selectedSynonyms.size(); i++) {
				Type selectedSynType = selectedSynonyms.at(i).getType();
				if (selectedSynType == Type::READ || selectedSynType == Type::PRINT || selectedSynType == Type::CALL) {
					string key = selectedSynonyms.at(i).getValue() + to_string(selectedSynonyms.at(i).getAttrRef());
					resultString += " " + finalMap[key];
				}
				else {
					resultString += " " + finalMap[selectedSynonyms.at(i).getValue()];
				}
			}
			projectedResults.push_back(resultString);
		}
	}
	return projectedResults;
}

string ResultProjector::convertSynonymResultToRequired(Type type, int result, AttrRef attrRef, PKB pkb) {
	string convertedResult;
	switch (type) {
	case Type::VARIABLE:
		convertedResult = pkb.getVarAtIdx(result);
		break;
	case Type::PROCEDURE:
		convertedResult = pkb.getProcAtIdx(result);
		break;
	case Type::CALL:
		if (attrRef == AttrRef::PROC_NAME) {
			convertedResult = pkb.getCallAtIdx(result);
		}
		else {
			convertedResult = to_string(result);
		}
		break;
	case Type::READ:
		if (attrRef == AttrRef::VAR_NAME) {
			convertedResult = pkb.getReadAtIdx(result);
		}
		else {
			convertedResult = to_string(result);
		}
		break;
	case Type::PRINT:
		if (attrRef == AttrRef::VAR_NAME) {
			convertedResult = pkb.getPrintAtIdx(result);
		}
		else {
			convertedResult = to_string(result);
		}
		break;
	default: // EVERYTHING ELSE
		convertedResult = to_string(result);
		break;
	}
	return convertedResult;
}

list<unordered_map<string, string>> ResultProjector::getSelectedClauseNotInTable(DesignEntity synonym, PKB pkb) {
	list<unordered_map<string, string>> projectedResults;
	//STATEMENT, READ, PRINT, CALL, WHILE, IF, ASSIGN, VARIABLE, CONSTANT, PROCEDURE, UNDERSCORE, FIXED --> need to update

	Type type = synonym.getType();

	switch (type) {
	case Type::STATEMENT:
		projectedResults = convertSetToList(synonym, pkb.getAllStmts());
		break;
	case Type::PROGLINE:
		projectedResults = convertSetToList(synonym, pkb.getAllStmts());
		break;
	case Type::READ:
		if (synonym.getAttrRef() == AttrRef::VAR_NAME) {
			projectedResults = convertSetToList(synonym, pkb.getReadVarNames());
		}
		else {
			projectedResults = convertSetToList(synonym, pkb.getReadStmts());
		}
		break;
	case Type::PRINT:
		if (synonym.getAttrRef() == AttrRef::VAR_NAME) {
			projectedResults = convertSetToList(synonym, pkb.getPrintVarNames());
		}
		else {
			projectedResults = convertSetToList(synonym, pkb.getPrintStmts());
		}
		break;
	case Type::CALL:	// call statements
		if (synonym.getAttrRef() == AttrRef::PROC_NAME) {
			projectedResults = convertSetToList(synonym, pkb.getCallProcNames());
		}
		else {
			projectedResults = convertSetToList(synonym, pkb.getCallStmts());
		}
		break;
	case Type::WHILE:
		projectedResults = convertSetToList(synonym, pkb.getWhileStmts());
		break;
	case Type::IF:
		projectedResults = convertSetToList(synonym, pkb.getIfStmts());
		break;
	case Type::ASSIGN:
		projectedResults = convertSetToList(synonym, pkb.getAssignStmts());
		break;
	case Type::VARIABLE:
		projectedResults = convertSetToList(synonym, pkb.getAllVariables());
		break;
	case Type::CONSTANT:
		projectedResults = convertSetToList(synonym, pkb.getAllConstant());
		break;
	case Type::PROCEDURE:
		projectedResults = convertSetToList(synonym, pkb.getAllProcedures());
		break;
	}
	return projectedResults;
}

list<unordered_map<string, string>> ResultProjector::convertSetToList(DesignEntity synonym, unordered_set<string> resultSet) {
	list<unordered_map<string, string>> results;
	unordered_map<string, string> row;
	Type type = synonym.getType();
	for (string result : resultSet) {
		if (type == Type::READ || type == Type::PRINT || type == Type::CALL) {
			string key = synonym.getValue() + to_string(synonym.getAttrRef());
			row[key] = result;
		}
		else {
			row[synonym.getValue()] = result;
		}
		results.push_back(row);
	}
	return results;
}

list<unordered_map<string, string>> ResultProjector::convertSetToList(DesignEntity synonym, unordered_set<int> resultSet) {
	list<unordered_map<string, string>> results;
	unordered_map<string, string> row;
	Type type = synonym.getType();
	for (int result : resultSet) {
		if (type == Type::READ || type == Type::PRINT || type == Type::CALL) {
			string key = synonym.getValue() + to_string(synonym.getAttrRef());
			row[key] = to_string(result);
		}
		else {
			row[synonym.getValue()] = to_string(result);
		}
		results.push_back(row);
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

unordered_set<int> ResultProjector::getPossibleValues(string synonym) {
	unordered_set<int> possibleValues;

	if (synonymExists(synonym)) {
		int tableNum = synonymTable.at(synonym);
		list<unordered_map<string, int>> resultTable = synonymResults.at(tableNum);
		for (auto result : resultTable) {
			possibleValues.insert(result.at(synonym));
		}
	}
	return possibleValues;
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
	for (const auto result : results) {
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
		if (synonym.second == key1InitialTableNum || synonym.second == key2InitialTableNum) {
			if (newResultsSet.size() != 0) { // update to new table
				synonymTable[synonym.first] = index;
			}
			else { // no more results, remove from table
				synonymTable.erase(synonym.first);
			}
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
