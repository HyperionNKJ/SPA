#pragma once

#include "ProcessedQuery.h"

class QueryPreprocessorWithParser {
public:
	ProcessedQuery query;

	QueryPreprocessorWithParser(std::string& withCl, ProcessedQuery& query);

	bool parse();
private:
	const std::string WITH_CL;

	DesignEntity parseWithParam(std::string& param);
	bool isValidAttrRefComparator(DesignEntity& paramOne, DesignEntity& paramTwo);
};