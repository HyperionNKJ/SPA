#pragma once

#include "ProcessedQuery.h"
#include "ResultProjector.h"

class QueryEvaluator {
private:
	bool findReducedDomain(Clause*, ResultProjector*);
	std::list<std::string> returnNegativeResult(std::vector<DesignEntity>&);

public:
	std::vector<Clause*> optimizationSort(std::vector<Clause*>&);
	void sortBooleanClauses(const std::vector<Clause*>&, std::vector<Clause*>&, std::vector<Clause*>&);
	void groupBasedOnConnectedSyn(std::vector<Clause*>&, std::vector<std::pair<std::unordered_set<std::string>, std::vector<Clause*>>>&);
	bool hasCommonSynonyms(const std::unordered_set<std::string>&, const std::unordered_set<std::string>&);
	void sortWithinEachGroup(std::vector<std::pair<std::unordered_set<std::string>, std::vector<Clause*>>>&);
	std::vector<Clause*> combineClauses(std::vector<std::pair<std::unordered_set<std::string>, std::vector<Clause*>>>&);

	std::list<std::string> evaluate(ProcessedQuery&, const PKB&);
};
