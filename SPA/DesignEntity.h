#pragma once
#include <string>
#include "Type.h"

using namespace std;

class DesignEntity {
private:
	string value;
	Type type;

public:
	DesignEntity();
	~DesignEntity();
	DesignEntity(string, Type);

	void setValue(string);
	void setType(Type);
	string getValue();
	Type getType();
};

