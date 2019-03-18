#pragma once

#include <list>
#include "ProcessedQuery.h"
#include "ResultProjector.h"

class QueryEvaluator {
private:
	void findReducedDomain(Clause*, ResultProjector*);

public:
	std::list<std::string> evaluate(ProcessedQuery&, const PKB&);
};
