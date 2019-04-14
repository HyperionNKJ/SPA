#pragma once
#include "Clause.h"

// A class that represents FollowsT clause and its evaluation logic
class FollowsT : public Clause {
private: 
	Result evaluateFixedSynonym(const std::string&, const std::string&, const Type&); // e.g. Follows*(12, w)
	Result evaluateFixedUnderscore(const std::string&); // e.g. Follows*(3, _)
	Result evaluateFixedFixed(const std::string&, const std::string&); // e.g. Follows*(4, 6)
	Result evaluateSynonymSynonym(const std::string&, const std::string&, const Type&, const Type&); // e.g. Follows*(r, a)
	Result evaluateSynonymUnderscore(const std::string&, const Type&); // e.g. Follows*(pr, _)
	Result evaluateSynonymFixed(const std::string&, const std::string&, const Type&); // Follows*(s, 12)
	Result evaluateUnderscoreSynonym(const std::string&, const Type&); // Follows*(_, a)
	Result evaluateUnderscoreUnderscore(); // Follows*(_, _)
	Result evaluateUnderscoreFixed(const std::string&); // Follows*(_, 23)

public:
	FollowsT(const DesignEntity&, const DesignEntity&);
	Result evaluate(const PKB&) override;
};