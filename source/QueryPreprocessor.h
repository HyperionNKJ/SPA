#pragma once

#include "ProcessedQuery.h"

class QueryPreprocessor {
public:
	QueryPreprocessor(std::string& query);
	bool parse();
	ProcessedQuery getProcessedQuery();
private:
	std::string& query;
	ProcessedQuery processedQuery;
};
