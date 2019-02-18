#pragma once
#include "Clause.h"

class UsesS : public Clause {
private:
	Result* evaluateFixedVariable(string, string); // e.g. Uses(7, v)
	Result* evaluateFixedUnderscore(string); // e.g. Uses(7, _)
	Result* evaluateFixedFixed(string, string); // e.g. Uses(7, "count")
	Result* evaluateSynonymVariable(string, string, Type); // e.g. Uses(s, v)
	Result* evaluateSynonymUnderscore(string, Type); // e.g. Uses(a, _)
	Result* evaluateSynonymFixed(string, string, Type); // e.g. Uses(w, "count")

public:
	UsesS(DesignEntity, DesignEntity);

	Result evaluate() override;
};