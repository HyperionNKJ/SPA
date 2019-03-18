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
			setPossibleValues(clause, &resultProjector);
		}

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

void QueryEvaluator::setPossibleValues(Clause* clause, ResultProjector* resultProjector) {
	unordered_set<string> synonyms = clause->getSynonyms(); // 0, 1, or 2 synonyms
	unordered_map<string, unordered_set<int>> possibleValues;
	for (auto s : synonyms) {
		if (resultProjector->synonymExists(s)) {
			possibleValues.insert({ s, resultProjector->getPossibleValues(s) });
		}
	}
	clause->setPossibleValues(possibleValues);
}
