#pragma once

#include <stdio.h>
#include <iostream>

#include "DesignExtractor.h"

bool DesignExtractor::insertProc(std::string procName) {
	return procList.insert(procName).second;
	return true;
}

bool DesignExtractor::insertCall(std::string procCalling, std::string procCalled) {
	if (callGraph.count(procCalling) < 1) {
		callGraph[procCalling] = { procCalled };
		return true;
	}
	return callGraph[procCalling].insert(procCalled).second;
}

bool DesignExtractor::insertProcUses(std::string procName, std::string varName) {
	if (procUsesTable.count(procName) < 1) {
		procUsesTable[procName] = { varName };
		return true;
	}
	return procUsesTable[procName].insert(varName).second;
}

bool DesignExtractor::insertProcModifies(std::string procName, std::string varName) {
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
		std::string currProc = topoSortedProc[i];
		std::unordered_set<std::string> calledByProc = callGraph[currProc];
		for (const auto &proc : calledByProc) {
			updateProcModifies(currProc, proc);
			updateProcUses(currProc, proc);
		}
	}
	return true;
}

bool DesignExtractor::topologicalSortCalls() {
	std::unordered_set<std::string> visitedProc;
	std::unordered_set<std::string> currentDFSVisited;
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

bool DesignExtractor::topologicalVisit(std::string procName, std::unordered_set<std::string>* visitedProc, std::unordered_set<std::string>* currentDFSVisited) {
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

void DesignExtractor::updateProcModifies(std::string procToUpdate, std::string procCalled) {
	for (const auto &elem : procModifiesTable[procCalled]) {
		procModifiesTable[procToUpdate].insert(elem);
	}
}

void DesignExtractor::updateProcUses(std::string procToUpdate, std::string procCalled) {
	for (const auto &elem : procUsesTable[procCalled]) {
		procUsesTable[procToUpdate].insert(elem);
	}
}

std::unordered_map<std::string, std::unordered_set<std::string>> DesignExtractor::getProcUsesTable() {
	return procUsesTable;
}

std::unordered_map<std::string, std::unordered_set<std::string>> DesignExtractor::getProcModifiesTable() {
	return procModifiesTable;
}

std::unordered_map<std::string, std::unordered_set<std::string>> DesignExtractor::getCallGraph() {
	return callGraph;
}

std::unordered_set<std::string> DesignExtractor::getProcList() {
	return procList;
}

std::string DesignExtractor::getErrorMessage() {
	return errorMessage;
}