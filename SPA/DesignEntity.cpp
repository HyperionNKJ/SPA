#include "DesignEntity.h"

DesignEntity::DesignEntity() {}

DesignEntity::~DesignEntity() {}

DesignEntity::DesignEntity(string value, Type type) {
	this->value = value;
	this->type = type;
}

void DesignEntity::setValue(string value) {
	this->value = value;
}

void DesignEntity::setType(Type type) {
	this->type = type;
}

string DesignEntity::getValue() {
	return this->value;
}

Type DesignEntity::getType() {
	return this->type;
}