#include "QueryEvaluator.h"
#include "ResultProjector.h"
#include "Clause.h"
#include <unordered_set>

std::list<std::string> QueryEvaluator::evaluate(ProcessedQuery processedQuery, PKB pkb) {
	ResultProjector resultProjector;
	resultProjector.resetResults(); // Reset possible old query result
	std::list<std::string> finalResult; // initially empty

	std::unordered_set<Clause> clauses = processedQuery.getClauses();
	for (auto clause : clauses) {
		Result clauseResult = clause.evaluate(pkb);
		if (clauseResult.hasPassed() && clauseResult.hasAnswer()) { // hasPassed && noAnswer denotes a True/False clause
			bool hasResultSoFar = resultProjector.combineResults(clauseResult.getAnswer());
			if (!hasResultSoFar) {
				return finalResult; // return empty result
			}
		}
		else if (!clauseResult.hasPassed()) {
			return finalResult;
		}
	}
	return resultProjector.getResults(processedQuery.getSelectedSynonym()); // To be clarified.
}
