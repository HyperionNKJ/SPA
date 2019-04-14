#include "QueryPreprocessorError.h"
#include "QueryProcessor.h"

std::list<std::string> QueryProcessor::evaluate(std::string& query, PKB& pkb) {
	QueryPreprocessor queryPreprocessor = QueryPreprocessor(query);
	QueryEvaluator queryEvaluator = QueryEvaluator();
	std::list<std::string> results; //initially empty
	ProcessedQuery processedQuery;

	try {
		processedQuery = queryPreprocessor.parse();
	}
	catch (QueryPreprocessorError& exception) {
		if (exception.isSemanticError() && exception.isBooleanResultClause()) {
			results.push_back("FALSE");
		}

		return results;
	}

	results = queryEvaluator.evaluate(processedQuery, pkb);
	return results;
}
