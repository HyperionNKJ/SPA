#pragma once

#include "DesignEntity.h"
#include "PKB.h"
#include <stdio.h>
#include <iostream>
#include <unordered_set>
#include <unordered_map> 

class ResultProjector {
private:
	void combineOneSynonym(unordered_set<int> queryResults, vector<string> synonyms);
	void combineTwoSynonyms(unordered_map<int, unordered_set<int>> queryResults, vector<string> synonyms);
	bool synonymExists(string);
	void addOneSyn(string key, unordered_set<int> results);
	void addTwoSyn(string key1, string key2, unordered_map<int, unordered_set<int>> results);
	void filterOneSynInTable(string key, unordered_set<int> queryResults);
	void filterTwoSynInSameTable(string key1, string key2, unordered_map<int, unordered_set<int>> queryResults);
	void mergeOneSyn(string existKey, string newKey, unordered_map<int, unordered_set<int>> queryResults);
	void mergeTables(string key1, string key2, unordered_map<int, unordered_set<int>> queryResults);
	bool existInMap(int key, unordered_map<int, unordered_set<int>> umap);
	bool existInMap(string key, unordered_map<string, int> umap);
	bool existInSet(int key, unordered_set<int> uset);
	void cleanUpTables(string);
	unordered_map<int, unordered_set<int>> invertResults(unordered_map<int, unordered_set<int>> queryResults);

	list<string> getSelectedClauseInTable(Type, list<int>, PKB);
	list<string> getSelectedClauseNotInTable(Type, PKB);
	list<string> convertVarIndexToVar(unordered_set<int>, PKB);
	list<string> convertVarIndexToVar(list<int>, PKB);
	list<string> convertProcIndexToProc(unordered_set<int>, PKB);
	list<string> convertProcIndexToProc(list<int>, PKB);
	list<string> convertIndexToString(unordered_set<int>, PKB);
	list<string> convertIndexToString(list<int>, PKB);
	list<string> convertSetToList(unordered_set<string>);

public:
	void resetResults();
	bool ResultProjector::combineResults(unordered_set<int> queryResultsOneSynonym, vector<string> synonyms);
	bool ResultProjector::combineResults(unordered_map<int, unordered_set<int>> queryResultsTwoSynonyms, vector<string> synonyms);
	list<string> getResults(DesignEntity selectedSynonym, PKB pkb);

	void printTables(); // for debugging
	unordered_map<string, int> getSynonymTable(); // for testing
	unordered_map<int, list<unordered_map<string, int>>> ResultProjector::getSynonymResults(); // for testing

};
