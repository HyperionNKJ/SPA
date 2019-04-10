#pragma once
#include "Clause.h"

class Affects : public Clause {
private:
	Result evaluateFixedSynonym(const string&, const string&); // e.g. Affects(5, a)
	Result evaluateFixedUnderscore(const string&); // e.g. Affects(6, _)
	Result evaluateFixedFixed(const string&, const string&); // e.g. Affects(11, 15)
	Result evaluateSynonymSynonym(const string&, const string&); // e.g. Affects(s, pl)
	Result evaluateSynonymUnderscore(const string&); // e.g. Affects(a, _)
	Result evaluateSynonymFixed(const string&, const string&); // Affects(pl, 14)
	Result evaluateUnderscoreSynonym(const string&); // Affects(_, s)
	Result evaluateUnderscoreUnderscore(); // Affects(_, _)
	Result evaluateUnderscoreFixed(const string&); // Affects(_, 8)

	bool evaluateSynSynFromModifier(const unordered_set<int>&, PKB&, unordered_map<int, unordered_set<int>>&, const bool&);
	bool evaluateSynSynFromUser(const unordered_set<int>&, PKB&, unordered_map<int, unordered_set<int>>&, const bool&);
	void evaluateSameSynonym(const unordered_set<int>&, PKB&, unordered_map<int, unordered_set<int>>&);

public:
	Affects(const DesignEntity&, const DesignEntity&);
	Result evaluate(const PKB&) override;
};