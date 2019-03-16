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
void Result::setAnswer(const string& variableSynonym, const unordered_set<string>& variables, const unordered_map<string, int>& varToIndexTable) {
	list<int> variableIndices;
	for (auto itr = variables.begin(); itr != variables.end(); itr++) { 
		string variable = *itr;
		variableIndices.push_back(varToIndexTable.at(variable));
	}
	unordered_map<string, list<int>> formattedAnswer({ {variableSynonym, variableIndices} });
	this->answer = formattedAnswer;
}

void Result::setAnswer(const string& stmtSynonym, const string& variableSynonym, const unordered_map<int, unordered_set<string>>& answer, const unordered_map<string, int>& varToIndexTable) {
	list<int> stmtList;
	list<int> variableIndices;

	for (auto itr = answer.begin(); itr != answer.end(); itr++) {
		int stmtNum = itr->first;
		unordered_set<string> variableNames = itr->second;
		auto it = variableNames.begin();
		for (unsigned int i = 0; i < variableNames.size(); i++) {
			stmtList.push_back(stmtNum);
			variableIndices.push_back(varToIndexTable.at(*it));
			it++;
		}
	}
	unordered_map<string, list<int>> formattedAnswer({ {stmtSynonym, stmtList}, {variableSynonym, variableIndices} });
	this->answer = formattedAnswer;
}

void Result::setAnswer(const string& stmt1Synonym, const string& stmt2Synonym, const unordered_map<int, unordered_set<int>>& answer) {
	list<int> stmt1List;
	list<int> stmt2List;

	for (auto itr = answer.begin(); itr != answer.end(); itr++) {
		int stmt1Num = itr->first;
		unordered_set<int> stmt2Numbers = itr->second;
		auto it = stmt2Numbers.begin();
		for (unsigned int i = 0; i < stmt2Numbers.size(); i++) {
			stmt1List.push_back(stmt1Num);
			stmt2List.push_back(*it);
			it++;
		}
	}
	unordered_map<string, list<int>> formattedAnswer({ {stmt1Synonym, stmt1List}, {stmt2Synonym, stmt2List} });
	this->answer = formattedAnswer;
}

void Result::setAnswer(const string& stmtSynonym, const unordered_set<int>& stmtNums) {
	list<int> stmtList(stmtNums.begin(), stmtNums.end());
	unordered_map<string, list<int>> formattedAnswer({ {stmtSynonym, stmtList}});
	this->answer = formattedAnswer;
}

void Result::setAnswer(const string& stmtSynonym, const int& answer) {
	list<int> stmtList({ answer });
	unordered_map<string, list<int>> formattedAnswer({ {stmtSynonym, stmtList} });
	this->answer = formattedAnswer;
}

void Result::setAnswer(const string& stmt1Synonym, const string& stmt2Synonym, const unordered_map<int, int>& answer) {
	list<int> stmt1List;
	list<int> stmt2List;

	for (auto itr = answer.begin(); itr != answer.end(); itr++) {
		int stmt1Num = itr->first;
		int stmt2Num = itr->second;

		stmt1List.push_back(stmt1Num);
		stmt2List.push_back(stmt2Num);
	}
	unordered_map<string, list<int>> formattedAnswer({ {stmt1Synonym, stmt1List}, {stmt2Synonym, stmt2List} });
	this->answer = formattedAnswer;
}

void Result::setAnswer(const string& stmtSynonym, const string& variableSynonym, const unordered_map<int, string>& answer, const unordered_map<string, int>& varToIndexTable) {
	list<int> stmtList;
	list<int> variableIndices;

	for (auto itr = answer.begin(); itr != answer.end(); itr++) {
		int stmtNum = itr->first;
		string variable = itr->second;

		stmtList.push_back(stmtNum);
		variableIndices.push_back(varToIndexTable.at(variable));
	}
	unordered_map<string, list<int>> formattedAnswer({ {stmtSynonym, stmtList}, {variableSynonym, variableIndices} });
	this->answer = formattedAnswer;
}