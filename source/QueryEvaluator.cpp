#include "QueryEvaluator.h"
#include "ResultProjector.h"
#include "Clause.h"
#include <unordered_set>
#include <map>

list<string> QueryEvaluator::evaluate(ProcessedQuery& processedQuery, const PKB& pkb) {
	ResultProjector resultProjector;
	resultProjector.resetResults(); // Reset possible old query result

	vector<Clause*>& booleanClauses = processedQuery.booleanClauses; // boolean clause refers to clause without synonym.
	vector<Clause*> expensiveBooleanClauses; // i.e. Next* / Affects / Affects* boolean clauses
	vector<Clause*> regularBooleanClauses; // other boolean clauses
	sortBooleanClauses(booleanClauses, expensiveBooleanClauses, regularBooleanClauses); 

	vector<Clause*>& sortedWithClauses = optimizationSort(processedQuery.withClauses); // all types of clauses below have synonyms. Hence optimization is required
	vector<Clause*>& sortedNextTClauses = optimizationSort(processedQuery.nextTClauses);
	vector<Clause*>& sortedAffectsClauses = optimizationSort(processedQuery.affectsClauses);
	vector<Clause*>& sortedAffectsTClauses = optimizationSort(processedQuery.affectsTClauses);
	vector<Clause*>& sortedOtherClauses = optimizationSort(processedQuery.otherClauses);

	vector<Clause*> combinedClauses; // optimally combined with the following order
	combinedClauses.insert(combinedClauses.end(), regularBooleanClauses.begin(), regularBooleanClauses.end());
	combinedClauses.insert(combinedClauses.end(), sortedWithClauses.begin(), sortedWithClauses.end());
	combinedClauses.insert(combinedClauses.end(), sortedOtherClauses.begin(), sortedOtherClauses.end());
	combinedClauses.insert(combinedClauses.end(), expensiveBooleanClauses.begin(), expensiveBooleanClauses.end());
	combinedClauses.insert(combinedClauses.end(), sortedNextTClauses.begin(), sortedNextTClauses.end());
	combinedClauses.insert(combinedClauses.end(), sortedAffectsClauses.begin(), sortedAffectsClauses.end());
	combinedClauses.insert(combinedClauses.end(), sortedAffectsTClauses.begin(), sortedAffectsTClauses.end());

	for (auto clause : combinedClauses) {
		ClauseType clauseType = clause->getClauseType();

		// if clause type is Next* / Affects / Affects*, we employ the logic of domain reduction OR caching to optimize evaluation
		bool clauseIsExpensive = clauseType == NEXT_T || clauseType == AFFECTS || clauseType == AFFECTS_T;
		bool reducedDomainExists = false;
		bool cacheResultExists = false;
		if (clauseIsExpensive && clause->getNumOfSynonyms() != 0) {
			reducedDomainExists = findReducedDomain(clause, &resultProjector); // reduce domain based on resultProjector's intermediate table
			// if reducedDomain exists, clause will use reduced domain to evaluate.
			
			if (!reducedDomainExists && (cacheResultExists = resultProjector.cacheExists(clause))) {
				bool hasResult = resultProjector.combineCacheResults(clause); // if results were cached beforehand, simply re-use the results.
				if (!hasResult) {
					return returnNegativeResult(processedQuery.resultClElemList);
				}
				continue; // skip re-evaluation
			}
		}

		Result clauseResult = clause->evaluate(pkb);
		int numOfSyn = clauseResult.getNumOfSyn(); // num of synonyms in clauseResult is zero if clause is false or if boolean clause
		if (clauseResult.hasPassed() && numOfSyn != 0) { // if clauseResult has synonym/s, send to ResultProjector to merge.
			bool hasResultSoFar;
			vector<string> synonyms = clauseResult.getSynonyms();
			bool shouldStoreInCache = !reducedDomainExists && !cacheResultExists && clauseIsExpensive; // Cannot store in cache if result was evaluated using reduced domain!
			if (numOfSyn == 1) {
				hasResultSoFar = resultProjector.combineResults(clauseResult.getOneSynAnswer(), synonyms);
				if (shouldStoreInCache) {
					resultProjector.storeInCache(clause, clauseResult.getOneSynAnswer());
				}
			}
			else if (numOfSyn == 2) { 
				hasResultSoFar = resultProjector.combineResults(clauseResult.getTwoSynAnswer(), synonyms);
				if (shouldStoreInCache) {
					resultProjector.storeInCache(clause, clauseResult.getTwoSynAnswer());
				}
			}
			if (!hasResultSoFar) {
				return returnNegativeResult(processedQuery.resultClElemList);
			}
		}
		else if (!clauseResult.hasPassed()) {
			return returnNegativeResult(processedQuery.resultClElemList);
		}
	}
	return resultProjector.getResults(processedQuery.resultClElemList, pkb);
}

list<string> QueryEvaluator::returnNegativeResult(vector<DesignEntity>& selectClause) {
	list<string> emptyResult;
	if (selectClause[0].getType() == Type::BOOLEAN) {
		emptyResult.push_back("FALSE");
	}
	return emptyResult;
}

// only called by NextT, Affects, and AffectsT
bool QueryEvaluator::findReducedDomain(Clause* clause, ResultProjector* resultProjector) {
	const unordered_set<string>& synonyms = clause->getSynonyms(); // 1 or 2 synonyms
	unordered_map<string, unordered_set<int>> reducedDomain;
	bool hasReducedDomain = false;
	for (const auto& s : synonyms) {
		if (resultProjector->synonymExists(s)) {
			reducedDomain.insert({ s, resultProjector->getPossibleValues(s) });
			hasReducedDomain = true;
		}
	}
	clause->setReducedDomain(reducedDomain);
	return hasReducedDomain;
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

void QueryEvaluator::sortBooleanClauses(const vector<Clause*>& booleanClauses, vector<Clause*>& expensiveBooleanClauses, vector<Clause*>& regularBooleanClauses) {
	for (const auto& clause : booleanClauses) {
		ClauseType clauseType = clause->getClauseType();
		if (clauseType == NEXT_T || clauseType == AFFECTS || clauseType == AFFECTS_T) {
			expensiveBooleanClauses.push_back(clause);
		}
		else {
			regularBooleanClauses.push_back(clause);
		}
	}
}
