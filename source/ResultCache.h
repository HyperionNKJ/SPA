#pragma once

#include "Clause.h"
#include "CacheType.h"
#include <stdio.h>
#include <iostream>
#include <string>
#include <unordered_set>
#include <unordered_map> 

/* Cache results for Next*, Affects and Affects* when the clause contains at least one synonym */

class ResultCache {
private:
	std::unordered_map<CacheType, std::unordered_map<int, std::unordered_set<int>>> SynSynSame; // Affects(a, a)
	std::unordered_map<CacheType, std::unordered_map<int, std::unordered_set<int>>> SynSynDiff; // Affects(a, a1)
	std::unordered_map<Type, std::unordered_set<int>> SynUnderscore; // Affects(a, _)
	std::unordered_map<Type, std::unordered_set<int>> UnderscoreSyn; // Affects(_, a)
	std::unordered_map<Type, std::unordered_map<int, std::unordered_set<int>>> SynFixed; // Affects(a, 1)
	std::unordered_map<Type, std::unordered_map<int, std::unordered_set<int>>> FixedSyn; // Affects(1, a)

	std::unordered_map<int, std::unordered_set<int>> twoSynCacheResult;
	std::unordered_set<int> oneSynCacheResult;

	bool isStmtType(Type type);
	CacheType convertToCacheType(Type type1, Type type2);
	CacheType startWithStatement(Type type2);
	CacheType startWithProgline(Type type2);
	CacheType startWithRead(Type type2);
	CacheType startWithPrint(Type type2);
	CacheType startWithCall(Type type2);
	CacheType startWithWhile(Type type2);
	CacheType startWithIf(Type type2);
	CacheType startWithAssign(Type type2);
	CacheType startWithSwitch(Type type2);


public:
	void resetCache(); // reset cache after each query
	bool cacheExists(Clause* clause); // checks if cached result exists for the clause
	std::unordered_set<int> getOneSynCacheResult(); // get cached result for single synonym (cached result is set after cacheExists() is called and is true)
	std::unordered_map<int, std::unordered_set<int>> getTwoSynCacheResult(); // get cached result for double synonyms (cached result is set after cacheExists() is called and is true)
	void storeInCache(Clause* clause, std::unordered_set<int>& result); // store result in cache for single synonym clause
	void storeInCache(Clause* clause, std::unordered_map<int, std::unordered_set<int>>& result); // store result in cache for double synonyms clause

	// for testing (getter & setter)
	bool isEquals(ResultCache other);
	void ResultCache::setSynSynSame(std::unordered_map<CacheType, std::unordered_map<int, std::unordered_set<int>>> result);
	void ResultCache::setSynSynDiff(std::unordered_map<CacheType, std::unordered_map<int, std::unordered_set<int>>> result);
	void ResultCache::setSynUnderscore(std::unordered_map<Type, std::unordered_set<int>> result);
	void ResultCache::setUnderscoreSyn(std::unordered_map<Type, std::unordered_set<int>> result);
	void ResultCache::setSynFixed(std::unordered_map<Type, std::unordered_map<int, std::unordered_set<int>>> result);
	void ResultCache::setFixedSyn(std::unordered_map<Type, std::unordered_map<int, std::unordered_set<int>>> result);
	std::unordered_map<CacheType, std::unordered_map<int, std::unordered_set<int>>> getSynSynSame();
	std::unordered_map<CacheType, std::unordered_map<int, std::unordered_set<int>>> getSynSynDiff();
	std::unordered_map<Type, std::unordered_set<int>> getSynUnderscore();
	std::unordered_map<Type, std::unordered_set<int>> getUnderscoreSyn();
	std::unordered_map<Type, std::unordered_map<int, std::unordered_set<int>>> getSynFixed();
	std::unordered_map<Type, std::unordered_map<int, std::unordered_set<int>>> getFixedSyn();
};