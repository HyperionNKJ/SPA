#include "QueryPreprocessorError.h"
#include "QueryProcessor.h"

std::list<std::string> QueryProcessor::evaluate(std::string& query, const PKB& pkb) {
	QueryPreprocessor queryPreprocessor = QueryPreprocessor(query);
	QueryEvaluator queryEvaluator = QueryEvaluator();
	std::list<std::string> results; //initially empty

	try {
		bool isValidQuery = queryPreprocessor.parse();
		if (!isValidQuery) {
			return results; // invalid query
		}
	}
	catch (QueryPreprocessorError& exception) {
		if (exception.isSemanticError() && exception.isBooleanResultClause()) {
			results.push_back("FALSE");
		}

		return results;
	}

	ProcessedQuery processedQuery = queryPreprocessor.getProcessedQuery();
	processedQuery.optimiseClauses();

	results = queryEvaluator.evaluate(processedQuery, pkb);
	return results;
}