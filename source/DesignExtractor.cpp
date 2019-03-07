#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

#include "DesignExtractor.h"
#include "PKB.h"

bool DesignExtractor::insertCall(string procCalling, string procCalled) {
	if (callGraph.count(procCalling) < 1) {
		callGraph[procCalling] = { procCalled };
	}
	else {
		callGraph[procCalling].insert(procCalled);
	}
	return true;
}

bool DesignExtractor::insertProcUses(string procName, string varName) {
	if (procUsesTable.count(procName) < 1) {
		procUsesTable[procName] = { varName };
	}
	else {
		procUsesTable[procName].insert(varName);
	}
	return true;
}

bool DesignExtractor::insertProcModifies(string procName, string varName) {
	if (procModifiesTable.count(procName) < 1) {
		procModifiesTable[procName] = { varName };
	}
	else {
		procModifiesTable[procName].insert(varName);
	}
}

bool DesignExtractor::insertProcCalledBy(string procName, int stmtNum) {
	procCalledByTable.insert({ procName, stmtNum });
}

void DesignExtractor::processCalls() {
	topologicalSortCalls();
	for (int i = 0; i < topologicalSortedProc.size(); i++) {
		string currProc = topologicalSortedProc[i];
		unordered_set<string> calledByProc = callGraph[currProc];
		for (const auto &proc : calledByProc) {
			setProcModifies(currProc, proc);
			setProcUses(currProc, proc);
		}
	}
}

void DesignExtractor::topologicalSortCalls() {

}

void DesignExtractor::setProcModifies(string procToUpdate, string procCalled) {
	for (const auto &elem : procModifiesTable[procCalled]) {
		procModifiesTable[procToUpdate].insert(elem);
	}
}

void DesignExtractor::setProcUses(string procToUpdate, string procCalled) {
	for (const auto &elem : procUsesTable[procCalled]) {
		procUsesTable[procToUpdate].insert(elem);
	}
}