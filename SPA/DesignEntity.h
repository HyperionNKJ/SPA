#pragma once
#include <string>
#include "Type.h"

using namespace std;

class DesignEntity {
private:
	

public:
	string value;
	Type type;
	DesignEntity();
	DesignEntity(const string&, const Type&);

	void setValue(const string&);
	void setType(const Type&);
	string getValue();
	Type getType();
};

