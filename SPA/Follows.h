#pragma once
#include "Clause.h"

class Follows : public Clause {
public:
	Follows(DesignEntity, DesignEntity);

	Result evaluate(PKB) override;
};