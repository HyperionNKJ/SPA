#include "Result.h"

bool Result::hasPassed() {
	return this->isPassed;
}

void Result::setPassed(bool isPassed) {
	this->isPassed = isPassed;
}

bool Result::hasAnswer() {
	return !(this->answer.empty());
}

unordered_map<string, list<int>> Result::getAnswer() {
	return this->answer;
}

// store variables as int corresponding to their index in varTable
void Result::setAnswer(string variableSynonym, unordered_set<string> variables) {
	list<int> variableIndices;
	for (auto itr = variables.begin(); itr != variables.end(); itr++) { 
		string variable = *itr;
		variableIndices.push_back(getVariableIndex(variable));
	}
	unordered_map<string, list<int>> formattedAnswer({ {variableSynonym, variableIndices} });
	this->answer = formattedAnswer;
}

void Result::setAnswer(string stmtSynonym, string variableSynonym, unordered_map<int, unordered_set<string>> answer) {
	list<int> stmtList;
	list<int> variableIndices;

	for (auto itr = answer.begin(); itr != answer.end(); itr++) {
		int stmtNum = itr->first;
		unordered_set<string> variableNames = itr->second;
		auto it = variableNames.begin();
		for (int i = 0; i < variableNames.size(); i++) {
			stmtList.push_back(stmtNum);
			int variableIndex = getVariableIndex(*it);
			variableIndices.push_back(variableIndex);
			it++;
		}
	}
	unordered_map<string, list<int>> formattedAnswer({ {stmtSynonym, stmtList}, {variableSynonym, variableIndices} });
	this->answer = formattedAnswer;
}

void Result::setAnswer(string stmt1Synonym, string stmt2Synonym, unordered_map<int, unordered_set<int>> answer) {
	list<int> stmt1List;
	list<int> stmt2List;

	for (auto itr = answer.begin(); itr != answer.end(); itr++) {
		int stmt1Num = itr->first;
		unordered_set<int> stmt2Numbers = itr->second;
		auto it = stmt2Numbers.begin();
		for (int i = 0; i < stmt2Numbers.size(); i++) {
			stmt1List.push_back(stmt1Num);
			stmt2List.push_back(*it);
			it++;
		}
	}
	unordered_map<string, list<int>> formattedAnswer({ {stmt1Synonym, stmt1List}, {stmt2Synonym, stmt2List} });
	this->answer = formattedAnswer;
}

void Result::setAnswer(string stmtSynonym, unordered_set<int> stmtNums) {
	list<int> stmtList(stmtNums.begin(), stmtNums.end());
	unordered_map<string, list<int>> formattedAnswer({ {stmtSynonym, stmtList}});
	this->answer = formattedAnswer;
}

void Result::setAnswer(string stmtSynonym, int answer) {
	list<int> stmtList({ answer });
	unordered_map<string, list<int>> formattedAnswer({ {stmtSynonym, stmtList} });
	this->answer = formattedAnswer;
}