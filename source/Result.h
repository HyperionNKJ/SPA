#pragma once
#include <unordered_map>
#include <unordered_set>

using namespace std;

class Result {
private:
	bool isPassed;
	vector<string> synonyms; // there can be 0, 1, or 2 synonyms
	unordered_set<int> oneSynonymAnswer; // data structure to store answer in the case of 1 synonym
	unordered_map<int, unordered_set<int>> twoSynonymAnswer; // in the case of 2 synonyms. Key = synonyms[0], Value = synonyms[1]

	unordered_set<int> convertStringsToIndices(const unordered_set<string>&, const unordered_map<string, int>&);

public:
	bool hasPassed();
	void setPassed(bool);
	vector<string> getSynonyms();
	int getNumOfSyn();
	unordered_set<int> getOneSynAnswer();
	unordered_map<int, unordered_set<int>> getTwoSynAnswer();

	// setters for oneSynonymAnswer 
	void setAnswer(const string&, const int&);
	void setAnswer(const string&, const unordered_set<int>&);
	void setAnswer(const string&, const unordered_set<string>&, const unordered_map<string, int>&);

	// setters for twoSynonymAnswer
	void setAnswer(const string&, const string&, const unordered_map<int, int>&);
	void setAnswer(const string&, const string&, const unordered_map<int, string>&, const unordered_map<string, int>&);
	void setAnswer(const string&, const string&, const unordered_map<int, unordered_set<int>>&);
	void setAnswer(const string&, const string&, const unordered_map<int, unordered_set<string>>&, const unordered_map<string, int>&);
	void setAnswer(const string&, const string&, const unordered_map<string, unordered_set<string>>&, const unordered_map<string, int>&);
	void setAnswer(const string&, const string&, const unordered_map<string, unordered_set<string>>&, const unordered_map<string, int>&, const unordered_map<string, int>&);
};
