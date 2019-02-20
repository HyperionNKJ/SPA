#pragma once
#include "Clause.h"

class ParentT : public Clause {
public:
	ParentT(DesignEntity, DesignEntity);

	Result evaluate(PKB) override;
};