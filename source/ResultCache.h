#pragma once

#include "Clause.h"
#include "CacheType.h"
#include <stdio.h>
#include <iostream>
#include <string>
#include <unordered_set>
#include <unordered_map> 

class ResultCache {
private:
	unordered_map<CacheType, unordered_map<int, unordered_set<int>>> SynSynSame; // Affects(a, a)
	unordered_map<CacheType, unordered_map<int, unordered_set<int>>> SynSynDiff; // Affects(a, a1)
	unordered_map<Type, unordered_set<int>> SynUnderscore; // Affects(a, _)
	unordered_map<Type, unordered_set<int>> UnderscoreSyn; // Affects(_, a)
	unordered_map<Type, unordered_map<int, unordered_set<int>>> SynFixed; // Affects(a, 1)
	unordered_map<Type, unordered_map<int, unordered_set<int>>> FixedSyn; // Affects(1, a)

	unordered_map<int, unordered_set<int>> twoSynCacheResult;
	unordered_set<int> oneSynCacheResult;

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
	void resetCache();
	bool cacheExists(Clause* clause);
	unordered_map<int, unordered_set<int>> getTwoSynCacheResult();
	unordered_set<int> getOneSynCacheResult();
	void storeInCache(Clause* clause, unordered_map<int, unordered_set<int>>& result); // Two Synonyms
	void storeInCache(Clause* clause, unordered_set<int>& result); // One Synonym

	// for testing
	bool isEquals(ResultCache other);
	void ResultCache::setSynSynSame(unordered_map<CacheType, unordered_map<int, unordered_set<int>>> result);
	void ResultCache::setSynSynDiff(unordered_map<CacheType, unordered_map<int, unordered_set<int>>> result);
	void ResultCache::setSynUnderscore(unordered_map<Type, unordered_set<int>> result);
	void ResultCache::setUnderscoreSyn(unordered_map<Type, unordered_set<int>> result);
	void ResultCache::setSynFixed(unordered_map<Type, unordered_map<int, unordered_set<int>>> result);
	void ResultCache::setFixedSyn(unordered_map<Type, unordered_map<int, unordered_set<int>>> result);
	unordered_map<CacheType, unordered_map<int, unordered_set<int>>> getSynSynSame();
	unordered_map<CacheType, unordered_map<int, unordered_set<int>>> getSynSynDiff();
	unordered_map<Type, unordered_set<int>> getSynUnderscore();
	unordered_map<Type, unordered_set<int>> getUnderscoreSyn();
	unordered_map<Type, unordered_map<int, unordered_set<int>>> getSynFixed();
	unordered_map<Type, unordered_map<int, unordered_set<int>>> getFixedSyn();
};