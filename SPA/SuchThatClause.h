#pragma once

#include <string>
#include "DesignEntity.h"

enum class RelationshipType {
	MODIFIES_S, USES_S, PARENT, PARENT_T, FOLLOWS, FOLLOWS_T
};

class SuchThatClause {
public:
	SuchThatClause();

	RelationshipType type;

	DesignEntity paramOne;
	DesignEntity paramTwo;
};
