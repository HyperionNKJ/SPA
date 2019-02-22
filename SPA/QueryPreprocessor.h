#pragma once

#include "ProcessedQuery.h"

class QueryPreprocessor {
public:
	QueryPreprocessor(const std::string& query);
	bool parse();
	ProcessedQuery getProcessedQuery();
private:
	const std::string& QUERY;
	ProcessedQuery processedQuery;
};
