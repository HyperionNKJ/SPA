#pragma once

#include <list>
#include <map>
#include "ProcessedQuery.h"
#include "ResultProjector.h"

using namespace std;

class QueryEvaluator {
private:
	void findReducedDomain(Clause*, ResultProjector*);
	vector<Clause*> optimizationSort(vector<Clause*>&);




	bool hasCommonSynonyms(const unordered_set<string>&, const unordered_set<string>&);

	//void sortBasedOnNumOfSyn(map<unordered_set<string>, vector<Clause*>, CompareBySize>&);
	//vector<Clause*> combineClauseGroups(const unordered_set<Clause*>&, const map<unordered_set<string>,  vector<Clause*>, CompareBySize>&);
public:
	void groupBasedOnConnectedSyn(vector<Clause*>&, vector<pair<unordered_set<string>, vector<Clause*>>>&);
	list<string> evaluate(ProcessedQuery&, const PKB&);
};
