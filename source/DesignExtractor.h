#pragma once

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <regex>
#include <vector>
#include <string>
#include <stack>
#include "PKB.h"
#include "Type.h"

using namespace std;

class DesignExtractor {
	unordered_map<string, unordered_set<string>> callGraph;
	unordered_map<string, unordered_set<string>> procUsesTable;
	unordered_map<string, unordered_set<string>> procModifiesTable;
	vector<string> topologicalSortedProc;
	unordered_map<string, int> procCalledByTable;

public:
	bool insertCall(string, string);
	bool insertProcUses(string, string);
	bool insertProcModifies(string, string);
	bool insertProcCalledBy(string, int);
	void processCalls();
	void topologicalSortCalls();

	void setProcModifies(string, string);
	void setProcUses(string, string);
	void updatePKBCall(string, int);
};