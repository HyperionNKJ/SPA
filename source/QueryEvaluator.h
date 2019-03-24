#pragma once

#include <list>
#include <map>
#include "ProcessedQuery.h"
#include "ResultProjector.h"

using namespace std;

struct CompareBySize {
	bool operator()(const unordered_set<string>& clause1, const unordered_set<string>& clause2) const {
		return clause1.size() > clause2.size();
	}
};

class QueryEvaluator {
private:
	void findReducedDomain(Clause*, ResultProjector*);

	vector<Clause*> optimizationSort(const unordered_set<Clause*>&);
	void groupBasedOnConnectedSyn(const unordered_set<Clause*>&, map<unordered_set<string>, vector<Clause*>, CompareBySize>&, unordered_set<Clause*>&);
	void sortBasedOnNumOfSyn(map<unordered_set<string>, vector<Clause*>, CompareBySize>&);
	vector<Clause*> combineClauseGroups(const unordered_set<Clause*>&, const map<unordered_set<string>,  vector<Clause*>, CompareBySize>&);
	bool hasCommonSynonyms(const unordered_set<string>&, const unordered_set<string>&);
	void addIntoGroup(map<unordered_set<string>, vector<Clause*>, CompareBySize>&, map<unordered_set<string>, vector<Clause*>, CompareBySize>::iterator&, unordered_set<string>&, Clause*);

public:
	list<string> evaluate(ProcessedQuery&, const PKB&);
};
