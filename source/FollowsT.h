#pragma once
#include "Clause.h"

class FollowsT : public Clause {
private: 
	Result evaluateFixedSynonym(const string&, const string&, const Type&); // e.g. Follows*(12, w)
	Result evaluateFixedUnderscore(const string&); // e.g. Follows*(3, _)
	Result evaluateFixedFixed(const string&, const string&); // e.g. Follows*(4, 6)
	Result evaluateSynonymSynonym(const string&, const string&, const Type&, const Type&); // e.g. Follows*(r, a)
	Result evaluateSynonymUnderscore(const string&, const Type&); // e.g. Follows*(pr, _)
	Result evaluateSynonymFixed(const string&, const string&, const Type&); // Follows*(s, 12)
	Result evaluateUnderscoreSynonym(const string&, const Type&); // Follows*(_, a)
	Result evaluateUnderscoreUnderscore(); // Follows*(_, _)
	Result evaluateUnderscoreFixed(const string&); // Follows*(_, 23)

public:
	FollowsT(const DesignEntity&, const DesignEntity&);
	Result evaluate(const PKB&) override;
};