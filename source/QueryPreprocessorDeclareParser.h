#pragma once

#include "ProcessedQuery.h"

class QueryPreprocessorDeclareParser {
public:
	ProcessedQuery& query;
	QueryPreprocessorDeclareParser(const std::string& statement, ProcessedQuery& query);
	bool parse();
private:
	const std::string& STATEMENT;
};
