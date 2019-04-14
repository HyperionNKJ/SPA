#pragma once

#include <stdio.h>
#include <iostream>
#include <vector>
#include <string>
#include <stack>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include "Type.h"

class DesignExtractor {
	std::unordered_set<std::string> procList;
	std::unordered_map<std::string, std::unordered_set<std::string>> callGraph;
	std::unordered_map<std::string, std::unordered_set<std::string>> procUsesTable;
	std::unordered_map<std::string, std::unordered_set<std::string>> procModifiesTable;
	std::vector<std::string> topoSortedProc;
	std::string errorMessage;

private:
	bool topologicalSortCalls();
	bool topologicalVisit(std::string, std::unordered_set<std::string>*, std::unordered_set<std::string>*);
	void updateProcModifies(std::string, std::string);
	void updateProcUses(std::string, std::string);
public:
	bool insertCall(std::string, std::string);
	bool insertProc(std::string);
	bool insertProcUses(std::string, std::string);
	bool insertProcModifies(std::string, std::string);

	bool processCalls();

	std::unordered_map<std::string, std::unordered_set<std::string>> getProcUsesTable();
	std::unordered_map<std::string, std::unordered_set<std::string>> getProcModifiesTable();
	std::unordered_map<std::string, std::unordered_set<std::string>> getCallGraph();
	std::unordered_set<std::string> getProcList();
	std::string getErrorMessage();
};