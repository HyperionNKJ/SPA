#pragma once
#include <unordered_map>
#include <unordered_set>

using namespace std;

class Result {
private:
	bool isPassed;
	unordered_map<string, list<int>> answer;

public:
	bool isPassed();
	void setPassed(bool);
	bool hasAnswer();
	unordered_map<string, list<int>> getAnswer();
	void setAnswer(string, unordered_set<int>);
	void setAnswer(string, unordered_set<string>);
	void setAnswer(string, string, unordered_map<int, unordered_set<string>>);

	void setAnswer(int); // to be implemented
	void setAnswer(string); // to be implemented
	void setAnswer(unordered_map<int, int>); // to be implemented
	void setAnswer(unordered_map<int, unordered_set<int>>); // to be implemented
};
