#pragma once

#include "DesignEntity.h"
#include "PKB.h"
#include <stdio.h>
#include <iostream>
#include <unordered_set>
#include <unordered_map> 

using namespace std;

class ResultProjector {
private:
	bool synonymExists(string);
	void addOneSyn(string, unordered_map<string, list<int>>);
	void addTwoSyn(string, string, unordered_map<string, list<int>>);
	void filterSynInTable(string, list<int>);
	void cleanUpTables(string);
	void filterOldTable(int, string);
	bool resultExists(int, list<int>);
	void eraseTableRow(int, unordered_map<string, list<int>>&, string);
	void mergeOneSyn(string, string, unordered_map<string, list<int>>);
	void duplicateResultsForRestOfTable(int, unordered_map<string, list<int>>&);
	int findIndex(int, list<int>);
	int getCorrespondingResult(int, list<int>);
	int getAndDeleteCorrespondingResult(int, string, string, unordered_map<string, list<int>>&);
	void combineDependentResults(string, string, unordered_map<string, list<int>>);
	bool dependentResultExists(string, string, int, int, unordered_map<string, list<int>>);
	void mergeTables(int, string, list<int>);

	list<string> getSelectedClauseInTable(Type, list<int>, PKB);
	list<string> getSelectedClauseNotInTable(Type, PKB);
	list<string> convertVarIndexToVar(unordered_set<int>, PKB);
	list<string> convertVarIndexToVar(list<int>, PKB);
	list<string> convertProcIndexToProc(unordered_set<int>, PKB);
	list<string> convertProcIndexToProc(list<int>, PKB);
	list<string> convertIndexToString(unordered_set<int>, PKB);
	list<string> convertIndexToString(list<int>, PKB);
	list<string> convertSetToList(unordered_set<string>);

public:
	void resetResults();
	bool combineResults(unordered_map<string, list<int>> queryResults);
	list<string> getResults(DesignEntity selectedSynonym, PKB pkb);
	void printTables(); // for debugging
	unordered_map<string, int> getSynonymTable(); // for testing
	unordered_map<int, unordered_map<string, list<int>>> getSynonymResults(); // for testing

};
