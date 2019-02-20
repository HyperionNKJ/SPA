#pragma once
#include <string>
#include <list>
#include "DesignEntity.h"
#include "Result.h"
#include "../SOURCE/PKB.h"

using namespace std;

class Clause {
protected:
	DesignEntity paraOne;
	DesignEntity paraTwo;
	PKB pkb;

	Clause(DesignEntity, DesignEntity);

public:
	void setParaOne(DesignEntity);
	void setParaTwo(DesignEntity);
	virtual Result evaluate(PKB) = 0;
};