#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <ctype.h>
#include <unordered_set>
#include <unordered_map>

#include "PKB.h"
#include "Type.h"

using namespace std;

// Frontend APIs
bool PKB::insertVar(string varName) {
	if (!varSet.count(varName)) {
		varSet.insert(varName);
		varTableByIdx.push_back(varName);
		varTableByName.insert({varName, varTableByIdx.size() - 1});
		return true;
	}
	return false;
}

bool PKB::insertConstant(int constant) {
	return constSet.insert(constant).second;
}

bool PKB::insertProc(string procName) {
	if (!procTableByName.count(procName)) {
		procSet.insert(procName);
		procTableByIdx.push_back(procName);
		procTableByName.insert({procName, procTableByIdx.size() - 1});
		return true;
	}
	return false;
}

unordered_set<int> PKB::getTypedStmtSet(Type type) {
	unordered_set<int> typedStmtSet;

	switch (type) {
	case STATEMENT:
	case PROGLINE:
		typedStmtSet = allStmts;
		break;
	case READ:
		typedStmtSet = readStmts;
		break;
	case PRINT:
		typedStmtSet = printStmts;
		break;
	case WHILE:
		typedStmtSet = whileStmts;
		break;
	case IF:
		typedStmtSet = ifStmts;
		break;
	case ASSIGN:
		typedStmtSet = assignStmts;
		break;
	case CALL:
		typedStmtSet = callStmts;
		break;
	}

	return typedStmtSet;
}

bool PKB::insertStmtType(int stmtNum, Type type) {
	unordered_set<int> typedStmtSet;
	switch (type) {
	case STATEMENT:
		return allStmts.insert(stmtNum).second;
	case READ:
		typedStmtSet = readStmts;
		break;
	case WHILE:
		typedStmtSet = whileStmts;
		break;
	case IF:
		typedStmtSet = ifStmts;
		break;
	case ASSIGN:
		typedStmtSet = assignStmts;
		break;
	case PRINT:
		typedStmtSet = printStmts;
		break;
	// wrong type
	default:
		return false;
	}
	return typedStmtSet.insert(stmtNum).second && allStmts.insert(stmtNum).second;
}

bool PKB::insertCPRStmtType(int stmtNum, Type type, string name) {
	bool isValidStmt;
	unordered_set<string> typedNameSet;
	unordered_set<int> typedStmtSet;
	unordered_map<string, int> typedTable;
	vector<string> typedVector; 

	switch(type) {
		case CALL:
			typedNameSet = calledSet;
			typedStmtSet = callStmts;
			typedTable = callTableByName;
			typedVector = callTableByIdx;
			break;
		case PRINT:
			typedNameSet = printSet;
			typedStmtSet = printStmts;
			typedTable = printTableByName;
			typedVector = printTableByIdx;
			break;
		case READ:
			typedNameSet = readSet;
			typedStmtSet = readStmts;
			typedTable = readTableByName;
			typedVector = readTableByIdx;
			break;
		default:
			return false;
		isValidStmt = allStmts.insert(stmtNum).second && typedStmtSet.insert(stmtNum).second;
	}
	if (!isValidStmt)
		return false;

	if (!typedNameSet.count(name)) {
		typedNameSet.insert(name);
		typedVector.push_back(name);
		typedTable.insert({name, typedVector.size() - 1});
		return true;
	}
	return false;
}

bool PKB::setFollows(int leader, int follower) {
	return leaderMap.insert({leader, follower}).second;
}

bool PKB::setFollowsT(int leader, int follower) {
	return leaderTMap[leader].insert(follower).second;
}

bool PKB::setFollowedBy(int leader, int follower) {
	return followerMap.insert({follower, leader}).second;
}

bool PKB::setFollowedByT(int leader, int follower) {
	return followerTMap[follower].insert(leader).second;
}

bool PKB::setParent(int parent, int child) {
	return parentMap[parent].insert(child).second;
}

bool PKB::setParentT(int parent, int child) {
	return parentTMap[parent].insert(child).second;
}

bool PKB::setChild(int parent, int child) {
	return childrenMap.insert({child, parent}).second;
}

