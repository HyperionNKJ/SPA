#pragma once

#include "ProcessedQuery.h"

class QueryPreprocessorWithParser {
public:
	QueryPreprocessorWithParser(std::string& clause, ProcessedQuery& query);
	void parse();
private:
	const std::string& CLAUSE;
	ProcessedQuery& query;
	DesignEntity parseWithParam(const std::string& param) const;
	void isValidAttrRefComparator(const DesignEntity& paramOne, const DesignEntity& paramTwo) const;
};