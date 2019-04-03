#pragma once

#include "ProcessedQuery.h"

class QueryPreprocessorPatternParser {
public:
	QueryPreprocessorPatternParser(std::string& clause, ProcessedQuery& query);

	bool parse();
private:
	static const std::regex EXPRESSION_REGEX;

	const std::string& CLAUSE;
	ProcessedQuery& query;

	DesignEntity parseEntRef(const std::string& parameter) const;

	static DesignEntity parseExpression(const std::string& expression);
};
