#pragma once

#include <list>
#include <map>
#include "ProcessedQuery.h"
#include "ResultProjector.h"

using namespace std;

class QueryEvaluator {
private:
	bool findReducedDomain(Clause*, ResultProjector*);

public:
	vector<Clause*> optimizationSort(vector<Clause*>&);
	void groupBasedOnConnectedSyn(vector<Clause*>&, vector<pair<unordered_set<string>, vector<Clause*>>>&);
	bool hasCommonSynonyms(const unordered_set<string>&, const unordered_set<string>&);
	void sortWithinEachGroup(vector<pair<unordered_set<string>, vector<Clause*>>>&);
	vector<Clause*> combineClauses(vector<pair<unordered_set<string>, vector<Clause*>>>&);

	list<string> evaluate(ProcessedQuery&, const PKB&);
};
