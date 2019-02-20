#pragma once

#include <list>
#include "../SOURCE/PKB.h"
#include "QueryPreprocessor.h"
#include "QueryEvaluator.h"

class QueryProcessor {
public:
	std::list<std::string> evaluate(std::string, PKB);
};