bool PKB::setChildT(int parent, int child) {
	return childrenTMap[child].insert(parent).second;
}

bool PKB::setModifies(int stmtNum, string varName) {
	modifiesByStmtNumMap[stmtNum].insert(varName);
	return modifiesByVarMap[varName].insert(stmtNum).second;
}

bool PKB::setModifies(string procName, string varName) {
	modifiesProcSet.insert(procName);
	varModifiedByProcMap[varName].insert(procName);
	return modifiesByProcMap[procName].insert(varName).second;
}

bool PKB::setUses(int stmtNum, string varName) {
	usesByStmtNumMap[stmtNum].insert(varName);
	return usesByVarMap[varName].insert(stmtNum).second;
}

bool PKB::setUses(string procName, string varName) {
	usesProcSet.insert(procName);
	varUsedByProcMap[varName].insert(procName);
	return usesByProcMap[procName].insert(varName).second;
}

bool PKB::insertPattern(string pattern, int stmtNum) {
	return patternMap[pattern].insert(stmtNum).second;
}

bool PKB::insertFullPattern(string fullPattern, int stmtNum) {
	return fullPatternMap[fullPattern].insert(stmtNum).second;
}

bool PKB::setCalls(string proc1, string proc2) {
	callSet.insert(proc1);
	return callMap[proc1].insert(proc2).second;
}

bool PKB::setCalledBy(string proc1, string proc2) {
	return calledByMap[proc2].insert(proc1).second;
}

bool PKB::setCallsT(string proc1, string proc2) {
	return callsTMap[proc1].insert(proc2).second;
}

bool PKB::setCalledByT(string proc1, string proc2) {
	return calledByTMap[proc2].insert(proc1).second;
}

bool PKB::setNext(int prevLine, int nextLine) {
	return nextMap[prevLine].insert(nextLine).second;
}

bool PKB::setPrevious(int prevLine, int nextLine) {
	return prevMap[nextLine].insert(prevLine).second;
}

// PQL APIs
unordered_set<int> PKB::getAllStmts() {
	return allStmts;
}

unordered_set<int> PKB::getReadStmts() {
	return readStmts;
}

unordered_set<int> PKB::getPrintStmts() {
	return printStmts;
}

unordered_set<int> PKB::getWhileStmts() {
	return whileStmts;
}

unordered_set<int> PKB::getIfStmts() {
	return ifStmts;
}

unordered_set<int> PKB::getAssignStmts() {
	return assignStmts;
}

unordered_set<string> PKB::getAllVariables() {
	return varSet;
}

unordered_set<int> PKB::getAllConstant() {
	return constSet;
}

unordered_set<string> PKB::getAllProcedures() {
	return procSet;
}

unordered_map<string, int> PKB::getVarTable() {
	return varTableByName;
}

unordered_set<int> PKB::getCallStmts() {
	return callStmts;
}

unordered_set<string> PKB::getCallProcNames() {
	return calledSet;
}

unordered_set<string> PKB::getPrintVarNames() {
	return printSet;
}

unordered_set<string> PKB::getReadVarNames() {
	return readSet;
}

bool PKB::isReadStmt(int stmtNum) {
	if (readStmts.count(stmtNum)) {
		return true;
	}
	return false;
}

bool PKB::isWhileStmt(int stmtNum) {
	if (whileStmts.count(stmtNum)) {
		return true;
	}
	return false;
}

bool PKB::isIfStmt(int stmtNum) {
	if (ifStmts.count(stmtNum)) {
		return true;
	}
	return false;
}

bool PKB::isAssignStmt(int stmtNum) {
	if (assignStmts.count(stmtNum)) {
		return true;
	}
	return false;
}

bool PKB::isPrintStmt(int stmtNum) {
	if (printStmts.count(stmtNum)) {
		return true;
	}
	return false;
}

bool PKB::isCallStmt(int stmtNum) {
	if (callStmts.count(stmtNum)) {
		return true;
	}
	return false;
}

int PKB::getVarIdx(string varName) {
	return varTableByName[varName];
}

