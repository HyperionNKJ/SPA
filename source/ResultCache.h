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


public:
	void resetCache();
	bool cacheExists(Clause* clause);
	unordered_map<int, unordered_set<int>> getTwoSynCacheResult();
	unordered_set<int> getOneSynCacheResult();
	void storeInCache(Clause* clause, unordered_map<int, unordered_set<int>>& result); // Two Synonyms
	void storeInCache(Clause* clause, unordered_set<int>& result); // One Synonym
};