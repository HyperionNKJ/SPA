#include "DesignEntity.h"

DesignEntity::DesignEntity() {}

DesignEntity::DesignEntity(const string& value, const Type& type) {
	this->value = value;
	this->type = type;
}

void DesignEntity::setValue(const string& value) {
	this->value = value;
}

void DesignEntity::setType(const Type& type) {
	this->type = type;
}

string DesignEntity::getValue() {
	return this->value;
}

Type DesignEntity::getType() {
	return this->type;
}