string PKB::getVarAtIdx(int varIdx) {
	return varTableByIdx[varIdx];
}

unordered_map<string, int> PKB::getVarTable() {
	return varTableByName;
}

int PKB::getProcIdx(string procName) {
	return procTableByName[procName];
}

string PKB::getProcAtIdx(int procIdx) {
	return procTableByIdx[procIdx];
}

unordered_map<string, int> PKB::getProcTable() {
	return procTableByName;
}

int PKB::getCallIdx(string procName) {
	return callTableByName[procName];
}

string PKB::getCallAtIdx(int callIdx) {
	return callTableByIdx[callIdx];
}

int PKB::getReadIdx(string varName) {
	return readTableByName[varName];
}

string PKB::getReadAtIdx(int readIdx) {
	return readTableByIdx[readIdx];
}

int PKB::getPrintIdx(string varName) {
	return printTableByName[varName];
}

string PKB::getPrintAtIdx(int printIdx) {
	return printTableByIdx[printIdx];
}

bool PKB::isModifies(int stmtNum, string varName) {
	if (allStmts.count(stmtNum)) {
		if (modifiesByStmtNumMap[stmtNum].count(varName))
			return true;
	}
	return false;
}

bool PKB::doesStmtModifies(int stmtNum) {
	if (allStmts.count(stmtNum)) {
		if (modifiesByStmtNumMap[stmtNum].size())
			return true;
	}
	return false;
}

unordered_set<string> PKB::getVarModifiedByStmt(int stmtNum) {
	return modifiesByStmtNumMap[stmtNum];
}

unordered_set<int> PKB::getStmtsThatModifiesVar(string varName, Type type) {
	unordered_set<int> allModifyingStmts = modifiesByVarMap[varName], typedStmtSet = getTypedStmtSet(type), resultSet;

	for (const auto &elem : allModifyingStmts) {
		if (typedStmtSet.count(elem)) {
			resultSet.insert(elem);
		}
	}

	return resultSet;
}

unordered_map<int, unordered_set<string>> PKB::getModifiesStmtVarPairs(Type type) {
	unordered_set<int> typedStmtSet = getTypedStmtSet(type);
	unordered_map<int, unordered_set<string>> resultSet;

	for (const auto &elem : typedStmtSet)
		resultSet.insert({elem, modifiesByStmtNumMap[elem]});

	return resultSet;
}

unordered_set<int> PKB::getStmtsThatModifiesVar(Type type) {
	unordered_set<int> typedStmtSet = getTypedStmtSet(type);
	unordered_set<int> resultSet;

	for (const auto &elem : typedStmtSet) {
		if (modifiesByStmtNumMap.count(elem))
			resultSet.insert(elem);
	}

	return resultSet;
}

bool PKB::isProcModifies(string procName, string varName) {
	if (modifiesByProcMap.count(procName) && modifiesByProcMap[procName].count(varName))
		return true;
	return false;
}

bool PKB::doesProcModifies(string procName) {
	if (modifiesByProcMap.count(procName))
		return true;
	return false;
}

unordered_set<string> PKB::getVarModifiedByProc(string procName) {
	return modifiesByProcMap[procName];
}

unordered_map<string, unordered_set<string>> PKB::getModifiesProcVarPairs() {
	return modifiesByProcMap;
}

unordered_set<string> PKB:: getProcThatModifiesVar() {
	return modifiesProcSet;
}

unordered_set<string> PKB::getProcThatModifiesVar(string varName) {
	return varModifiedByProcMap[varName];
}

bool PKB::isUses(int stmtNum, string varName) {
	if (allStmts.count(stmtNum)) {
		if (usesByStmtNumMap[stmtNum].count(varName))
			return true;
	}
	return false;
}

bool PKB::doesStmtUses(int stmtNum) {
	if (allStmts.count(stmtNum)) {
		if (usesByStmtNumMap[stmtNum].size())
			return true;
	}
	return false;
}

unordered_set<string> PKB::getVarUsedByStmt(int stmtNum) {
	return usesByStmtNumMap[stmtNum];
}

