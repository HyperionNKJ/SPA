#include "QueryProcessor.h"

std::list<std::string> QueryProcessor::evaluate(std::string query, PKB pkb) {
	QueryPreprocessor queryPreprocessor = QueryPreprocessor(query);
	QueryEvaluator queryEvaluator = QueryEvaluator();
	std::list<std::string> results; //initially empty

	bool isValidQuery = queryPreprocessor.parse();
	if (!isValidQuery) {
		return results; // invalid query
	}
	ProcessedQuery processedQuery = queryPreprocessor.getProcessedQuery();
	std::list<std::string> results = queryEvaluator.evaluate(processedQuery, pkb);
	return results;
}
