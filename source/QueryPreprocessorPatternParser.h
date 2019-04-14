#pragma once
#include <regex>
#include "ProcessedQuery.h"

// Parses the pattern clause.
class QueryPreprocessorPatternParser {
public:
	// Initializes a newly created QueryPreprocessorPatternParser.
	QueryPreprocessorPatternParser(std::string& clause, ProcessedQuery& query);
	
	// Parses the pattern clause.
// Returns true if parsing is successful and false if unsucessful.
	void parse();
private:
	static const std::regex EXPRESSION_REGEX;
	const std::string& CLAUSE;
	ProcessedQuery& query;
	DesignEntity parseEntRef(const std::string& parameter) const;
	DesignEntity parseExpression(const std::string& expression) const;
};
