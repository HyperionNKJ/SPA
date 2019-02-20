#pragma once
#include "Clause.h"

class FollowsT : public Clause {
public:
	FollowsT(DesignEntity, DesignEntity);

	Result evaluate(PKB) override;
};