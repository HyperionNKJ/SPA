#pragma once

#include "ProcessedQuery.h"

class QueryPreprocessorWithParser {
public:
	QueryPreprocessorWithParser(std::string& withCl, ProcessedQuery& query);

	bool parse();
private:
	const std::string& WITH_CL;

	ProcessedQuery& query;

	DesignEntity parseWithParam(const std::string& param) const;
	bool isValidAttrRefComparator(const DesignEntity& paramOne, const DesignEntity& paramTwo) const;
};