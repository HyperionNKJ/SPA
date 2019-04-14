#pragma once

#include "AttrRef.h"
#include "Type.h"
#include <string>
#include <vector>

class DesignEntity {
private:
	std::string value;
	Type type;
	AttrRef attrRef;

public:
	DesignEntity();
	DesignEntity(const std::string&, const Type&);
	DesignEntity(const std::string&, const Type&, const AttrRef&);

	void setValue(const std::string&);
	void setType(const Type&);
	void setAttrRef(const AttrRef&);

	std::string getValue();
	Type getType();
	AttrRef getAttrRef();

	bool isType(const Type& type) const;
	bool isAnyType(const std::vector<Type>& types) const;
	bool isAttrRef(const AttrRef& attrRef) const;
	bool isAnyAttrRef(const std::vector<AttrRef>& attrRefs) const;
	bool isStmtNo() const;
	bool isVar() const;
	bool isInvalid() const;
	std::string toString() const;
	bool operator==(const DesignEntity& other) const;
};
