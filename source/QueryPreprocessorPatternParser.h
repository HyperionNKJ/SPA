#pragma once

#include <regex>
#include "ProcessedQuery.h"

class QueryPreprocessorPatternParser {
public:
	QueryPreprocessorPatternParser(std::string& clause, ProcessedQuery& query);
	void parse();
private:
	static const std::regex EXPRESSION_REGEX;
	const std::string& CLAUSE;
	ProcessedQuery& query;
	DesignEntity parseEntRef(const std::string& parameter) const;
	DesignEntity parseExpression(const std::string& expression) const;
};
