#pragma once

#include "DesignEntity.h"
#include "AttrRef.h"
#include "PKB.h"
#include "ResultCache.h"

class ResultProjector {
private:
	// combine results
	void combineOneSynonym(std::unordered_set<int> queryResults, std::vector<std::string> synonyms);
	void combineTwoSynonyms(std::unordered_map<int, std::unordered_set<int>> queryResults, std::vector<std::string> synonyms);
	void addOneSyn(std::string key, std::unordered_set<int> results);
	void addTwoSyn(std::string key1, std::string key2, std::unordered_map<int, std::unordered_set<int>> results);
	void filterOneSynInTable(std::string key, std::unordered_set<int> queryResults);
	void filterTwoSynInSameTable(std::string key1, std::string key2, std::unordered_map<int, std::unordered_set<int>> queryResults);
	void mergeOneSyn(std::string existKey, std::string newKey, std::unordered_map<int, std::unordered_set<int>> queryResults);
	void mergeTables(std::string key1, std::string key2, std::unordered_map<int, std::unordered_set<int>> queryResults);
	bool existInMap(int key, std::unordered_map<int, std::unordered_set<int>> umap);
	bool existInMap(std::string key, std::unordered_map<std::string, int> umap);
	bool existInSet(int key, std::unordered_set<int> uset);
	void cleanUpTables(std::string);
	std::unordered_map<int, std::unordered_set<int>> invertResults(std::unordered_map<int, std::unordered_set<int>> queryResults);

	// select
	std::string convertSynonymResultToRequired(Type, int, AttrRef, PKB);
	std::string convertSynonymResultToRequired(Type, std::string, AttrRef, PKB);
	std::unordered_set<std::string> getAllResults(Type, PKB);
	std::unordered_set<std::string> convertSetToString(std::unordered_set<int>);

	// caching
	bool isStmtType(Type type);

public:
	void resetResults(); // reset results after each query
	bool combineResults(std::unordered_set<int> queryResultsOneSynonym, std::vector<std::string> synonyms); // combine results into intermediate table for single synonym clause
	bool combineResults(std::unordered_map<int, std::unordered_set<int>> queryResultsTwoSynonyms, std::vector<std::string> synonyms); // combine results into intermediate table for double synonyms clause
	std::list<std::string> getResults(std::vector<DesignEntity> selectedSynonyms, PKB pkb); // get selected results
	bool synonymExists(std::string synonym); // checks if synonym exists in the intermediate table
	std::unordered_set<int> getPossibleValues(std::string synonym); // get all values of a synonym in the intermediate table

	ResultCache* getCacheType(Clause* clause); // get CacheType for the clause
	bool cacheExists(Clause* clause); // checks if cached result exists for the clause
	void storeInCache(Clause* clause, std::unordered_set<int> queryResultsOneSynonym); // store result in cache for single synonym clause
	void storeInCache(Clause* clause, std::unordered_map<int, std::unordered_set<int>> queryResultsTwoSynonyms); // store result in cache for double synonyms clause
	bool combineCacheResults(Clause* clause); // get results from cache and combine cached results into intermediate table

	// for testing (getter & setter)
	std::unordered_map<std::string, int> getSynonymTable();
	void setSynonymTable(std::unordered_map<std::string, int> synTable);
	std::unordered_map<int, std::list<std::unordered_map<std::string, int>>> getSynonymResults();
	void setSynonymResults(std::unordered_map<int, std::list<std::unordered_map<std::string, int>>> synResults);
	ResultCache getAffectsCache();
	ResultCache getAffectsTCache();
	ResultCache getNextTCache();
};
