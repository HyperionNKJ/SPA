#pragma once

#include "ProcessedQuery.h"

class QueryPreprocessorResultParser {
public:
	QueryPreprocessorResultParser(const std::string& clause, ProcessedQuery& query);

	bool parse();
private:
	const std::string& CLAUSE;

	ProcessedQuery& query;

	bool addElement(std::string& elem);
};
