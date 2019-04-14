#pragma once
#include "Clause.h"

// A class that represents Calls clause and its evaluation logic
class Calls : public Clause {
private:
	Result evaluateFixedSynonym(const std::string&, const std::string&); // e.g. Calls("main", p)
	Result evaluateFixedUnderscore(const std::string&); // e.g. Calls("main", _)
	Result evaluateFixedFixed(const std::string&, const std::string&); // e.g. Calls("main", "first")
	Result evaluateSynonymSynonym(const std::string&, const std::string&); // e.g. Calls(p, q)
	Result evaluateSynonymUnderscore(const std::string&); // e.g. Calls(p, _)
	Result evaluateSynonymFixed(const std::string&, const std::string&); // Calls(p, "first")
	Result evaluateUnderscoreSynonym(const std::string&); // Calls(_, q)
	Result evaluateUnderscoreUnderscore(); // Calls(_, _)
	Result evaluateUnderscoreFixed(const std::string&); // Calls(_, "first")

public:
	Calls(const DesignEntity&, const DesignEntity&);
	Result evaluate(const PKB&) override;
};
