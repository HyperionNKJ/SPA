#pragma once

#include "ProcessedQuery.h"

class QueryPreprocessorParser {
public:
	QueryPreprocessorParser(const std::vector<std::string>& statements);
	void parse();
	ProcessedQuery getQuery();
private:
	const std::vector<std::string>& STATEMENTS;
	ProcessedQuery query;
};