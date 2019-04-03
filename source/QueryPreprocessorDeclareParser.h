#pragma once

#include <regex>
#include "ProcessedQuery.h"
#include "Type.h"

class QueryPreprocessorDeclareParser {
public:
	ProcessedQuery& query;

	QueryPreprocessorDeclareParser(const std::string& statement, ProcessedQuery& query);

	bool parse();
private:
	const std::string& STATEMENT;
};
