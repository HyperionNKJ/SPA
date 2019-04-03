#pragma once

#include "ProcessedQuery.h"

class QueryPreprocessorWithParser {
public:
	QueryPreprocessorWithParser(std::string& withCl, ProcessedQuery& query);

	bool parse();
private:
	const std::string& WITH_CL;

	ProcessedQuery& query;

	DesignEntity parseWithParam(std::string& param);
	bool isValidAttrRefComparator(DesignEntity& paramOne, DesignEntity& paramTwo);
};