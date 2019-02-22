#pragma once
#include "Clause.h"

class Follows : public Clause {
private:
	Result* evaluateFixedSynonym(string, string, Type); // e.g. Follows(12, w)
	Result* evaluateFixedUnderscore(string); // e.g. Follows(3, _)
	Result* evaluateFixedFixed(string, string); // e.g. Follows(4, 6)
	Result* evaluateSynonymSynonym(string, string, Type, Type); // e.g. Follows(r, a)
	Result* evaluateSynonymUnderscore(string, Type); // e.g. Follows(pr, _)
	Result* evaluateSynonymFixed(string, string, Type); // Follows(s, 12)
	Result* evaluateUnderscoreSynonym(string, Type); // Follows(_, a)
	Result* evaluateUnderscoreUnderscore(); // Follows(_, _)
	Result* evaluateUnderscoreFixed(string); // Follows(_, 23)

public:
	Follows(DesignEntity, DesignEntity);

	Result evaluate(PKB) override;
};