unordered_set<int> PKB::getStmtsThatUsesVar(string varName, Type type) {
	unordered_set<int> allUsesStmts = usesByVarMap[varName], typedStmtSet = getTypedStmtSet(type), resultSet;

	for (const auto &elem : allUsesStmts) {
		if (typedStmtSet.count(elem)) {
			resultSet.insert(elem);
		}
	}

	return resultSet;
}

unordered_map<int, unordered_set<string>> PKB::getUsesStmtVarPairs(Type type) {
	unordered_set<int> typedStmtSet = getTypedStmtSet(type);
	unordered_map<int, unordered_set<string>> resultSet;

	for (const auto &elem : typedStmtSet)
		resultSet.insert({elem, usesByStmtNumMap[elem]});

	return resultSet;
}

unordered_set<int> PKB::getStmtsThatUsesVar(Type type) {
	unordered_set<int> typedStmtSet = getTypedStmtSet(type);
	unordered_set<int> resultSet;

	for (const auto &elem : typedStmtSet) {
		if (usesByStmtNumMap.count(elem))
			resultSet.insert(elem);
	}

	return resultSet;
}

bool PKB::isProcUses(string procName, string varName) {
	if (usesByProcMap.count(procName) && usesByProcMap[procName].count(varName))
		return true;
	return false;
}

bool PKB::doesProcUses(string procName) {
	if (usesByProcMap.count(procName))
		return true;
	return false;
}

unordered_set<string> PKB::getVarUsedByProc(string procName) {
	return usesByProcMap[procName];
}

unordered_map<string, unordered_set<string>> PKB::getUsesProcVarPairs() {
	return usesByProcMap;
}

unordered_set<string> PKB::getProcThatUsesVar() {
	return usesProcSet;
}

unordered_set<string> PKB::getProcThatUsesVar(string varName) {
	return varUsedByProcMap[varName];
}

bool PKB::isParent(int stmtNum1, int stmtNum2) {
	if (parentMap.count(stmtNum1) && parentMap[stmtNum1].count(stmtNum2))
		return true;
	return false;
}

bool PKB::isParentT(int stmtNum1, int stmtNum2) {
	if (parentTMap.count(stmtNum1) && parentTMap[stmtNum1].count(stmtNum2))
		return true;
	return false;
}

bool PKB::hasChildren(int stmtNum) {
	if (parentMap.count(stmtNum))
		return true;
	return false;
}

bool PKB::hasParent(int stmtNum) {
	if (childrenMap.count(stmtNum))
		return true;
	return false;
}

unordered_map<int, unordered_set<int>> PKB::getParentChildrenPairs(Type parentType, Type childrenType) {
	unordered_set<int> parentTypedStmtSet = getTypedStmtSet(parentType), childrenTypedStmtSet = getTypedStmtSet(childrenType);
	unordered_map<int, unordered_set<int>> resultMap;

	for (const auto &elem : parentTypedStmtSet) {
		if (parentMap.count(elem)) {
			for (const auto &elem2 : parentMap[elem]) {
				if (childrenTypedStmtSet.count(elem2)) {
					resultMap[elem].insert(elem2);
				}
			}
		}
	}

	return resultMap;
}

unordered_map<int, unordered_set<int>> PKB::getParentChildrenTPairs(Type parentType, Type childrenType) {
	unordered_set<int> parentTypedStmtSet = getTypedStmtSet(parentType), childrenTypedStmtSet = getTypedStmtSet(childrenType);
	unordered_map<int, unordered_set<int>> resultMap;

	for (const auto &elem : parentTypedStmtSet) {
		if (parentTMap.count(elem)) {
			for (const auto &elem2 : parentTMap[elem]) {
				if (childrenTypedStmtSet.count(elem2)) {
					resultMap[elem].insert(elem2);
				}
			}
		}
	}

	return resultMap;
}

unordered_set<int> PKB::getParentStmts(Type parentType) {
	unordered_set<int> typedStmtSet = getTypedStmtSet(parentType), resultSet;

	for (const auto &elem : parentMap) {
		if (typedStmtSet.count(elem.first)) {
			resultSet.insert(elem.first);
		}
	}

	return resultSet;
}

