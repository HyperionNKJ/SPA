#pragma once
#include <string>
#include <list>
#include "DesignEntity.h"
#include "Result.h"

using namespace std;

class Clause {
protected:
	DesignEntity paraOne;
	DesignEntity paraTwo;

	Clause(DesignEntity, DesignEntity);

public:
	void setParaOne(DesignEntity);
	void setParaTwo(DesignEntity);
	virtual Result evaluate() = 0;
};