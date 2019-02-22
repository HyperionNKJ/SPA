#pragma once

#include <string>
#include "ParameterType.h"

enum class RelationshipType {
	MODIFIES_S, USES_S, PARENT, PARENT_T, FOLLOWS, FOLLOWS_T
};

class SuchThatClause {
public:
	SuchThatClause();

	RelationshipType type;

	ParameterType paramOneType;
	ParameterType paramTwoType;

	std::string paramOneValue;
	std::string paramTwoValue;
};