unordered_set<int> PKB::getChildrenStmts(Type childrenType) {
	unordered_set<int> typedStmtSet = getTypedStmtSet(childrenType), resultSet;

	for (const auto &elem : childrenMap) {
		if (typedStmtSet.count(elem.first)) {
			resultSet.insert(elem.first);
		}
	}

	return resultSet;
}

int PKB::getParentOf(int stmtNum, Type parentType) {
	unordered_set<int> typedStmtSet = getTypedStmtSet(parentType);

	if (childrenMap.count(stmtNum) && typedStmtSet.count(childrenMap[stmtNum])) {
		return childrenMap[stmtNum];
	}
	return -1;
}

unordered_set<int> PKB::getParentTOf(int stmtNum, Type parentType) {
	unordered_set<int> typedStmtSet = getTypedStmtSet(parentType), resultSet;

	if (childrenTMap.count(stmtNum)) {
		for (const auto &elem : childrenTMap[stmtNum]) {
			if (typedStmtSet.count(elem)) {
				resultSet.insert(elem);
			}
		}
	}

	return resultSet;
}

unordered_set<int> PKB::getChildrenOf(int stmtNum, Type childrenType) {
	unordered_set<int> typedStmtSet = getTypedStmtSet(childrenType), resultSet;

	if (parentMap.count(stmtNum)) {
		for (const auto &elem : parentMap[stmtNum]) {
			if (typedStmtSet.count(elem)) {
				resultSet.insert(elem);
			}
		}
	}

	return resultSet;
}

unordered_set<int> PKB::getChildrenTOf(int stmtNum, Type childrenType) {
	unordered_set<int> typedStmtSet = getTypedStmtSet(childrenType), resultSet;

	if (parentTMap.count(stmtNum)) {
		for (const auto &elem : parentTMap[stmtNum]) {
			if (typedStmtSet.count(elem)) {
				resultSet.insert(elem);
			}
		}
	}

	return resultSet;
}

bool PKB::isFollows(int stmtNum1, int stmtNum2) {
	if (leaderMap.count(stmtNum1) && leaderMap[stmtNum1] == stmtNum2)
		return true;
	return false;
}

bool PKB::isFollowsT(int stmtNum1, int stmtNum2) {
	if (leaderTMap.count(stmtNum1) && leaderTMap[stmtNum1].count(stmtNum2))
		return true;
	return false;
}

bool PKB::hasFollower(int stmtNum) {
	if (leaderMap.count(stmtNum))
		return true;
	return false;
}

bool PKB::hasLeader(int stmtNum) {
	if (followerMap.count(stmtNum))
		return true;
	return false;
}

unordered_map<int, int> PKB::getLeaderFollowerPairs(Type leaderType, Type followerType) {
	unordered_set<int> leaderTypedStmtSet = getTypedStmtSet(leaderType), followerTypedStmtSet = getTypedStmtSet(followerType);
	unordered_map<int, int> resultMap;

	for (const auto &elem : leaderTypedStmtSet) {
		if (leaderMap.count(elem) && followerTypedStmtSet.count(leaderMap[elem]))
			resultMap.insert({elem, leaderMap[elem]});
	}

	return resultMap;
}

unordered_map<int, unordered_set<int>> PKB::getLeaderFollowerTPairs(Type leaderType, Type followerType) {
	unordered_set<int> leaderTypedStmtSet = getTypedStmtSet(leaderType), followerTypedStmtSet = getTypedStmtSet(followerType);
	unordered_map<int, unordered_set<int>> resultMap;

	for (const auto &elem : leaderTypedStmtSet) {
		if (leaderTMap.count(elem)) {
			for (const auto &elem2 : leaderTMap[elem]) {
				if (followerTypedStmtSet.count(elem2)){
					resultMap[elem].insert(elem2);
				}
			}
		}
	}

	return resultMap;
}

unordered_set<int> PKB::getLeaderStmts(Type leaderType) {
	unordered_set<int> typedStmtSet = getTypedStmtSet(leaderType), resultSet;

	for (const auto &elem : leaderMap) {
		if (typedStmtSet.count(elem.first)) {
			resultSet.insert(elem.first);
		}
	}

	return resultSet;
}

