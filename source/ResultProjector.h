#pragma once

#include "DesignEntity.h"
#include "AttrRef.h"
#include "PKB.h"
#include "ResultCache.h"
#include <stdio.h>
#include <iostream>
#include <string>
#include <unordered_set>
#include <unordered_map> 

class ResultProjector {
private:
	void combineOneSynonym(unordered_set<int> queryResults, vector<string> synonyms);
	void combineTwoSynonyms(unordered_map<int, unordered_set<int>> queryResults, vector<string> synonyms);
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

	string convertSynonymResultToRequired(Type, int, AttrRef, PKB);
	string convertSynonymResultToRequired(Type, string, AttrRef, PKB);
	unordered_set<string> getAllResults(Type, PKB);
	unordered_set<string> convertSetToString(unordered_set<int>);

public:
	void resetResults();
	bool combineResults(unordered_set<int> queryResultsOneSynonym, vector<string> synonyms);
	bool combineResults(unordered_map<int, unordered_set<int>> queryResultsTwoSynonyms, vector<string> synonyms);
	list<string> getResults(vector<DesignEntity> selectedSynonyms, PKB pkb);
	bool synonymExists(string synonym);
	unordered_set<int> getPossibleValues(string synonym);

	ResultCache* getCacheType(Clause* clause);
	bool cacheExists(Clause* clause);
	void storeInCache(Clause* clause, unordered_set<int> queryResultsOneSynonym);
	void storeInCache(Clause* clause, unordered_map<int, unordered_set<int>> queryResultsTwoSynonyms);
	bool isStmtType(Type type);
	bool combineCacheResults(Clause* clause);

	// for testing
	unordered_map<string, int> getSynonymTable(); 
	void setSynonymTable(unordered_map<string, int> synTable);
	unordered_map<int, list<unordered_map<string, int>>> getSynonymResults();
	void setSynonymResults(unordered_map<int, list<unordered_map<string, int>>> synResults);
	ResultCache getAffectsCache();
	ResultCache getAffectsTCache();
	ResultCache getNextTCache();

	// for debugging
	void printTables();
};
