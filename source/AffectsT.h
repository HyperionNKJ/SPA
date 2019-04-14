#pragma once
#include "Clause.h"

class AffectsT : public Clause {
private:
	Result evaluateFixedSynonym(const std::string&, const std::string&); // e.g. Affects*(5, a)
	Result evaluateFixedUnderscore(const std::string&); // e.g. Affects*(6, _)
	Result evaluateFixedFixed(const std::string&, const std::string&); // e.g. Affects*(11, 15)
	Result evaluateSynonymSynonym(const std::string&, const std::string&); // e.g. Affects*(s, pl)
	Result evaluateSynonymUnderscore(const std::string&); // e.g. Affects*(a, _)
	Result evaluateSynonymFixed(const std::string&, const std::string&); // Affects*(pl, 14)
	Result evaluateUnderscoreSynonym(const std::string&); // Affects*(_, s)
	Result evaluateUnderscoreUnderscore(); // Affects*(_, _)
	Result evaluateUnderscoreFixed(const std::string&); // Affects*(_, 8)

	bool evaluateSynSynFromModifier(const std::unordered_set<int>&, PKB&, std::unordered_map<int, std::unordered_set<int>>&, const bool&);
	bool evaluateSynSynFromUser(const std::unordered_set<int>&, PKB&, std::unordered_map<int, std::unordered_set<int>>&, const bool&);
	void evaluateSameSynonym(const std::unordered_set<int>&, PKB&, std::unordered_map<int, std::unordered_set<int>>&);

public:
	AffectsT(const DesignEntity&, const DesignEntity&);
	Result evaluate(const PKB&) override;
};