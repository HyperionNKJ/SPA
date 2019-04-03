#pragma once

#include "ProcessedQuery.h"

class QueryPreprocessor {
public:
	QueryPreprocessor(std::string& query);
	bool parse();
	ProcessedQuery getProcessedQuery() const;
private:
	std::string& query;
	ProcessedQuery processedQuery;
};
