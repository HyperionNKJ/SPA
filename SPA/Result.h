#pragma once
#include <unordered_map>
#include <unordered_set>

using namespace std;

class Result {
private:
	bool isPassed;
	unordered_map<string, list<int>> answer;

public:
	bool hasPassed();
	void setPassed(bool);
	bool hasAnswer();
	unordered_map<string, list<int>> getAnswer();
	void setAnswer(const string&, const int&);
	void setAnswer(const string&, const unordered_set<int>&);
	void setAnswer(const string&, const unordered_set<string>&, const unordered_map<string, int>&);
	void setAnswer(const string&, const string&, const unordered_map<int, unordered_set<string>>&, const unordered_map<string, int>&);
	void setAnswer(const string&, const string&, const unordered_map<int, int>&);
	void setAnswer(const string&, const string&, const unordered_map<int, string>&, const unordered_map<string, int>&);
	void setAnswer(const string&, const string&, const unordered_map<int, unordered_set<int>>&);

	void setAnswer(const string&); // to be implemented
};
