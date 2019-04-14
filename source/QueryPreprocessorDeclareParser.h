#pragma once

#include "ProcessedQuery.h"

class QueryPreprocessorDeclareParser {
public:
	ProcessedQuery& query;
	QueryPreprocessorDeclareParser(const std::string& statement, ProcessedQuery& query, const std::string& selectCl);
	void parse();
private:
	const std::string& STATEMENT;
	const std::string& SELECT_CL;
};
