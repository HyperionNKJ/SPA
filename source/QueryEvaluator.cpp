#include "QueryEvaluator.h"
#include "ResultProjector.h"
#include "Clause.h"
#include <unordered_set>
#include <map>

std::list<std::string> QueryEvaluator::evaluate(ProcessedQuery& processedQuery, const PKB& pkb) {
	ResultProjector resultProjector;
	resultProjector.resetResults(); // Reset possible old query result
	std::list<std::string> emptyResult;

	std::unordered_set<Clause*> withClauses = processedQuery.withClauses;
	std::unordered_set<Clause*> suchThatPatternClauses = processedQuery.clauses;
	// std::vector<Clause*> sortedClauses = optimizationSort(suchThatPatternClauses); 
	std::vector<Clause*> combinedClauses = { withClauses.begin(), withClauses.end() };

	combinedClauses.insert(combinedClauses.end(), suchThatPatternClauses.begin(), suchThatPatternClauses.end() );
	//combinedClauses.insert(combinedClauses.end(), sortedClauses.begin(), sortedClauses.end());

	for (auto clause : combinedClauses) {
		if (clause->getClauseType() == NEXT_T) {
			findReducedDomain(clause, &resultProjector); // reduce domain based on resultProjector's intermediate table
		}

		Result clauseResult = clause->evaluate(pkb);
		int numOfSyn = clauseResult.getNumOfSyn();
		if (clauseResult.hasPassed() && numOfSyn != 0) { // if clause has synonym/s, send to ResultProjector to merge.
			bool hasResultSoFar;
			vector<string> synonyms = clauseResult.getSynonyms();
			if (numOfSyn == 1) {
				hasResultSoFar = resultProjector.combineResults(clauseResult.getOneSynAnswer(), synonyms);
			}
			else if (numOfSyn == 2) { 
				hasResultSoFar = resultProjector.combineResults(clauseResult.getTwoSynAnswer(), synonyms);
			}
			if (!hasResultSoFar) {
				return emptyResult; 
			}
		}
		else if (!clauseResult.hasPassed()) {
			return emptyResult;
		}
	}
	return resultProjector.getResults(processedQuery.resultClElemList, pkb);
}

void QueryEvaluator::findReducedDomain(Clause* clause, ResultProjector* resultProjector) {
	unordered_set<string> synonyms = clause->getSynonyms(); // 0, 1, or 2 synonyms
	unordered_map<string, unordered_set<int>> reducedDomain;
	for (auto s : synonyms) {
		if (resultProjector->synonymExists(s)) {
			reducedDomain.insert({ s, resultProjector->getPossibleValues(s) });
		}
	}
	clause->setReducedDomain(reducedDomain);
}

/*
// to test
struct CompareBySize {
	bool operator()(const std::unordered_set<Clause*>& clause1, const std::unordered_set<Clause*>& clause2) const {
		return clause1.size() > clause2.size();
	}
};

// Function that encapsulates all optimization logic
std::vector<Clause*> QueryEvaluator::optimizationSort(const std::unordered_set<Clause*>& suchThatPatternClauses) {
	std::map<std::unordered_set<string>, std::vector<Clause*>, CompareBySize> intermediateClauses; // key = common synonyms, value = clauses with those synonyms

	groupBasedOnConnectedSyn(suchThatPatternClauses, intermediateClauses); // group clauses base on common synonyms
	sortBasedOnNumOfSyn(intermediateClauses); // sort clauses within each group
	return combineClauseGroups(intermediateClauses);
}

void QueryEvaluator::groupBasedOnConnectedSyn(const std::unordered_set<Clause*>& unorderedClauses, std::map<std::unordered_set<string>, std::vector<Clause*>, CompareBySize>& connectedClauses) {
	for (const auto& clause : unorderedClauses) {
		std::unordered_set<string> synonyms = clause->getSynonyms();
		bool addedIntoGroup = false;
		for (auto& itr = connectedClauses.begin(); itr != connectedClauses.end(); ++itr) {
			if (hasCommonSynonyms(synonyms, itr->first)) {
				addIntoGroup(connectedClauses, itr, synonyms, clause);
				addedIntoGroup = true;
			}
		}
		if (!addedIntoGroup) {
			connectedClauses.insert({ synonyms, {clause} });
		}
	}
}

void QueryEvaluator::addIntoGroup(std::map<std::unordered_set<string>, std::vector<Clause*>, CompareBySize>& connectedClauses, std::map<std::unordered_set<string>, std::vector<Clause*>, CompareBySize>::iterator itr, std::unordered_set<string> synonyms, Clause* clause) {
	std::unordered_set<string> synonymGroup = itr->first;
	std::vector<Clause*> clauseGroup = itr->second;
	connectedClauses.erase(itr);
	synonymGroup.insert(synonyms.begin(), synonyms.end());
	clauseGroup.push_back(clause);
	connectedClauses.insert({ synonymGroup, clauseGroup });
}

bool compareByNumOfSyn(Clause* clause1, Clause* clause2) {
	return clause1->getSynonyms().size() < clause2->getSynonyms().size();
}

void QueryEvaluator::sortBasedOnNumOfSyn(std::map<std::unordered_set<string>, std::vector<Clause*>, CompareBySize>& unsortedClauses) {
	for (const auto& synClausePair : unsortedClauses) {
		std::vector<Clause*> clauses = synClausePair.second;
		std::sort(clauses.begin(), clauses.end(), compareByNumOfSyn);
	}
}

std::vector<Clause*> QueryEvaluator::combineClauseGroups(std::map<std::unordered_set<string>, std::vector<Clause*>, CompareBySize>& partitionedClauses) {
	std::vector<Clause*> combinedClause;
	for (const auto& synClausePair : partitionedClauses) {
		combinedClause.insert(combinedClause.end(), synClausePair.second.begin(), synClausePair.second.end());
	}
	return combinedClause;
}

bool QueryEvaluator::hasCommonSynonyms(const std::unordered_set<string>& synonymSet1, const std::unordered_set<string>& synonymSet2) {
	for (const auto& syn : synonymSet1) {
		if (synonymSet2.count(syn)) {
			return true;
		}
	}
	return false;
}
*/