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
		Result clauseResult = clause->evaluate(pkb);
		if (clauseResult.hasPassed() && clauseResult.hasAnswer()) { // hasPassed && noAnswer denotes a True/False clause
			bool hasResultSoFar = resultProjector.combineResults(clauseResult.getAnswer());
			if (!hasResultSoFar) {
				return emptyResult; 
			}
		}
		else if (!clauseResult.hasPassed()) {
			return emptyResult;
		}
	}
	return resultProjector.getResults(processedQuery.getSelectedSynonym(), pkb); // To be clarified.
}
