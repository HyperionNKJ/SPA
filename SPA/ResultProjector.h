#pragma once

#include <stdio.h>
#include <iostream>
#include <unordered_map> 

using namespace std;

class ResultProjector {
private:
	bool synonymExists(string);
	void addOneSyn(string, unordered_map<string, list<int>>);
	void addTwoSyn(string, string);
	void filterSyn(string, list<int>);
	bool indexDoesNotExist(int, list<int>);
	void eraseTableRow(int, unordered_map<string, list<int>>&, string);
	void mergeOneSyn(string, string newKey);
	void mergeDiffTable(string, string);

public:
	void resetResults();
	bool combineResults(unordered_map<string, list<int>> queryResults);
	list<string> getResults(string selectedSynonym);
	void printTables(); // for debugging
	unordered_map<string, int> getSynonymTable();
	unordered_map<int, unordered_map<string, list<int>>> getSynonymResults();

};