unordered_set<int> PKB::getFollowerStmts(Type followerType) {
	unordered_set<int> typedStmtSet = getTypedStmtSet(followerType), resultSet;

	for (const auto &elem : followerMap) {
		if (typedStmtSet.count(elem.first)) {
			resultSet.insert(elem.first);
		}
	}

	return resultSet;
}

int PKB::getLeaderOf(int stmtNum, Type leaderType) {
	unordered_set<int> typedStmtSet = getTypedStmtSet(leaderType);

	if (followerMap.count(stmtNum) && typedStmtSet.count(followerMap[stmtNum])) {
		return followerMap[stmtNum];
	}
	return -1;
}

unordered_set<int> PKB::getLeaderTOf(int stmtNum, Type leaderType) {
	unordered_set<int> typedStmtSet = getTypedStmtSet(leaderType), resultSet;

	if (followerTMap.count(stmtNum)) {
		for (const auto &elem : followerTMap[stmtNum]) {
			if (typedStmtSet.count(elem)) {
				resultSet.insert(elem);
			}
		}
	}

	return resultSet;
}

int PKB::getFollowerOf(int stmtNum, Type followerType) {
	unordered_set<int> typedStmtSet = getTypedStmtSet(followerType);
	int result = -1;

	if (leaderMap.count(stmtNum) && typedStmtSet.count(leaderMap[stmtNum]))
		result = leaderMap[stmtNum];

	return result;
}

unordered_set<int> PKB::getFollowerTOf(int stmtNum, Type followerType) {
	unordered_set<int> typedStmtSet = getTypedStmtSet(followerType), resultSet;

	if (leaderTMap.count(stmtNum)) {
		for (const auto &elem : leaderTMap[stmtNum]) {
			if (typedStmtSet.count(elem)) {
				resultSet.insert(elem);
			}
		}
	}

	return resultSet;
}

string PKB::getVarModifiedByAssignStmt(int stmtNum) {
	if (modifiesByStmtNumMap.count(stmtNum) && isAssignStmt(stmtNum)) {
		return *modifiesByStmtNumMap[stmtNum].begin();
	}
	return "";
}

unordered_set<int> PKB::getAssignStmtsThatModifiesVar(string varName) {
	return modifiesByVarMap[varName];
}

unordered_set<int> PKB::getAssignStmtsWithSubMatch(string subString) {
	return patternMap[subString];
}

unordered_set<int> PKB::getAssignStmtsWithExactMatch(string exactString) {
	return fullPatternMap[exactString];
}

bool PKB::isCalls(string callerName, string receiverName) {
	if (callMap.count(callerName) && callMap[callerName].count(receiverName))
		return true;
	return false;
}

bool PKB::isCallsT(string callerName, string receiverName) {
	if (callsTMap.count(callerName) && callsTMap[callerName].count(receiverName))
		return true;
	return false;
}

bool PKB::hasReceiver(string callerName) {
	if (callMap.count(callerName))
		return true;
	return false;
}

bool PKB::hasCaller(string receiverName) {
	if (calledByMap.count(receiverName))
		return true;
	return false;
}

unordered_map<string, unordered_set<string>> PKB::getCallerReceiverPairs() {
	return callMap;
}

unordered_map<string, unordered_set<string>> PKB::getCallerReceiverTPairs() {
	return callsTMap;
}

unordered_set<string> PKB::getCallerProcedures() {
	return callSet;
}

unordered_set<string> PKB::getReceiverProcedures() {
	return calledSet;
}

unordered_set<string> PKB::getCallerOf(string receiverName) {
	return calledByMap[receiverName];
}

unordered_set<string> PKB::getCallerTOf(string receiverName) {
	return calledByTMap[receiverName];
}

unordered_set<string> PKB::getReceiverOf(string callerName) {
	return callMap[callerName];
}

unordered_set<string> PKB::getReceiverTOf(string callerName) {
	return callsTMap[callerName];
}

