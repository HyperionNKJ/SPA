#pragma once
#include <string>
#include <list>
#include "DesignEntity.h"
#include "Result.h"
#include "PKB.h"

using namespace std;

class Clause {
protected:
	DesignEntity paraOne;
	DesignEntity paraTwo;
	PKB pkb;

	Clause(const DesignEntity&, const DesignEntity&);

public:
	void setParaOne(const DesignEntity&);
	void setParaTwo(const DesignEntity&);
	virtual Result evaluate(const PKB&) = 0;
};