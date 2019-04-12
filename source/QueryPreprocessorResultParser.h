#pragma once

#include "ProcessedQuery.h"

class QueryPreprocessorResultParser {
public:
	QueryPreprocessorResultParser(const std::string& clause, ProcessedQuery& query);
	void parse();
private:
	static const std::vector<Type> VALID_TYPES;

	const std::string& CLAUSE;
	ProcessedQuery& query;
	void addElement(const std::string& elem);
};
