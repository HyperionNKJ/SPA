#include "QueryEvaluator.h"
#include "ResultProjector.h"
#include "Clause.h"
#include <unordered_set>
#include <map>

	/*
list<string> QueryEvaluator::evaluate(ProcessedQuery& processedQuery, const PKB& pkb) {
	ResultProjector resultProjector;
	resultProjector.resetResults(); // Reset possible old query result
	list<string> emptyResult;

	vector<Clause*> booleanClauses = processedQuery.booleanClauses; // boolean clause refers to clause without synonym.
	vector<Clause*> withClauses = processedQuery.withClauses; // all clauses below have synonyms
	vector<Clause*> otherClauses = processedQuery.otherClauses;
	vector<Clause*> affectsClauses = processedQuery.affectsClauses;
	vector<Clause*> affectsTClauses = processedQuery.affectsTClauses;
	vector<Clause*> nextTClauses = processedQuery.nextTClauses;


	vector<Clause*> sortedClauses = optimizationSort(otherClauses);
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



// Function that encapsulates optimization logic
vector<Clause*> QueryEvaluator::optimizationSort(vector<Clause*>& unconnectedClauses) {
	vector<pair<unordered_set<string>, vector<Clause*>>> connectedClauses; // unordered_set<string> = common synonyms, vector<Clause*> = clauses with those synonyms
	groupBasedOnConnectedSyn(unconnectedClauses, connectedClauses); // group clauses base on common synonyms

} 
	*/

void QueryEvaluator::groupBasedOnConnectedSyn(vector<Clause*>& unconnectedClauses, vector<pair<unordered_set<string>, vector<Clause*>>>& connectedClauses) {
	for (auto& clause : unconnectedClauses) {
		unordered_set<string> synonyms = clause->getSynonyms();
		bool addedIntoGroup = false;
		for (auto& itr = connectedClauses.begin(); itr != connectedClauses.end(); ++itr) {
			if (hasCommonSynonyms(synonyms, itr->first)) {
				itr->first.insert(synonyms.begin(), synonyms.end());
				itr->second.push_back(clause);
				addedIntoGroup = true;
				vector<pair<unordered_set<string>, vector<Clause*>>>::iterator ptr = itr;
				ptr++;
				for (; ptr != connectedClauses.end(); ptr++) {
					if (hasCommonSynonyms(synonyms, ptr->first)) {
						itr->first.insert(ptr->first.begin(), ptr->first.end());
						itr->second.insert(itr->second.end(), ptr->second.begin(), ptr->second.end());
						connectedClauses.erase(ptr);
						break;
					}
				}
				break;
			}
		}
		if (!addedIntoGroup) {
			vector<Clause*> newClauseGroup = { clause };
			connectedClauses.push_back(make_pair(synonyms, newClauseGroup));
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

/*
	sortBasedOnNumOfSyn(intermediateClauses); // sort clauses within each group
	return combineClauseGroups(noSynClauses, intermediateClauses);



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
*/