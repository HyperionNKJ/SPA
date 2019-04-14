#pragma once
#include "ProcessedQuery.h"

// Parses the result clause.
class QueryPreprocessorResultParser {
public:
	// Initializes a newly created QueryPreprocessorPatternParser.
	QueryPreprocessorResultParser(const std::string& clause, ProcessedQuery& query);
	
	// Parses the result clause.
	// Returns true if parsing is successful and false if unsucessful.
	void parse();
private:
	static const std::vector<Type> VALID_TYPES;

	const std::string& CLAUSE;
	ProcessedQuery& query;
	void addElement(const std::string& elem);
};
