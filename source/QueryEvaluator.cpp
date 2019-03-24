#include "QueryEvaluator.h"
#include "ResultProjector.h"
#include "Clause.h"
#include <unordered_set>
#include <map>

list<string> QueryEvaluator::evaluate(ProcessedQuery& processedQuery, const PKB& pkb) {
	ResultProjector resultProjector;
	resultProjector.resetResults(); // Reset possible old query result
	list<string> emptyResult;

	unordered_set<Clause*> withClauses = processedQuery.withClauses;
	unordered_set<Clause*> suchThatPatternClauses = processedQuery.clauses;
	vector<Clause*> sortedClauses = optimizationSort(suchThatPatternClauses); 
	vector<Clause*> combinedClauses = { withClauses.begin(), withClauses.end() };
	combinedClauses.insert(combinedClauses.end(), sortedClauses.begin(), sortedClauses.end());

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



// Function that encapsulates all optimization logic
vector<Clause*> QueryEvaluator::optimizationSort(const unordered_set<Clause*>& suchThatPatternClauses) {
	map<unordered_set<string>, vector<Clause*>, CompareBySize> intermediateClauses; // key = common synonyms, value = clauses with those synonyms
	unordered_set<Clause*> noSynClauses; // true/fase clause without any synonym

	groupBasedOnConnectedSyn(suchThatPatternClauses, intermediateClauses, noSynClauses); // group clauses base on common synonyms
	sortBasedOnNumOfSyn(intermediateClauses); // sort clauses within each group
	return combineClauseGroups(noSynClauses, intermediateClauses);
}

void QueryEvaluator::groupBasedOnConnectedSyn(const unordered_set<Clause*>& unorderedClauses, map<unordered_set<string>, vector<Clause*>, CompareBySize>& connectedClauses, unordered_set<Clause*>& noSynClauses) {
	for (const auto& clause : unorderedClauses) {
		unordered_set<string> synonyms = clause->getSynonyms();
		if (synonyms.empty()) {
			noSynClauses.insert(clause);
			continue;
		}
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

void QueryEvaluator::addIntoGroup(map<unordered_set<string>, vector<Clause*>, CompareBySize>& connectedClauses, map<unordered_set<string>, vector<Clause*>, CompareBySize>::iterator& itr, unordered_set<string>& synonyms, Clause* clause) {
	unordered_set<string> synonymGroup = itr->first;
	vector<Clause*> clauseGroup = itr->second;
	connectedClauses.erase(itr);
	synonymGroup.insert(synonyms.begin(), synonyms.end());
	clauseGroup.push_back(clause);
	connectedClauses.insert({ synonymGroup, clauseGroup });
}

bool compareByNumOfSyn(Clause* clause1, Clause* clause2) {
	return clause1->getSynonyms().size() < clause2->getSynonyms().size();
}

void QueryEvaluator::sortBasedOnNumOfSyn(map<unordered_set<string>, vector<Clause*>, CompareBySize>& unsortedClauses) {
	for (const auto& synClausePair : unsortedClauses) {
		vector<Clause*> clauses = synClausePair.second;
		sort(clauses.begin(), clauses.end(), compareByNumOfSyn);
	}
}

vector<Clause*> QueryEvaluator::combineClauseGroups(const unordered_set<Clause*>& noSynClauses, const map<unordered_set<string>, vector<Clause*>, CompareBySize>& partitionedClauses) {
	vector<Clause*> combinedClause;
	combinedClause.insert(combinedClause.begin(), noSynClauses.begin(), noSynClauses.end()); // add clauses without synonym first

	for (const auto& synClausePair : partitionedClauses) {
		combinedClause.insert(combinedClause.end(), synClausePair.second.begin(), synClausePair.second.end());
	}
	return combinedClause;
}

bool QueryEvaluator::hasCommonSynonyms(const unordered_set<string>& synonymSet1, const unordered_set<string>& synonymSet2) {
	for (const auto& syn : synonymSet1) {
		if (synonymSet2.count(syn)) {
			return true;
		}
	}
	return false;
}
