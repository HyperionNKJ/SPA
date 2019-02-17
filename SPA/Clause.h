#pragma once
#include <string>
#include <list>
#include <unordered_map>
#include "DesignEntity.h"

using namespace std;

class Clause {
protected:
	DesignEntity paraOne;
	DesignEntity paraTwo;

	Clause(DesignEntity, DesignEntity);

public:
	void setParaOne(DesignEntity);
	void setParaTwo(DesignEntity);
	virtual unordered_map<string, list<int>> evaluate() = 0;
};