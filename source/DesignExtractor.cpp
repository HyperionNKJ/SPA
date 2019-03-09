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
	return true;
}

bool DesignExtractor::insertProcCalledBy(string procName, int stmtNum) {
	procCalledByTable.insert({ procName, stmtNum });
	return true;
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
		//can populate pkb with proc info here
		for (const auto &elem : procModifiesTable[currProc]) {
			pkb->setModifies(currProc, elem);
		}
		for (const auto &elem : procUsesTable[currProc]) {
			pkb->setUses(currProc, elem);
		}
	}
	//next need to populate pkb with call statement uses/modifies
	//need to validate here the procedure exists
	// TODO: Abstract this out later
	for (const auto &elem : procCalledByTable) {
		string procName = elem.first;
		int stmtNum = elem.second;
		if (procList.count(procName) < 1) {
			cout << "Call to non-existent function detected at line " << stmtNum << endl;
			return false;
		}
		for (const auto &elem : procModifiesTable[procName]) {
			pkb->setModifies(stmtNum, elem);
		}
		for (const auto &elem : procUsesTable[procName]) {
			pkb->setUses(stmtNum, elem);
		}
	}
	//finally need to process calls*
	processCallsTransitive();
	return true;
}

bool DesignExtractor::topologicalSortCalls() {
	unordered_set<string> visitedProc;
	unordered_set<string> currentDFSVisited;
	for (const auto &elem : procList) {
		if (visitedProc.count(elem) < 1) {
			bool result = topologicalVisit(elem, visitedProc, currentDFSVisited);
			if (!result) {
				return false;
			}
		}
	}
	return true;
}

bool DesignExtractor::topologicalVisit(string procName, unordered_set<string> visitedProc, unordered_set<string> currentDFSVisited) {
	if (visitedProc.count(procName) > 0) {
		return true;
	}
	else if (currentDFSVisited.count(procName) > 0) {
		//not a DAG, since we point back to a node higher in this dfs search, return error due to cyclic calls
		cout << "Cyclic/Self calls in program are detected" << endl;
		return false;
	}
	else if (procList.count(procName) < 1) {
		//procedure called does not exist, return error
		cout << "Error: Call statement to non-existent procedure " << procName << "found" << endl;
		return false;
	}
	else {
		currentDFSVisited.insert(procName);
		for (const auto &nextProc : callGraph[procName]) {
			topologicalVisit(nextProc, visitedProc, currentDFSVisited);
		}
	}
	visitedProc.insert(procName);
	topoSortedProc.push_back(procName);
}

void DesignExtractor::processCallsTransitive() {
	//for each procedure, go through the calls graph with BFS and generate calls*. Not the most efficient but it will do.
	//then insert into PKB
	for (const auto &proc : procList) {
		queue<string> bfsQueue;
		for (const auto &calledProc : callGraph[proc]) {
			bfsQueue.push(calledProc);
		}
		while (bfsQueue.size() > 0) {
			string currCalledTProc = bfsQueue.front();
			bfsQueue.pop();
			//pkb->insertCallsT(proc, currCalledTProc);
			//pkb->insertCalledByT(currCalledTProc, proc);
			for (const auto &calledProc : callGraph[currCalledTProc]) {
				bfsQueue.push(calledProc);
			}
		}
	}
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

void DesignExtractor::setPKB(PKB * p) {
	pkb = p;
}