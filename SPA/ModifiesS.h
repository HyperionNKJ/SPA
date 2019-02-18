#pragma once
#include "Clause.h"

class ModifiesS : public Clause {
private:
	Result* evaluateFixedVariable(string, string); // e.g. Modifies(7, v)
	Result* evaluateFixedUnderscore(string); // e.g. Modifies(7, _)
	Result* evaluateFixedFixed(string, string); // e.g. Modifies(7, "count")
	Result* evaluateSynonymVariable(string, string, Type); // e.g. Modifies(s, v)
	Result* evaluateSynonymUnderscore(string, Type); // e.g. Modifies(a, _)
	Result* evaluateSynonymFixed(string, string, Type); // e.g. Modifies(w, "count")

public:
	ModifiesS(DesignEntity, DesignEntity);

	Result evaluate() override;
};