/******** Alvin's methods for Next* *************/

bool PKB::isNextT(int firstLine, int secondLine) {
	unordered_set<int> visitedLines;
	unordered_set<int> currentNextLines;
	int toExplore = firstLine;
	bool found = false;
	while (!found) {
		if (nextMap.count(toExplore) > 0) {
			int nextExplore = -1;
			currentNextLines = nextMap[firstLine];
			for (auto &elem : currentNextLines) {
				if (elem > nextExplore && elem <= secondLine) {
					nextExplore = elem;
				}
			}
			toExplore = nextExplore;
			if (toExplore == secondLine) {
				found = true;
			}
		}
		else {
			found = false;
			break;
		}
	}
	return found;
}

unordered_set<int> PKB::getNextTOf(int firstLine, Type stmtType) {
	unordered_set<int> visitedLines;
	unordered_set<int> typedStmtSet, resultSet;
	switch (stmtType) {
	case STATEMENT:
		typedStmtSet = allStmts;
		break;
	case READ:
		typedStmtSet = readStmts;
		break;
	case PRINT:
		typedStmtSet = printStmts;
		break;
	case WHILE:
		typedStmtSet = whileStmts;
		break;
	case IF:
		typedStmtSet = ifStmts;
		break;
	case ASSIGN:
		typedStmtSet = assignStmts;
		break;
	}
	queue<int> linesQueue;
	for (auto &elem : nextMap[firstLine]) {
		linesQueue.push(elem);
	}
	while (linesQueue.size() > 0) {
		int currLine = linesQueue.front();
		linesQueue.pop();
		if (visitedLines.count(currLine) <= 0) {
			visitedLines.insert(currLine);
			for (auto &elem : nextMap[currLine]) {
				linesQueue.push(elem);
			}
		}
	}
	for (auto &elem : visitedLines) {
		if (typedStmtSet.count(elem) > 0) {
			resultSet.insert(elem);
		}
	}
	return resultSet;
}

unordered_set<int> PKB::getPreviousTOf(int firstLine, Type stmtType) {
	unordered_set<int> visitedLines;
	unordered_set<int> typedStmtSet, resultSet;
	switch (stmtType) {
	case STATEMENT:
		typedStmtSet = allStmts;
		break;
	case READ:
		typedStmtSet = readStmts;
		break;
	case PRINT:
		typedStmtSet = printStmts;
		break;
	case WHILE:
		typedStmtSet = whileStmts;
		break;
	case IF:
		typedStmtSet = ifStmts;
		break;
	case ASSIGN:
		typedStmtSet = assignStmts;
		break;
	}
	queue<int> linesQueue;
	for (auto &elem : prevMap[firstLine]) {
		linesQueue.push(elem);
	}
	while (linesQueue.size() > 0) {
		int currLine = linesQueue.front();
		linesQueue.pop();
		if (visitedLines.count(currLine) <= 0) {
			visitedLines.insert(currLine);
			for (auto &elem : prevMap[currLine]) {
				linesQueue.push(elem);
			}
		}
	}
	for (auto &elem : visitedLines) {
		if (typedStmtSet.count(elem) > 0) {
			resultSet.insert(elem);
		}
	}
	return resultSet;
}

unordered_map<int, unordered_set<int>> PKB::getPreviousNextTPairs(Type firstStmtType, Type secondStmtType) {
	unordered_map<int, unordered_set<int>> resultMap;
	unordered_set<int> typedStmtSet;
	switch (firstStmtType) {
	case STATEMENT:
		typedStmtSet = allStmts;
		break;
	case READ:
		typedStmtSet = readStmts;
		break;
	case PRINT:
		typedStmtSet = printStmts;
		break;
	case WHILE:
		typedStmtSet = whileStmts;
		break;
	case IF:
		typedStmtSet = ifStmts;
		break;
	case ASSIGN:
		typedStmtSet = assignStmts;
		break;
	}
	for (auto &elem : typedStmtSet) {
		unordered_set<int> resultSet = getNextTOf(elem, secondStmtType);
		resultMap[elem] = resultSet;
	}
	return resultMap;
}