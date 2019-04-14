#include "DesignEntity.h"
#include "QueryPreprocessorHelper.h"

DesignEntity::DesignEntity() {}

DesignEntity::DesignEntity(const std::string& value, const Type& type) {
	this->value = value;
	this->type = type;
	this->attrRef = AttrRef::UNASSIGNED;
}

DesignEntity::DesignEntity(const std::string& value, const Type& type, const AttrRef& attrRef) {
	this->value = value;
	this->type = type;
	this->attrRef = attrRef;
}

void DesignEntity::setValue(const std::string& value) {
	this->value = value;
}

void DesignEntity::setType(const Type& type) {
	this->type = type;
}

void DesignEntity::setAttrRef(const AttrRef& attrRef) {
	this->attrRef = attrRef;
}

std::string DesignEntity::getValue() {
	return this->value;
}

Type DesignEntity::getType() {
	return this->type;
}

AttrRef DesignEntity::getAttrRef() {
	return this->attrRef;
}

bool DesignEntity::isType(const Type& other) const {
	return type == other;
}

bool DesignEntity::isAnyType(const std::vector<Type>& types) const {
	for (Type index : types) {
		if (type == index) {
			return true;
		}
	}

	return false;
}

bool DesignEntity::isAttrRef(const AttrRef& other) const {
	return attrRef == other;
}

bool DesignEntity::isAnyAttrRef(const std::vector<AttrRef>& attrRefs) const {
	for (AttrRef index : attrRefs) {
		if (attrRef == index) {
			return true;
		}
	}

	return false;
}

bool DesignEntity::isStmtNo() const {
	return type == Type::FIXED && QueryPreprocessorHelper::isInt(value);
}

bool DesignEntity::isVar() const {
	return type == Type::FIXED && QueryPreprocessorHelper::isVar(value);
}

bool DesignEntity::isInvalid() const {
	return type == Type::INVALID;
}

std::string DesignEntity::toString() const {
	size_t typeInt = static_cast<std::size_t>(type);
	size_t attrRefInt = static_cast<std::size_t>(attrRef);
	return std::to_string(typeInt) + std::to_string(attrRef) + value;
}

bool DesignEntity::operator==(const DesignEntity &other) const {
	return type == other.type && attrRef == other.attrRef && value == other.value;
}
