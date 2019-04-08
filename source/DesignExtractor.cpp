#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>

#include "DesignExtractor.h"
#include "PKB.h"

bool DesignExtractor::insertProc(string procName) {
	return procList.insert(procName).second;
	return true;
}

bool DesignExtractor::insertCall(string procCalling, string procCalled) {
	if (callGraph.count(procCalling) < 1) {
		callGraph[procCalling] = { procCalled };
		return true;
	}
	return callGraph[procCalling].insert(procCalled).second;
}

bool DesignExtractor::insertProcUses(string procName, string varName) {
	if (procUsesTable.count(procName) < 1) {
		procUsesTable[procName] = { varName };
		return true;
	}
	return procUsesTable[procName].insert(varName).second;
}

bool DesignExtractor::insertProcModifies(string procName, string varName) {
	if (procModifiesTable.count(procName) < 1) {
		procModifiesTable[procName] = { varName };
		return true;
	}
	return procModifiesTable[procName].insert(varName).second;
}

bool DesignExtractor::processCalls() {
	bool result = topologicalSortCalls();
	if (!result) {
		return false;
	}
	for (unsigned int i = 0; i < topoSortedProc.size(); i++) {
		string currProc = topoSortedProc[i];
		unordered_set<string> calledByProc = callGraph[currProc];
		for (const auto &proc : calledByProc) {
			updateProcModifies(currProc, proc);
			updateProcUses(currProc, proc);
		}
	}
	return true;
}

bool DesignExtractor::topologicalSortCalls() {
	unordered_set<string> visitedProc;
	unordered_set<string> currentDFSVisited;
	for (const auto &elem : procList) {
		if (visitedProc.count(elem) < 1) {
			bool result = topologicalVisit(elem, &visitedProc, &currentDFSVisited);
			if (!result) {
				return false;
			}
		}
	}
	return true;
}

bool DesignExtractor::topologicalVisit(string procName, unordered_set<string>* visitedProc, unordered_set<string>* currentDFSVisited) {
	if (visitedProc->count(procName) > 0) {
		return true;
	}
	else if (currentDFSVisited->count(procName) > 0) {
		//not a DAG, since we point back to a node higher in this dfs search, return error due to cyclic calls
		errorMessage = "Cyclic/Self calls in program are detected";
		return false;
	}
	else if (procList.count(procName) < 1) {
		//procedure called does not exist, return error
		errorMessage = "Call statement to non-existent procedure " + procName + " found";
		return false;
	}
	else {
		currentDFSVisited->insert(procName);
		for (const auto &nextProc : callGraph[procName]) {
			bool result = topologicalVisit(nextProc, visitedProc, currentDFSVisited);
			if (!result) {
				return false;
			}
		}
	}
	visitedProc->insert(procName);
	topoSortedProc.push_back(procName);
	return true;
}

void DesignExtractor::updateProcModifies(string procToUpdate, string procCalled) {
	for (const auto &elem : procModifiesTable[procCalled]) {
		procModifiesTable[procToUpdate].insert(elem);
	}
}

void DesignExtractor::updateProcUses(string procToUpdate, string procCalled) {
	for (const auto &elem : procUsesTable[procCalled]) {
		procUsesTable[procToUpdate].insert(elem);
	}
}

unordered_map<string, unordered_set<string>> DesignExtractor::getProcUsesTable() {
	return procUsesTable;
}

unordered_map<string, unordered_set<string>> DesignExtractor::getProcModifiesTable() {
	return procModifiesTable;
}

unordered_map<string, unordered_set<string>> DesignExtractor::getCallGraph() {
	return callGraph;
}

unordered_set<string> DesignExtractor::getProcList() {
	return procList;
}

string DesignExtractor::getErrorMessage() {
	return errorMessage;
}