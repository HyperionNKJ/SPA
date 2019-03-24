#pragma once

#include "ProcessedQuery.h"

class QueryPreprocessorPatternParser {
public:
	QueryPreprocessorPatternParser(std::string& clause, ProcessedQuery& query);

	bool parse();
private:
	static const std::regex IDENT_REGEX;
	static const std::regex INT_REGEX;
	static const std::regex REL_REF_REGEX;
	static const std::regex EXPRESSION_REGEX;

	std::string clause;
	ProcessedQuery query;

	DesignEntity parseEntRef(std::string& parameter);
	DesignEntity parseExpression(std::string& expression);
};
