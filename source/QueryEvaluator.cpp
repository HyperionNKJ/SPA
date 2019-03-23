#include "QueryEvaluator.h"
#include "ResultProjector.h"
#include "Clause.h"
#include <unordered_set>

std::list<std::string> QueryEvaluator::evaluate(ProcessedQuery& processedQuery, const PKB& pkb) {
	ResultProjector resultProjector;
	resultProjector.resetResults(); // Reset possible old query result
	std::list<std::string> emptyResult;

	std::unordered_set<Clause*> clauses = processedQuery.getClauses();
	for (auto clause : clauses) {
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
	return resultProjector.getResults(processedQuery.getSelectedSynonym(), pkb);
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
