#pragma once

#include "ProcessedQuery.h"

class QueryPreprocessorSuchThatParser {
public:
	ProcessedQuery query;

	QueryPreprocessorSuchThatParser(const std::string& clause, ProcessedQuery& query);

	bool parse();
private:

	const std::string CLAUSE;

	static bool isRelRef(std::string& relRef);
};
