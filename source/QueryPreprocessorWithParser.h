#pragma once

#include "ProcessedQuery.h"

// Parses with clauses
class QueryPreprocessorWithParser {
public:
	// Initializes a newly created QueryPreprocessorWithParser.
	QueryPreprocessorWithParser(std::string& clause, ProcessedQuery& query);
	
	// Parses the with clause.
	void parse();
private:
	const std::string& CLAUSE;
	ProcessedQuery& query;
	DesignEntity parseWithParam(const std::string& param) const;
	void isValidAttrRefComparator(const DesignEntity& paramOne, const DesignEntity& paramTwo) const;
};