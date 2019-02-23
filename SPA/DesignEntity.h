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
	DesignEntity(const string&, const Type&);

	void setValue(const string&);
	void setType(const Type&);
	string getValue();
	Type getType();
};

