#pragma once
#include <string>
#include "AttrRef.h"
#include "Type.h"

using namespace std;

class DesignEntity {
private:
	string value;
	Type type;
	AttrRef attrRef;

public:
	
	DesignEntity();
	DesignEntity(const string&, const Type&);
	DesignEntity(const string&, const Type&, const AttrRef&);

	void setValue(const string&);
	void setType(const Type&);
	void setAttrRef(const AttrRef&);

	string getValue();
	Type getType();
	AttrRef getAttrRef();
};

