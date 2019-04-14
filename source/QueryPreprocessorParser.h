#pragma once
#include "ProcessedQuery.h"

// Parses the query.
class QueryPreprocessorParser {
public:
	// Initializes a newly created QueryPreprocessorParser.
	QueryPreprocessorParser(const std::vector<std::string>& statements);
	
	// Parses the statements.
	void parse();

	// Returns the processed query.
	ProcessedQuery getQuery();
private:
	const std::vector<std::string>& STATEMENTS;
	ProcessedQuery query;
};