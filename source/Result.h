#pragma once
#include <unordered_map>
#include <unordered_set>

// The class which encapsulates the result for every clause. 
class Result {
private:
	bool isPassed;
	std::vector<std::string> synonyms; // there can be 0, 1, or 2 synonyms
	std::unordered_set<int> oneSynonymAnswer; // data structure to store answer in the case of 1 synonym
	std::unordered_map<int, std::unordered_set<int>> twoSynonymAnswer; // in the case of 2 synonyms. Key = synonyms[0], Value = synonyms[1]

	std::unordered_set<int> convertStringsToIndices(const std::unordered_set<std::string>&, const std::unordered_map<std::string, int>&);

public:
	bool hasPassed();
	void setPassed(bool);
	std::vector<std::string> getSynonyms();
	int getNumOfSyn();
	std::unordered_set<int> getOneSynAnswer();
	std::unordered_map<int, std::unordered_set<int>> getTwoSynAnswer();

	// setters for oneSynonymAnswer 
	void setAnswer(const std::string&, const int&);
	void setAnswer(const std::string&, const std::unordered_set<int>&);
	void setAnswer(const std::string&, const std::unordered_set<std::string>&, const std::unordered_map<std::string, int>&);

	// setters for twoSynonymAnswer
	void setAnswer(const std::string&, const std::string&, const std::unordered_map<int, int>&);
	void setAnswer(const std::string&, const std::string&, const std::unordered_map<int, std::string>&, const std::unordered_map<std::string, int>&);
	void setAnswer(const std::string&, const std::string&, const std::unordered_map<int, std::unordered_set<int>>&);
	void setAnswer(const std::string&, const std::string&, const std::unordered_map<int, std::unordered_set<std::string>>&, const std::unordered_map<std::string, int>&);
	void setAnswer(const std::string&, const std::string&, const std::unordered_map<std::string, std::unordered_set<int>>&, const std::unordered_map<std::string, int>&);
	void setAnswer(const std::string&, const std::string&, const std::unordered_map<std::string, std::unordered_set<std::string>>&, const std::unordered_map<std::string, int>&);
	void setAnswer(const std::string&, const std::string&, const std::unordered_map<std::string, std::unordered_set<std::string>>&, const std::unordered_map<std::string, int>&, const std::unordered_map<std::string, int>&);
};
