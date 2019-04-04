#pragma once

#include "ProcessedQuery.h"

class QueryPreprocessorWithParser {
public:
	QueryPreprocessorWithParser(std::string& clause, ProcessedQuery& query);
	bool parse();
private:
	const std::string& CLAUSE;
	ProcessedQuery& query;
	DesignEntity parseWithParam(const std::string& param) const;
	bool isValidAttrRefComparator(const DesignEntity& paramOne, const DesignEntity& paramTwo) const;
};