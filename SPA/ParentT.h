#pragma once
#include "Clause.h"

class ParentT : public Clause {
private:
	Result* evaluateFixedSynonym(string, string, Type); // e.g. Parent*(12, w)
	Result* evaluateFixedUnderscore(string); // e.g. Parent*(3, _)
	Result* evaluateFixedFixed(string, string); // e.g. Parent*(4, 6)
	Result* evaluateSynonymSynonym(string, string, Type, Type); // e.g. Parent*(i, a)
	Result* evaluateSynonymUnderscore(string, Type); // e.g. Parent*(w, _)
	Result* evaluateSynonymFixed(string, string, Type); // Parent*(i, 12)
	Result* evaluateUnderscoreSynonym(string, Type); // Parent*(_, a)
	Result* evaluateUnderscoreUnderscore(); // Parent*(_, _)
	Result* evaluateUnderscoreFixed(string); // Parent*(_, 23)

public:
	ParentT(DesignEntity, DesignEntity);

	Result evaluate(PKB) override;
};