#pragma once

#include <stdio.h>
#include <iostream>
#include <vector>
#include <string>
#include <stack>
#include <queue>
#include "PKB.h"
#include "Type.h"

class DesignExtractor {
	PKB * pkb;
	unordered_set<string> procList;
	unordered_map<string, unordered_set<string>> callGraph;
	unordered_map<string, unordered_set<string>> procUsesTable;
	unordered_map<string, unordered_set<string>> procModifiesTable;
	vector<string> topoSortedProc;

private:
	bool topologicalSortCalls();
	bool topologicalVisit(string, unordered_set<string>*, unordered_set<string>*);
	void updateProcModifies(string, string);
	void updateProcUses(string, string);
public:
	bool insertCall(string, string);
	bool insertProc(string);
	bool insertProcUses(string, string);
	bool insertProcModifies(string, string);
	void setPKB(PKB*);

	bool processCalls();

	unordered_map<string, unordered_set<string>> getProcUsesTable();
	unordered_map<string, unordered_set<string>> getProcModifiesTable();
	unordered_map<string, unordered_set<string>> getCallGraph();
	unordered_set<string> getProcList();
};