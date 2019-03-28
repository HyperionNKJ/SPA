#pragma once

#include "ProcessedQuery.h"

class QueryPreprocessorPatternParser {
public:
	ProcessedQuery query;

	QueryPreprocessorPatternParser(std::string& clause, ProcessedQuery& query);

	bool parse();
private:
	static const std::regex EXPRESSION_REGEX;

	const std::string CLAUSE;

	DesignEntity parseEntRef(std::string& parameter);

	static DesignEntity parseExpression(std::string& expression);
};
