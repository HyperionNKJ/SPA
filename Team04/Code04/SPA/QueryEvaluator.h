#pragma once

#include <list>
#include "ProcessedQuery.h"

class QueryEvaluator {
public:
	std::list<std::string> evaluate(ProcessedQuery&, const PKB&);
};
