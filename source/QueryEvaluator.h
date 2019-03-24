#pragma once

#include <list>
#include <map>
#include "ProcessedQuery.h"
#include "ResultProjector.h"

class QueryEvaluator {
private:
	void findReducedDomain(Clause*, ResultProjector*);
	std::vector<Clause*> optimizationSort(const std::unordered_set<Clause*>&);

	// to test
	struct CompareBySize {
		bool operator()(const std::unordered_set<Clause*>& clause1, const std::unordered_set<Clause*>& clause2) const {
			return clause1.size() < clause2.size();
		}
	};

	bool compareByNumOfSyn(Clause* clause1, Clause* clause2) {
		return clause1->getSynonyms().size() > clause2->getSynonyms().size();
	}

	void groupBasedOnConnectedSyn(const std::unordered_set<Clause*>&, std::map<std::unordered_set<string>, std::vector<Clause*>, CompareBySize>&);
	void sortBasedOnNumOfSyn(std::map<std::unordered_set<string>, std::vector<Clause*>, CompareBySize>&);
	std::vector<Clause*> combineClauseGroups(std::map<std::unordered_set<string>, std::vector<Clause*>, CompareBySize>&);
	bool hasCommonSynonyms(const std::unordered_set<string>&, const std::unordered_set<string>&);
	void addIntoGroup(std::map<std::unordered_set<string>, std::vector<Clause*>, CompareBySize>&, std::map<std::unordered_set<string>, std::vector<Clause*>, CompareBySize>::iterator, std::unordered_set<string>, Clause*);

public:
	std::list<std::string> evaluate(ProcessedQuery&, const PKB&);
};
