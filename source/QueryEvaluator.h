#pragma once

#include <list>
#include <map>
#include "ProcessedQuery.h"
#include "ResultProjector.h"

struct CompareBySize {
	bool operator()(const std::unordered_set<string>& clause1, const std::unordered_set<string>& clause2) const {
		return clause1.size() > clause2.size();
	}
};

class QueryEvaluator {
private:
	void findReducedDomain(Clause*, ResultProjector*);

	std::vector<Clause*> optimizationSort(const std::unordered_set<Clause*>&);
	void groupBasedOnConnectedSyn(const std::unordered_set<Clause*>&, std::map<std::unordered_set<string>, std::vector<Clause*>, CompareBySize>&, std::unordered_set<Clause*>&);
	void sortBasedOnNumOfSyn(std::map<std::unordered_set<string>, std::vector<Clause*>, CompareBySize>&);
	std::vector<Clause*> combineClauseGroups(std::unordered_set<Clause*>&, std::map<std::unordered_set<string>, std::vector<Clause*>, CompareBySize>&);
	bool hasCommonSynonyms(const std::unordered_set<string>&, const std::unordered_set<string>&);
	void addIntoGroup(std::map<std::unordered_set<string>, std::vector<Clause*>, CompareBySize>&, std::map<std::unordered_set<string>, std::vector<Clause*>, CompareBySize>::iterator, std::unordered_set<string>, Clause*);

public:
	std::list<std::string> evaluate(ProcessedQuery&, const PKB&);
};
