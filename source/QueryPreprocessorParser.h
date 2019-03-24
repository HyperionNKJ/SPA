#pragma once

#include "ProcessedQuery.h"
#include "Statement.h"

class QueryPreprocessorParser {
public:
	QueryPreprocessorParser(const std::vector<std::string>& statements);
	bool parse();
	ProcessedQuery getQuery();
private:
	std::vector<std::string> STATEMENTS;
	ProcessedQuery query;
};