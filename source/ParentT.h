#pragma once
#include "Clause.h"

// A class that represents ParentT clause and its evaluation logic
class ParentT : public Clause {
private:
	Result evaluateFixedSynonym(const std::string&, const std::string&, const Type&); // e.g. Parent*(12, w)
	Result evaluateFixedUnderscore(const std::string&); // e.g. Parent*(3, _)
	Result evaluateFixedFixed(const std::string&, const std::string&); // e.g. Parent*(4, 6)
	Result evaluateSynonymSynonym(const std::string&, const std::string&, const Type&, const Type&); // e.g. Parent*(i, a)
	Result evaluateSynonymUnderscore(const std::string&, const Type&); // e.g. Parent*(w, _)
	Result evaluateSynonymFixed(const std::string&, const std::string&, const Type&); // Parent*(i, 12)
	Result evaluateUnderscoreSynonym(const std::string&, const Type&); // Parent*(_, a)
	Result evaluateUnderscoreUnderscore(); // Parent*(_, _)
	Result evaluateUnderscoreFixed(const std::string&); // Parent*(_, 23)

public:
	ParentT(const DesignEntity&, const DesignEntity&);
	Result evaluate(const PKB&) override;
};