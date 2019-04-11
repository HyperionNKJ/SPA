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
	catch (const char* msg) {
		if (msg == "Invalid") {
			results.push_back("FALSE");
			return results;
		}
	}
	

	ProcessedQuery processedQuery = queryPreprocessor.getProcessedQuery();
	processedQuery.optimiseClauses();

	results = queryEvaluator.evaluate(processedQuery, pkb);
	return results;
}