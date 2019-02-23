#pragma once

#include <string>
#include "ParameterType.h"
#include "DesignEntity.h"

enum class RelationshipType {
	MODIFIES_S, USES_S, PARENT, PARENT_T, FOLLOWS, FOLLOWS_T
};

class SuchThatClause {
public:
	SuchThatClause();

	RelationshipType type;

	Type paramOne;
	Type paramTwo;

	Type paramOneType;
	Type paramTwoType;

	std::string paramOneValue;
	std::string paramTwoValue;
};
