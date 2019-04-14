#pragma once
#include "Clause.h"

// A class that represents NextT clause and its evaluation logic
class NextT : public Clause {
private:
	Result evaluateFixedSynonym(const std::string&, const std::string&, const Type&); // e.g. Next*(12, a)
	Result evaluateFixedUnderscore(const std::string&); // e.g. Next*(12, _)
	Result evaluateFixedFixed(const std::string&, const std::string&); // e.g. Next*(12, 13)
	Result evaluateSynonymSynonym(const std::string&, const std::string&, const Type&, const Type&); // e.g. Next*(s, s1)
	Result evaluateSynonymUnderscore(const std::string&, const Type&); // e.g. Next*(w, _)
	Result evaluateSynonymFixed(const std::string&, const std::string&, const Type&); // Next*(s, 14)
	Result evaluateUnderscoreSynonym(const std::string&, const Type&); // Next*(_, pl)
	Result evaluateUnderscoreUnderscore(); // Next*(_, _)
	Result evaluateUnderscoreFixed(const std::string&); // Next*(_, 23)

	bool evaluateSynSynFromPrev(const std::unordered_set<int>&, const Type&, PKB&, std::unordered_map<int, std::unordered_set<int>>&, const bool&);
	bool evaluateSynSynFromNext(const std::unordered_set<int>&, const Type&, PKB&, std::unordered_map<int, std::unordered_set<int>>&, const bool&);
	void evaluateSameSynonym(const std::unordered_set<int>&, PKB&, std::unordered_map<int, std::unordered_set<int>>&);

public:
	NextT(const DesignEntity&, const DesignEntity&);
	Result evaluate(const PKB&) override;
};