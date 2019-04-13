#include "ResultProjector.h"

ResultCache affectsCache;
ResultCache affectsTCache;
ResultCache nextTCache;
static std::unordered_map<std::string, int> synonymTable;
static std::unordered_map<int, std::list<std::unordered_map<std::string, int>>> synonymResults;
static int index;

// Reset tables for new query
void ResultProjector::resetResults()
{
	affectsCache.resetCache();
	affectsTCache.resetCache();
	nextTCache.resetCache();
	synonymTable.clear();
	synonymResults.clear();
	index = 0;
}

std::list<std::string> ResultProjector::getResults(std::vector<DesignEntity> selectedSynonyms, PKB pkb) {

	std::list<std::string> projectedResults;

	// early return if it's BOOLEAN. Just put in case.
	// actually if no common results, should be return false and break out of loop in QueryEvaluator and return FALSE already.
	// No need to wait till getResults()
	if (selectedSynonyms.size() == 1) {
		if (selectedSynonyms.at(0).getType() == Type::BOOLEAN) { // FALSE boolean handled in QueryEvaluator during early return
			projectedResults.push_back("TRUE");
			return projectedResults;
		}
	}

	std::unordered_map<int, std::list<DesignEntity>> selectedSynonymTableMap;
	std::unordered_map<std::string, std::list<DesignEntity>> selectedSynNotInTableMap;

	// 1. Loop through all selected synonyms and get their table numbers
	for (auto selectedSynonym : selectedSynonyms) {
		std::string synonym = selectedSynonym.getValue();
		if (synonymExists(synonym)) {
			int tableNum = synonymTable[selectedSynonym.getValue()];
			selectedSynonymTableMap[tableNum].push_back(selectedSynonym);
		}
		else {
			std::string synonym = selectedSynonym.getValue();
			selectedSynNotInTableMap[synonym].push_back(selectedSynonym);
		}
	}

	// 2. For each tableNum, put selected results in an unordered_map
	std::vector<std::list<std::unordered_map<std::string, std::string>>> allTableResults;
	// 2a. Results not in table
	for (auto selectedSynonym : selectedSynNotInTableMap) {
		std::list<std::unordered_map<std::string, std::string>> selectedResults;
		if (selectedSynonym.second.size() != 0) {
			DesignEntity firstSynonymEntity = *(selectedSynonym.second.begin());
			Type type = firstSynonymEntity.getType();
			std::unordered_set<std::string> results = getAllResults(type, pkb);

			std::unordered_map<std::string, std::string> rowResult;
			for (std::string result : results) {
				for (DesignEntity selectedSyn : selectedSynonym.second) {
					if (selectedSyn.getType() == Type::READ || selectedSyn.getType() == Type::PRINT || selectedSyn.getType() == Type::CALL) {
						std::string key = selectedSyn.getValue() + to_string(selectedSyn.getAttrRef());
						rowResult[key] = convertSynonymResultToRequired(type, result, selectedSyn.getAttrRef(), pkb);
					}
					else {
						rowResult[selectedSyn.getValue()] = result;
					}
				}
				selectedResults.push_back(rowResult);
			}
		}
		allTableResults.push_back(selectedResults);
	}

	// 2b. Results in table
	for (auto tableMap : selectedSynonymTableMap) {
		std::list<std::unordered_map<std::string, std::string>> selectedResults;
		std::unordered_set<std::string> testForDuplicatesSet;
		std::list<std::unordered_map<std::string, int>> results = synonymResults[tableMap.first];
		for (auto result : results) {
			std::string testForDuplicates = "";
			std::unordered_map<std::string, std::string> rowResult;
			for (auto selectedSyn : tableMap.second) {
				std::string convertedResult = convertSynonymResultToRequired(selectedSyn.getType(), result.at(selectedSyn.getValue()), selectedSyn.getAttrRef(), pkb);
				if (selectedSyn.getType() == Type::READ || selectedSyn.getType() == Type::PRINT || selectedSyn.getType() == Type::CALL) {
					std::string key = selectedSyn.getValue() + to_string(selectedSyn.getAttrRef());
					rowResult[key] = convertedResult;
				}
				else {
					rowResult[selectedSyn.getValue()] = convertedResult;
				}
				testForDuplicates += convertedResult + " ";
			}
			if (testForDuplicatesSet.find(testForDuplicates) == testForDuplicatesSet.end()) { // only insert result if it is not a duplicate. Reduce cross product time
				testForDuplicatesSet.insert(testForDuplicates);
				selectedResults.push_back(rowResult);
			}
		}
		allTableResults.push_back(selectedResults);
	}

	// 3. Cross product all maps
	std::list<std::unordered_map<std::string, std::string>> finalMaps;
	if (!allTableResults.empty()) {
		finalMaps = allTableResults.at(0);
		for (size_t i = 1; i < allTableResults.size(); i++) {
			std::list<std::unordered_map<std::string, std::string>> tableResults = allTableResults.at(i);

			int size = finalMaps.size();
			int count = 0; // for early break, no need loop through all that just added

			for (auto& itr = finalMaps.begin(); itr != finalMaps.end();) {
				if (count == size) {
					break;
				}
				for (auto tableResult : tableResults) {
					std::unordered_map<std::string, std::string> newResult = (*itr); // merge 2 maps together
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
			std::string resultString;

			DesignEntity firstSelectedSyn = selectedSynonyms.at(0);
			if (firstSelectedSyn.getType() == Type::READ || firstSelectedSyn.getType() == Type::PRINT || firstSelectedSyn.getType() == Type::CALL) {
				std::string key = firstSelectedSyn.getValue() + to_string(firstSelectedSyn.getAttrRef());
				resultString = finalMap[key];
			}
			else {
				resultString = finalMap[firstSelectedSyn.getValue()];
			}
			
			for (size_t i = 1; i < selectedSynonyms.size(); i++) {
				Type selectedSynType = selectedSynonyms.at(i).getType();
				if (selectedSynType == Type::READ || selectedSynType == Type::PRINT || selectedSynType == Type::CALL) {
					std::string key = selectedSynonyms.at(i).getValue() + to_string(selectedSynonyms.at(i).getAttrRef());
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

std::unordered_set<std::string> ResultProjector::getAllResults(Type type, PKB pkb) {
	std::unordered_set<std::string> results;

	switch (type) {
	case Type::STATEMENT:
		results = convertSetToString(pkb.getAllStmts());
		break;
	case Type::PROGLINE:
		results = convertSetToString(pkb.getAllStmts());
		break;
	case Type::READ:
		results = convertSetToString(pkb.getReadStmts());
		break;
	case Type::PRINT:
		results = convertSetToString(pkb.getPrintStmts());
		break;
	case Type::CALL:
		results = convertSetToString(pkb.getCallStmts());
		break;
	case Type::WHILE:
		results = convertSetToString(pkb.getWhileStmts());
		break;
	case Type::IF:
		results = convertSetToString(pkb.getIfStmts());
		break;
	case Type::ASSIGN:
		results = convertSetToString(pkb.getAssignStmts());
		break;
	case Type::VARIABLE:
		results = pkb.getAllVariables();
		break;
	case Type::CONSTANT:
		results = convertSetToString(pkb.getAllConstant());
		break;
	case Type::PROCEDURE:
		results = pkb.getAllProcedures();
		break;
	case Type::SWITCH:
		results = convertSetToString(pkb.getSwitchStmts());
		break;
	}
	return results;
}

std::unordered_set<std::string> ResultProjector::convertSetToString(std::unordered_set<int> resultSet) {
	std::unordered_set<std::string> convertedSet;
	for (int result : resultSet) {
		convertedSet.insert(to_string(result));
	}
	return convertedSet;
}

std::string ResultProjector::convertSynonymResultToRequired(Type type, std::string result, AttrRef attrRef, PKB pkb) {
	int resultInt = stoi(result);
	std::string convertedResult;
	
	switch (type) {
	case Type::CALL:
		if (attrRef == AttrRef::PROC_NAME) {
			convertedResult = pkb.getCallAtStmtNum(resultInt);
		}
		else {
			convertedResult = result;
		}
		break;
	case Type::READ:
		if (attrRef == AttrRef::VAR_NAME) {
			convertedResult = pkb.getReadAtStmtNum(resultInt);
		}
		else {
			convertedResult = result;
		}
		break;
	case Type::PRINT:
		if (attrRef == AttrRef::VAR_NAME) {
			convertedResult = pkb.getPrintAtStmtNum(resultInt);
		}
		else {
			convertedResult = result;
		}
		break;
	default: // EVERYTHING ELSE
		convertedResult = result;
		break;
	}
	return convertedResult;
}

std::string ResultProjector::convertSynonymResultToRequired(Type type, int result, AttrRef attrRef, PKB pkb) {
	std::string convertedResult;
	switch (type) {
	case Type::VARIABLE:
		convertedResult = pkb.getVarAtIdx(result);
		break;
	case Type::PROCEDURE:
		convertedResult = pkb.getProcAtIdx(result);
		break;
	case Type::CALL:
		if (attrRef == AttrRef::PROC_NAME) {
			convertedResult = pkb.getCallAtStmtNum(result);
		}
		else {
			convertedResult = to_string(result);
		}
		break;
	case Type::READ:
		if (attrRef == AttrRef::VAR_NAME) {
			convertedResult = pkb.getReadAtStmtNum(result);
		}
		else {
			convertedResult = to_string(result);
		}
		break;
	case Type::PRINT:
		if (attrRef == AttrRef::VAR_NAME) {
			convertedResult = pkb.getPrintAtStmtNum(result);
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

bool ResultProjector::combineResults(std::unordered_set<int> queryResultsOneSynonym, std::vector<std::string> synonyms) { // one synonym
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

bool ResultProjector::combineResults(std::unordered_map<int, std::unordered_set<int>> queryResultsTwoSynonyms, std::vector<std::string> synonyms) { // two synonyms
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

void ResultProjector::combineOneSynonym(std::unordered_set<int> queryResults, std::vector<std::string> synonyms) {
	std::string key = synonyms.at(0);

	if (synonymExists(key)) { // 1 common synonym
		filterOneSynInTable(key, queryResults);
	}
	else { // no common synonym
		addOneSyn(key, queryResults);
	}
}

void ResultProjector::combineTwoSynonyms(std::unordered_map<int, std::unordered_set<int>> queryResults, std::vector<std::string> synonyms) {
	std::string key1 = synonyms.at(0);
	std::string key2 = synonyms.at(1);
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
			std::unordered_map<int, std::unordered_set<int>> invertedQueryResults = invertResults(queryResults);
			mergeOneSyn(key2, key1, invertedQueryResults);
		}
	}
	else { // no common synonyms
		addTwoSyn(key1, key2, queryResults);
	}
}

bool ResultProjector::synonymExists(std::string synonym) {
	if (!synonymTable.empty()) {
		if (synonymTable.find(synonym) != synonymTable.end()) {
			return true;
		}
	}
	return false;
}

std::unordered_set<int> ResultProjector::getPossibleValues(std::string synonym) {
	std::unordered_set<int> possibleValues;

	if (synonymExists(synonym)) {
		int tableNum = synonymTable.at(synonym);
		std::list<std::unordered_map<std::string, int>> resultTable = synonymResults.at(tableNum);
		for (auto result : resultTable) {
			possibleValues.insert(result.at(synonym));
		}
	}
	return possibleValues;
}

void ResultProjector::addOneSyn(std::string key, std::unordered_set<int> results) {
	synonymTable[key] = index;

	std::list<std::unordered_map<std::string, int>> newSet;
	std::unordered_map<std::string, int> newSetEntry;
	for (const auto result : results) {
		newSetEntry[key] = result;
		newSet.push_back(newSetEntry);
	}

	synonymResults[index] = newSet;
	index++;
}

void ResultProjector::addTwoSyn(std::string key1, std::string key2, std::unordered_map<int, std::unordered_set<int>> results) {
	synonymTable[key1] = index;
	synonymTable[key2] = index;

	std::list<std::unordered_map<std::string, int>> newSet;
	std::unordered_map<std::string, int> newSetEntry;
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
void ResultProjector::filterOneSynInTable(std::string key, std::unordered_set<int> queryResults) {
	int tableNum = synonymTable[key];
	std::list<std::unordered_map<std::string, int>>& prevResults = synonymResults[tableNum];

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
void ResultProjector::filterTwoSynInSameTable(std::string key1, std::string key2, std::unordered_map<int, std::unordered_set<int>> queryResults) {
	int tableNum = synonymTable[key1];
	std::list<std::unordered_map<std::string, int>>& prevResults = synonymResults[tableNum];

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

void ResultProjector::mergeOneSyn(std::string existKey, std::string newKey, std::unordered_map<int, std::unordered_set<int>> queryResults) {
	int tableNum = synonymTable[existKey];
	std::list<std::unordered_map<std::string, int>>& prevResults = synonymResults[tableNum];

	for (auto itr = prevResults.begin(); itr != prevResults.end();) {
		int prevKey1Value = (*itr).at(existKey);

		if (existInMap(prevKey1Value, queryResults)) { // results overlap
			if (!existInMap(newKey, (*itr))) { // dependent result not added before (for when iterating to end of map when adding duplicated results)
				std::unordered_map<std::string, int> currRow = (*itr); // get a copy of current row result
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

void ResultProjector::mergeTables(std::string key1, std::string key2, std::unordered_map<int, std::unordered_set<int>> queryResults) {
	int key1InitialTableNum = synonymTable[key1];
	int key2InitialTableNum = synonymTable[key2];

	std::list<std::unordered_map<std::string, int>> key1PrevResults = synonymResults[key1InitialTableNum];
	std::list<std::unordered_map<std::string, int>> key2PrevResults = synonymResults[key2InitialTableNum];
	std::list<std::unordered_map<std::string, int>> newResultsSet;

	for (auto key1PrevResult : key1PrevResults) {
		int prevKey1Value = key1PrevResult[key1];
		if (existInMap(prevKey1Value, queryResults)) { // results overlap for key1
			for (auto key2PrevResult : key2PrevResults) {
				int prevKey2Value = key2PrevResult[key2];
				if (existInSet(prevKey2Value, queryResults[prevKey1Value])) { // results overlap for key2
					std::unordered_map<std::string, int> newResult = key1PrevResult; // merge 2 maps together
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
	std::unordered_map<std::string, int> synTable = synonymTable; // cannot just iterate using synonymTable, error when erasing
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

bool ResultProjector::existInMap(int key, std::unordered_map<int, std::unordered_set<int>> umap) {
	if (!umap.empty()) {
		if (umap.find(key) != umap.end()) {
			return true;
		}
	}
	return false;
}

bool ResultProjector::existInMap(std::string key, std::unordered_map<std::string, int> umap) {
	if (!umap.empty()) {
		if (umap.find(key) != umap.end()) {
			return true;
		}
	}
	return false;
}

bool ResultProjector::existInSet(int key, std::unordered_set<int> uset) {
	if (!uset.empty()) {
		if (uset.find(key) != uset.end()) {
			return true;
		}
	}
	return false;
}

void ResultProjector::cleanUpTables(std::string key) {
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
		std::unordered_map<std::string, int> synTable = synonymTable; // cannot just iterate using synonymTable, error when erasing
		for (auto synonym : synTable) {
			if (synonym.second == tableNum) {
				synonymTable.erase(synonym.first);
			}
		}
	}
}

std::unordered_map<int, std::unordered_set<int>> ResultProjector::invertResults(std::unordered_map<int, std::unordered_set<int>> queryResults) {
	std::unordered_map<int, std::unordered_set<int>> newResults;

	for (auto queryResult : queryResults) {
		int key = queryResult.first;
		std::unordered_set<int> values = queryResult.second;

		for (auto value : values) {
			newResults[value].insert(key);
		}
	}
	return newResults;
}

ResultCache* ResultProjector::getCacheType(Clause* clause) {
	ClauseType clauseType = clause->getClauseType();
	ResultCache* resultCache;

	switch (clauseType) {
	case ClauseType::AFFECTS:
		resultCache = &affectsCache;
		break;
	case ClauseType::AFFECTS_T:
		resultCache = &affectsTCache;
		break;
	case ClauseType::NEXT_T:
		resultCache = &nextTCache;
		break;
	}

	return resultCache;
}

bool ResultProjector::cacheExists(Clause* clause) {
	ResultCache* resultCache = getCacheType(clause);
	return resultCache->cacheExists(clause);
}

void ResultProjector::storeInCache(Clause* clause, std::unordered_set<int> queryResultsOneSynonym) {
	ResultCache* resultCache = getCacheType(clause);
	resultCache->storeInCache(clause, queryResultsOneSynonym);
}

void ResultProjector::storeInCache(Clause* clause, std::unordered_map<int, std::unordered_set<int>> queryResultsTwoSynonyms) {
	ResultCache* resultCache = getCacheType(clause);
	resultCache->storeInCache(clause, queryResultsTwoSynonyms);
}

bool ResultProjector::combineCacheResults(Clause* clause) {
	DesignEntity paraOne = clause->getParaOne();
	DesignEntity paraTwo = clause->getParaTwo();
	Type paraOneType = paraOne.getType();
	Type paraTwoType = paraTwo.getType();

	std::vector<std::string> synonyms;
	ResultCache* resultCache = getCacheType(clause);

	if (isStmtType(paraOneType) && isStmtType(paraTwoType)) {
		std::unordered_map<int, std::unordered_set<int>> cacheResult = resultCache->getTwoSynCacheResult();
		synonyms.push_back(paraOne.getValue());
		synonyms.push_back(paraTwo.getValue());

		return combineResults(cacheResult, synonyms);
	}
	else {
		if (isStmtType(paraOneType)) {
			synonyms.push_back(paraOne.getValue());
		}
		else if (isStmtType(paraTwoType)) {
			synonyms.push_back(paraTwo.getValue());
		}
		std::unordered_set<int> cacheResult = resultCache->getOneSynCacheResult();
		return combineResults(cacheResult, synonyms);
	}
}

bool ResultProjector::isStmtType(Type type) {
	return  type == Type::STATEMENT || type == Type::PROGLINE || type == Type::READ || type == Type::PRINT || type == Type::CALL ||
		type == Type::WHILE || type == Type::IF || type == Type::ASSIGN || type == Type::SWITCH;
}

// For testing purposes
std::unordered_map<std::string, int> ResultProjector::getSynonymTable() {
	return synonymTable;
}

void ResultProjector::setSynonymTable(std::unordered_map<std::string, int> synTable) {
	synonymTable = synTable;
}

std::unordered_map<int, std::list<std::unordered_map<std::string, int>>> ResultProjector::getSynonymResults() {
	return synonymResults;
}

void ResultProjector::setSynonymResults(std::unordered_map<int, std::list<std::unordered_map<std::string, int>>> synResults) {
	synonymResults = synResults;
}

ResultCache ResultProjector::getAffectsCache() {
	return affectsCache;
}

ResultCache ResultProjector::getAffectsTCache() {
	return affectsTCache;
}

ResultCache ResultProjector::getNextTCache() {
	return nextTCache;
}
