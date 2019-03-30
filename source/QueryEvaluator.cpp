#include "QueryEvaluator.h"
#include "ResultProjector.h"
#include "Clause.h"
#include <unordered_set>
#include <map>

list<string> QueryEvaluator::evaluate(ProcessedQuery& processedQuery, const PKB& pkb) {
	ResultProjector resultProjector;
	resultProjector.resetResults(); // Reset possible old query result
	list<string> emptyResult;

	vector<Clause*>& booleanClauses = processedQuery.booleanClauses; // boolean clause refers to clause without synonym.
	vector<Clause*>& sortedWithClauses = optimizationSort(processedQuery.withCLauses); // all types of clauses below have synonyms. Hence optimization is required
	vector<Clause*>& sortedAffectsClauses = optimizationSort(processedQuery.affectsClauses);
	vector<Clause*>& sortedAffectsTClauses = optimizationSort(processedQuery.affectsTClauses);
	vector<Clause*>& sortedNextTClauses = optimizationSort(processedQuery.nextTClauses);
	vector<Clause*>& sortedOtherClauses = optimizationSort(processedQuery.otherClauses);

	vector<Clause*> combinedClauses; // optimally combined with the following order
	combinedClauses.insert(combinedClauses.end(), booleanClauses.begin(), booleanClauses.end());
	combinedClauses.insert(combinedClauses.end(), sortedWithClauses.begin(), sortedWithClauses.end());
	combinedClauses.insert(combinedClauses.end(), sortedOtherClauses.begin(), sortedOtherClauses.end());
	combinedClauses.insert(combinedClauses.end(), sortedAffectsClauses.begin(), sortedAffectsClauses.end());
	combinedClauses.insert(combinedClauses.end(), sortedAffectsTClauses.begin(), sortedAffectsTClauses.end());
	combinedClauses.insert(combinedClauses.end(), sortedNextTClauses.begin(), sortedNextTClauses.end());

	for (auto clause : combinedClauses) {
		if (clause->getClauseType() == NEXT_T) {
			findReducedDomain(clause, &resultProjector); // reduce domain based on resultProjector's intermediate table
		}

		Result clauseResult = clause->evaluate(pkb);
		int numOfSyn = clauseResult.getNumOfSyn(); // num of synonyms in clauseResult is zero if clause is false
		if (clauseResult.hasPassed() && numOfSyn != 0) { // if clauseResult has synonym/s, send to ResultProjector to merge.
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

// Function that encapsulates optimization logic
vector<Clause*> QueryEvaluator::optimizationSort(vector<Clause*>& unconnectedClauses) {
	vector<pair<unordered_set<string>, vector<Clause*>>> connectedClauses; // unordered_set<string> = common synonyms, vector<Clause*> = clauses with those synonyms
	groupBasedOnConnectedSyn(unconnectedClauses, connectedClauses); // group clauses base on common synonyms
	sortWithinEachGroup(connectedClauses); // Within each group, sort clauses such that synonym/s in one clause appears in a previous clause
	return combineClauses(connectedClauses);
} 

void QueryEvaluator::groupBasedOnConnectedSyn(vector<Clause*>& unconnectedClauses, vector<pair<unordered_set<string>, vector<Clause*>>>& connectedClauses) {
	for (auto& clause : unconnectedClauses) {
		unordered_set<string> synonyms = clause->getSynonyms();
		bool addedIntoGroup = false;
		for (auto& itr = connectedClauses.begin(); itr != connectedClauses.end(); ++itr) {
			unordered_set<string>& synonymGroup = itr->first;
			vector<Clause*>& clauseGroup = itr->second;
			if (hasCommonSynonyms(synonyms, synonymGroup)) {
				synonymGroup.insert(synonyms.begin(), synonyms.end());
				clauseGroup.push_back(clause);
				addedIntoGroup = true;
				// New added clause may connect this synonym-clause group with another subsequent synonym-clause group. 
				vector<pair<unordered_set<string>, vector<Clause*>>>::iterator ptr = itr;
				ptr++;
				for (; ptr != connectedClauses.end(); ptr++) {
					unordered_set<string>& subsequentSynGroup = ptr->first;
					vector<Clause*>& subsequentClauseGroup = ptr->second;
					if (hasCommonSynonyms(synonyms, subsequentSynGroup)) { 
						// combine the 2 groups together
						synonymGroup.insert(subsequentSynGroup.begin(), subsequentSynGroup.end());
						clauseGroup.insert(clauseGroup.end(), subsequentClauseGroup.begin(), subsequentClauseGroup.end());
						connectedClauses.erase(ptr);
						break;
					}
				}
				break;
			}
		}
		if (!addedIntoGroup) {
			connectedClauses.push_back(make_pair(synonyms, vector<Clause*>({ clause })));
		}
	}
}

bool QueryEvaluator::hasCommonSynonyms(const unordered_set<string>& synonymSet1, const unordered_set<string>& synonymSet2) {
	for (const auto& syn : synonymSet1) {
		if (synonymSet2.count(syn)) {
			return true;
		}
	}
	return false;
}

// Within each group, sort clauses such that synonym/s in one clause appears in any previous clause i.e. (s,a) -> (a, b) -> (s,w) -> (w,c)
void QueryEvaluator::sortWithinEachGroup(vector<pair<unordered_set<string>, vector<Clause*>>>& connectedClauses) {
	for (auto& synClausePair : connectedClauses) {
		vector<Clause*>& clauseGroup = synClausePair.second;
		if (clauseGroup.size() < 3) {
			continue; // clause group with two or less clauses are always sorted
		}
		unordered_set<string> appearedBefore;
		unordered_set<string> clauseOneSyn = clauseGroup[0]->getSynonyms();
		unordered_set<string> clauseTwoSyn = clauseGroup[1]->getSynonyms();
		appearedBefore.insert(clauseOneSyn.begin(), clauseOneSyn.end());
		appearedBefore.insert(clauseTwoSyn.begin(), clauseTwoSyn.end());

		for (unsigned int i = 2; i < clauseGroup.size(); i++) {
			if (!hasCommonSynonyms(clauseGroup[i]->getSynonyms(), appearedBefore)) { // if synonym/s have not appeared previously,
				for (unsigned int j = i + 1; j < clauseGroup.size(); j++) { // then iterate until we find a clause where synonym/s appeared previously
					if (hasCommonSynonyms(clauseGroup[j]->getSynonyms(), appearedBefore)) {
						Clause* temp = clauseGroup[j]; // and then swap
						clauseGroup[j] = clauseGroup[i];
						clauseGroup[i] = temp;
						break;
					}
				}
			}
			unordered_set<string> synonyms = clauseGroup[i]->getSynonyms(); // updated if there was a swap
			appearedBefore.insert(synonyms.begin(), synonyms.end());
		}
	}
}

vector<Clause*> QueryEvaluator::combineClauses(vector<pair<unordered_set<string>, vector<Clause*>>>& connectedClauses) {
	vector<Clause*> combinedClauses;
	for (auto& synClausePair : connectedClauses) {
		vector<Clause*>& clauseGroup = synClausePair.second;
		combinedClauses.insert(combinedClauses.end(), clauseGroup.begin(), clauseGroup.end());
	}
	return combinedClauses;
}
