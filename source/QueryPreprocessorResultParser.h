#pragma once

#include "ProcessedQuery.h"

class QueryPreprocessorResultParser {
public:
	ProcessedQuery query;

	QueryPreprocessorResultParser(const std::string& clause, ProcessedQuery& query);

	bool parse();
private:

	const std::string CLAUSE;

	bool addElement(std::string& elem);
};
