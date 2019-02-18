#include "Clause.h"

#pragma once

class ModifiesS : public Clause {
private:
	Result* evaluateFixedVariable(string, string); // e.g. (7, v)
	Result* evaluateFixedUnderscore(string); // e.g. (7, _)
	Result* evaluateFixedFixed(string, string); // e.g. (7, "count")
	Result* evaluateSynonymVariable(string, string, Type); // e.g. (s, v)
	Result* evaluateSynonymUnderscore(string, Type); // e.g. (a, _)
	Result* evaluateSynonymFixed(string, string, Type); // e.g. (w, "count")

public:
	ModifiesS(DesignEntity, DesignEntity);

	Result evaluate() override;
};