#pragma once
#include "Clause.h"

class Parent : public Clause {
public:
	Parent(DesignEntity, DesignEntity);

	Result evaluate(PKB) override;
};