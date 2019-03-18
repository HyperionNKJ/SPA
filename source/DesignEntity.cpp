#include "DesignEntity.h"

DesignEntity::DesignEntity() {}

DesignEntity::DesignEntity(const string& value, const Type& type) {
	this->value = value;
	this->type = type;
	this->attrRef = AttrRef::UNASSIGNED;
}

DesignEntity::DesignEntity(const string& value, const Type& type, const AttrRef& attrRef) {
	this->value = value;
	this->type = type;
	this->attrRef = attrRef;
}

void DesignEntity::setValue(const string& value) {
	this->value = value;
}

void DesignEntity::setType(const Type& type) {
	this->type = type;
}

void DesignEntity::setAttrRef(const AttrRef& attrRef) {
	this->attrRef = attrRef;
}

string DesignEntity::getValue() {
	return this->value;
}

Type DesignEntity::getType() {
	return this->type;
}

AttrRef DesignEntity::getAttrRef() {
	return this->attrRef;
}