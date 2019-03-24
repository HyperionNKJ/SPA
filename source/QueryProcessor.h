#pragma once

#include <list>
#include "PKB.h"
#include "QueryPreprocessor.h"
#include "QueryEvaluator.h"

class QueryProcessor {
public:
	std::list<std::string> evaluate(std::string&, const PKB&);
};


