#pragma once

#include <unordered_map>
#include "DesignEntityType.h"

class DesignEntityTypeFactory {
public:
	// design entity type to string map and string map to design entity type
	static const std::unordered_map<DesignEntityType, std::string> DESIGN_ENTITY_TO_STRING;
	static const std::unordered_map<std::string, DesignEntityType> STRING_TO_DESIGN_ENTITY;

	// getters for retrieving string and enum of a design entity type
	static const DesignEntityType getDesignEntityType(const std::string& designEntityTypeStr);
	static const std::string getDesignEntityString(const DesignEntityType& designEntityType);
};
