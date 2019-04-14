#pragma once
#include "ProcessedQuery.h"

// Parses the select clause.
class QueryPreprocessorSelectParser {
public:
	// Initializes a newly created QueryPreprocessorSelectParser.
	QueryPreprocessorSelectParser(const std::string& statement, ProcessedQuery& query);
	
	// Parse select clause.
	void parse();
private:
	enum ClauseType { RESULT, SUCH_THAT, PATTERN, WITH };
	const std::string& STATEMENT;
	ProcessedQuery& query;
};
