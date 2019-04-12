#pragma once

#include "ProcessedQuery.h"

class QueryPreprocessor {
public:
	QueryPreprocessor(std::string& query);
	ProcessedQuery parse();
private:
	std::string& query;
	ProcessedQuery processedQuery;
};
