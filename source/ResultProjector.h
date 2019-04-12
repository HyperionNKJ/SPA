#pragma once

#include "DesignEntity.h"
#include "AttrRef.h"
#include "PKB.h"
#include "ResultCache.h"

class ResultProjector {
private:
	// combine results
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

	// select
	string convertSynonymResultToRequired(Type, int, AttrRef, PKB);
	string convertSynonymResultToRequired(Type, string, AttrRef, PKB);
	unordered_set<string> getAllResults(Type, PKB);
	unordered_set<string> convertSetToString(unordered_set<int>);

	// caching
	bool isStmtType(Type type);

public:
	void resetResults(); // reset results after each query
	bool combineResults(unordered_set<int> queryResultsOneSynonym, vector<string> synonyms); // combine results into intermediate table for single synonym clause
	bool combineResults(unordered_map<int, unordered_set<int>> queryResultsTwoSynonyms, vector<string> synonyms); // combine results into intermediate table for double synonyms clause
	list<string> getResults(vector<DesignEntity> selectedSynonyms, PKB pkb); // get selected results
	bool synonymExists(string synonym); // checks if synonym exists in the intermediate table
	unordered_set<int> getPossibleValues(string synonym); // get all values of a synonym in the intermediate table

	ResultCache* getCacheType(Clause* clause); // get CacheType for the clause
	bool cacheExists(Clause* clause); // checks if cached result exists for the clause
	void storeInCache(Clause* clause, unordered_set<int> queryResultsOneSynonym); // store result in cache for single synonym clause
	void storeInCache(Clause* clause, unordered_map<int, unordered_set<int>> queryResultsTwoSynonyms); // store result in cache for double synonyms clause
	bool combineCacheResults(Clause* clause); // get results from cache and combine cached results into intermediate table

	// for testing (getter & setter)
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
