#pragma once

#include <ctype.h>
#include <stdio.h>
#include <iostream>
#include <queue>
#include <set>
#include <string>
#include <vector>
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

unordered_set<int>* PKB::getTypedStmtSet(Type type) {
	unordered_set<int> *typedStmtSet;

	switch (type) {
	case STATEMENT:
	case PROGLINE:
		typedStmtSet = &allStmts;
		break;
	case READ:
		typedStmtSet = &readStmts;
		break;
	case PRINT:
		typedStmtSet = &printStmts;
		break;
	case WHILE:
		typedStmtSet = &whileStmts;
		break;
	case IF:
		typedStmtSet = &ifStmts;
		break;
	case ASSIGN:
		typedStmtSet = &assignStmts;
		break;
	case CALL:
		typedStmtSet = &callStmts;
		break;
	}

	return typedStmtSet;
}

bool PKB::insertStmtType(int stmtNum, Type type) {
	unordered_set<int> *typedStmtSet;
	switch (type) {
	case STATEMENT:
		return allStmts.insert(stmtNum).second;
	case READ:
		typedStmtSet = &readStmts;
		break;
	case WHILE:
		typedStmtSet = &whileStmts;
		break;
	case IF:
		typedStmtSet = &ifStmts;
		break;
	case ASSIGN:
		typedStmtSet = &assignStmts;
		break;
	case PRINT:
		typedStmtSet = &printStmts;
		break;
	// wrong type
	default:
		return false;
	}
	return typedStmtSet->insert(stmtNum).second && allStmts.insert(stmtNum).second;
}

bool PKB::insertCPRStmtType(int stmtNum, Type type, string name) {
	bool isValidStmt;
	unordered_set<string> *typedNameSet;
	unordered_set<int> *typedStmtSet;
	unordered_map<string, int> *typedTable;
	unordered_map<string, unordered_set<int>> *typedByNameMap;
	unordered_map<int, string> *typedByStmtNumMap;
	vector<string> *typedVector;

	switch(type) {
		case CALL:
			typedNameSet = &calledSet;
			typedStmtSet = &callStmts;
			typedTable = &callTableByName;
			typedByStmtNumMap = &callMapByStmtNum;
			typedByNameMap = &calledStmtMap;
			typedVector = &callTableByIdx;
			break;
		case PRINT:
			typedNameSet = &printSet;
			typedStmtSet = &printStmts;
			typedTable = &printTableByName;
			typedByStmtNumMap = &printMapByStmtNum;
			typedByNameMap = &printStmtMap;
			typedVector = &printTableByIdx;
			break;
		case READ:
			typedNameSet = &readSet;
			typedStmtSet = &readStmts;
			typedTable = &readTableByName;
			typedByStmtNumMap = &readMapByStmtNum;
			typedByNameMap = &readStmtMap;
			typedVector = &readTableByIdx;
			break;
		default:
			return false;
	}
	typedByNameMap->operator[](name).insert(stmtNum);
	typedByStmtNumMap->insert({stmtNum, name});
	isValidStmt = allStmts.insert(stmtNum).second && typedStmtSet->insert(stmtNum).second;
	if (!isValidStmt)
		return false;

	if (!typedNameSet->count(name)) {
		typedNameSet->insert(name);
		typedVector->push_back(name);
		typedTable->insert({name, typedVector->size() - 1});
		return true;
	}
	return false;
}

bool PKB::insertPattern(string pattern, int stmtNum) {
	return patternMap[pattern].insert(stmtNum).second;
}

bool PKB::insertFullPattern(string fullPattern, int stmtNum) {
	return fullPatternMap[fullPattern].insert(stmtNum).second;
}

bool PKB::insertIfControlVar(int ifStmtNum, string varName) {
	ifControlStmtSet.insert(ifStmtNum);
	ifControlVarMap[varName].insert(ifStmtNum);
	return ifControlStmtMap[ifStmtNum].insert(varName).second;
}

bool PKB::insertWhileControlVar(int whileStmtNum, string varName) {
	whileControlStmtSet.insert(whileStmtNum);
	whileControlVarMap[varName].insert(whileStmtNum);
	return whileControlStmtMap[whileStmtNum].insert(varName).second;
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

string PKB::getCallAtStmtNum(int stmtNum) {
	if (callMapByStmtNum.count(stmtNum))
		return callMapByStmtNum[stmtNum];
	return "";
}

int PKB::getReadIdx(string varName) {
	return readTableByName[varName];
}

string PKB::getReadAtIdx(int readIdx) {
	return readTableByIdx[readIdx];
}

string PKB::getReadAtStmtNum(int stmtNum) {
	if (readMapByStmtNum.count(stmtNum))
		return readMapByStmtNum[stmtNum];
	return "";
}

int PKB::getPrintIdx(string varName) {
	return printTableByName[varName];
}

string PKB::getPrintAtIdx(int printIdx) {
	return printTableByIdx[printIdx];
}

string PKB::getPrintAtStmtNum(int stmtNum) {
	if (printMapByStmtNum.count(stmtNum))
		return printMapByStmtNum[stmtNum];
	return "";
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
	unordered_set<int> allModifyingStmts = modifiesByVarMap[varName], *typedStmtSet = getTypedStmtSet(type), resultSet;

	for (const auto &elem : allModifyingStmts) {
		if (typedStmtSet->count(elem)) {
			resultSet.insert(elem);
		}
	}

	return resultSet;
}

unordered_map<int, unordered_set<string>> PKB::getModifiesStmtVarPairs(Type type) {
	unordered_set<int> *typedStmtSet = getTypedStmtSet(type);
	unordered_map<int, unordered_set<string>> resultSet;

	for (const auto &elem : *typedStmtSet)
		resultSet.insert({elem, modifiesByStmtNumMap[elem]});

	return resultSet;
}

unordered_set<int> PKB::getStmtsThatModifiesVar(Type type) {
	unordered_set<int> *typedStmtSet = getTypedStmtSet(type);
	unordered_set<int> resultSet;

	for (const auto &elem : *typedStmtSet) {
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
	unordered_set<int> allUsesStmts = usesByVarMap[varName], *typedStmtSet = getTypedStmtSet(type), resultSet;

	for (const auto &elem : allUsesStmts) {
		if (typedStmtSet->count(elem)) {
			resultSet.insert(elem);
		}
	}

	return resultSet;
}

unordered_map<int, unordered_set<string>> PKB::getUsesStmtVarPairs(Type type) {
	unordered_set<int> *typedStmtSet = getTypedStmtSet(type);
	unordered_map<int, unordered_set<string>> resultSet;

	for (const auto &elem : *typedStmtSet)
		resultSet.insert({elem, usesByStmtNumMap[elem]});

	return resultSet;
}

unordered_set<int> PKB::getStmtsThatUsesVar(Type type) {
	unordered_set<int> *typedStmtSet = getTypedStmtSet(type);
	unordered_set<int> resultSet;

	for (const auto &elem : *typedStmtSet) {
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
	unordered_set<int> *parentTypedStmtSet = getTypedStmtSet(parentType), *childrenTypedStmtSet = getTypedStmtSet(childrenType);
	unordered_map<int, unordered_set<int>> resultMap;

	for (const auto &elem : *parentTypedStmtSet) {
		if (parentMap.count(elem)) {
			for (const auto &elem2 : parentMap[elem]) {
				if (childrenTypedStmtSet->count(elem2)) {
					resultMap[elem].insert(elem2);
				}
			}
		}
	}

	return resultMap;
}

unordered_map<int, unordered_set<int>> PKB::getParentChildrenTPairs(Type parentType, Type childrenType) {
	unordered_set<int> *parentTypedStmtSet = getTypedStmtSet(parentType), *childrenTypedStmtSet = getTypedStmtSet(childrenType);
	unordered_map<int, unordered_set<int>> resultMap;

	for (const auto &elem : *parentTypedStmtSet) {
		if (parentTMap.count(elem)) {
			for (const auto &elem2 : parentTMap[elem]) {
				if (childrenTypedStmtSet->count(elem2)) {
					resultMap[elem].insert(elem2);
				}
			}
		}
	}

	return resultMap;
}

unordered_set<int> PKB::getParentStmts(Type parentType) {
	unordered_set<int> *typedStmtSet = getTypedStmtSet(parentType), resultSet;

	for (const auto &elem : parentMap) {
		if (typedStmtSet->count(elem.first)) {
			resultSet.insert(elem.first);
		}
	}

	return resultSet;
}

unordered_set<int> PKB::getChildrenStmts(Type childrenType) {
	unordered_set<int> *typedStmtSet = getTypedStmtSet(childrenType), resultSet;

	for (const auto &elem : childrenMap) {
		if (typedStmtSet->count(elem.first)) {
			resultSet.insert(elem.first);
		}
	}

	return resultSet;
}

int PKB::getParentOf(int stmtNum, Type parentType) {
	unordered_set<int> *typedStmtSet = getTypedStmtSet(parentType);

	if (childrenMap.count(stmtNum) && typedStmtSet->count(childrenMap[stmtNum])) {
		return childrenMap[stmtNum];
	}
	return -1;
}

unordered_set<int> PKB::getParentTOf(int stmtNum, Type parentType) {
	unordered_set<int> *typedStmtSet = getTypedStmtSet(parentType), resultSet;

	if (childrenTMap.count(stmtNum)) {
		for (const auto &elem : childrenTMap[stmtNum]) {
			if (typedStmtSet->count(elem)) {
				resultSet.insert(elem);
			}
		}
	}

	return resultSet;
}

unordered_set<int> PKB::getChildrenOf(int stmtNum, Type childrenType) {
	unordered_set<int> *typedStmtSet = getTypedStmtSet(childrenType), resultSet;

	if (parentMap.count(stmtNum)) {
		for (const auto &elem : parentMap[stmtNum]) {
			if (typedStmtSet->count(elem)) {
				resultSet.insert(elem);
			}
		}
	}

	return resultSet;
}

unordered_set<int> PKB::getChildrenTOf(int stmtNum, Type childrenType) {
	unordered_set<int> *typedStmtSet = getTypedStmtSet(childrenType), resultSet;

	if (parentTMap.count(stmtNum)) {
		for (const auto &elem : parentTMap[stmtNum]) {
			if (typedStmtSet->count(elem)) {
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
	unordered_set<int> *leaderTypedStmtSet = getTypedStmtSet(leaderType), *followerTypedStmtSet = getTypedStmtSet(followerType);
	unordered_map<int, int> resultMap;

	for (const auto &elem : *leaderTypedStmtSet) {
		if (leaderMap.count(elem) && followerTypedStmtSet->count(leaderMap[elem]))
			resultMap.insert({elem, leaderMap[elem]});
	}

	return resultMap;
}

unordered_map<int, unordered_set<int>> PKB::getLeaderFollowerTPairs(Type leaderType, Type followerType) {
	unordered_set<int> *leaderTypedStmtSet = getTypedStmtSet(leaderType), *followerTypedStmtSet = getTypedStmtSet(followerType);
	unordered_map<int, unordered_set<int>> resultMap;

	for (const auto &elem : *leaderTypedStmtSet) {
		if (leaderTMap.count(elem)) {
			for (const auto &elem2 : leaderTMap[elem]) {
				if (followerTypedStmtSet->count(elem2)){
					resultMap[elem].insert(elem2);
				}
			}
		}
	}

	return resultMap;
}

unordered_set<int> PKB::getLeaderStmts(Type leaderType) {
	unordered_set<int> *typedStmtSet = getTypedStmtSet(leaderType), resultSet;

	for (const auto &elem : leaderMap) {
		if (typedStmtSet->count(elem.first)) {
			resultSet.insert(elem.first);
		}
	}

	return resultSet;
}

unordered_set<int> PKB::getFollowerStmts(Type followerType) {
	unordered_set<int> *typedStmtSet = getTypedStmtSet(followerType), resultSet;

	for (const auto &elem : followerMap) {
		if (typedStmtSet->count(elem.first)) {
			resultSet.insert(elem.first);
		}
	}

	return resultSet;
}

int PKB::getLeaderOf(int stmtNum, Type leaderType) {
	unordered_set<int> *typedStmtSet = getTypedStmtSet(leaderType);

	if (followerMap.count(stmtNum) && typedStmtSet->count(followerMap[stmtNum])) {
		return followerMap[stmtNum];
	}
	return -1;
}

unordered_set<int> PKB::getLeaderTOf(int stmtNum, Type leaderType) {
	unordered_set<int> *typedStmtSet = getTypedStmtSet(leaderType), resultSet;

	if (followerTMap.count(stmtNum)) {
		for (const auto &elem : followerTMap[stmtNum]) {
			if (typedStmtSet->count(elem)) {
				resultSet.insert(elem);
			}
		}
	}

	return resultSet;
}

int PKB::getFollowerOf(int stmtNum, Type followerType) {
	unordered_set<int> *typedStmtSet = getTypedStmtSet(followerType);
	int result = -1;

	if (leaderMap.count(stmtNum) && typedStmtSet->count(leaderMap[stmtNum]))
		result = leaderMap[stmtNum];

	return result;
}

unordered_set<int> PKB::getFollowerTOf(int stmtNum, Type followerType) {
	unordered_set<int> *typedStmtSet = getTypedStmtSet(followerType), resultSet;

	if (leaderTMap.count(stmtNum)) {
		for (const auto &elem : leaderTMap[stmtNum]) {
			if (typedStmtSet->count(elem)) {
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

bool PKB::isNext(int prevLineNum, int nextLineNum) {
	if (nextMap.count(prevLineNum) && nextMap[prevLineNum].count(nextLineNum))
		return true;
	return false;
}

bool PKB::isNextT(int firstLine, int secondLine) {
	unordered_set<int> visitedLines = {};
	return isNextT(firstLine, secondLine, &visitedLines);
}

bool PKB::isNextT(int firstLine, int secondLine, unordered_set<int>* visitedLines) {
	int toExplore = firstLine;
	bool found = false;
	while (!found) {
		if (visitedLines->count(toExplore) > 0) {
			return false;
		}
		visitedLines->insert(toExplore);
		if (nextMap.count(toExplore) > 0) {
			unordered_set<int> currentNextLines = nextMap[toExplore];
			//if there are 2 possible next, it is an if or while
			if (currentNextLines.size() > 1) {
				for (auto &elem : currentNextLines) {
					if (elem == secondLine) {
						return true;
					}
					if (isNextT(elem, secondLine, visitedLines)) {
						return true;
					}
				}
				//failed to find second line in either search, return false
				return false;
			}
			else
			{
				//otherwise just follow the next line
				for (auto &elem : currentNextLines) toExplore = elem;
			}
			if (toExplore == secondLine) {
				return true;
			}
		}
		else {
			break;
		}
	}
	return false;
}

bool PKB::hasNext(int prevLineNum) {
	if (nextMap.count(prevLineNum))
		return true;
	return false;
}

bool PKB::hasPrevious(int nextLineNum) {
	if (prevMap.count(nextLineNum))
		return true;
	return false;
}

bool PKB::hasNextT() {
	if (nextMap.size())
		return true;
	return false;
}

unordered_map<int, unordered_set<int>> PKB::getPreviousNextPairs(Type previousType, Type nextType) {
	unordered_set<int> *prevTypedStmtSet = getTypedStmtSet(previousType), *nextTypedStmtSet = getTypedStmtSet(nextType);
	unordered_map<int, unordered_set<int>> resultMap;

	for (const auto&elem: nextMap) {
		if (prevTypedStmtSet->count(elem.first)) {
			for (const auto&elem2: elem.second) {
				if (nextTypedStmtSet->count(elem2))
					resultMap[elem.first].insert(elem2);
			}
		}
	}

	return resultMap;
}

unordered_map<int, unordered_set<int>> PKB::getPreviousNextTPairs(Type firstStmtType, Type secondStmtType) {
	unordered_map<int, unordered_set<int>> resultMap;
	unordered_set<int> *typedStmtSet = getTypedStmtSet(firstStmtType);
	for (auto &elem : *typedStmtSet) {
		unordered_set<int> resultSet = getNextTOf(elem, secondStmtType);
		resultMap[elem] = resultSet;
	}
	return resultMap;
}

unordered_set<int> PKB::getPreviousLines(Type previousType) {
	unordered_set<int> *typedStmtSet = getTypedStmtSet(previousType), resultSet;

	for (const auto&elem : nextMap) {
		if (typedStmtSet->count(elem.first))
			resultSet.insert(elem.first);
	}

	return resultSet;
}

unordered_set<int> PKB::getNextLines(Type nextType) {
	unordered_set<int> *typedStmtSet = getTypedStmtSet(nextType), resultSet;

	for (const auto&elem : prevMap) {
		if (typedStmtSet->count(elem.first))
			resultSet.insert(elem.first);
	}

	return resultSet;
}

unordered_set<int> PKB::getPreviousOf(int nextLineNum, Type previousType) {
	unordered_set<int> *typedStmtSet = getTypedStmtSet(previousType), resultSet;

	if (prevMap.count(nextLineNum)) {
		for (const auto&elem : prevMap[nextLineNum]) {
			if (typedStmtSet->count(elem)) {
				resultSet.insert(elem);
			}
		}
	}

	return resultSet;
}

unordered_set<int> PKB::getPreviousTOf(int firstLine, Type stmtType) {
	unordered_set<int> visitedLines;
	unordered_set<int> *typedStmtSet = getTypedStmtSet(stmtType), resultSet;
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
		if (typedStmtSet->count(elem) > 0) {
			resultSet.insert(elem);
		}
	}
	return resultSet;
}

unordered_set<int> PKB::getNextOf(int prevLineNum, Type nextType) {
	unordered_set<int> *typedStmtSet = getTypedStmtSet(nextType), resultSet;

	if (nextMap.count(prevLineNum)) {
		for (const auto &elem : nextMap[prevLineNum]) {
			if (typedStmtSet->count(elem)) {
				resultSet.insert(elem);
			}
		}
	}

	return resultSet;
}

unordered_set<int> PKB::getNextTOf(int firstLine, Type stmtType) {
	unordered_set<int> visitedLines;
	unordered_set<int> *typedStmtSet = getTypedStmtSet(stmtType), resultSet;
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
		if (typedStmtSet->count(elem) > 0) {
			resultSet.insert(elem);
		}
	}
	return resultSet;
}

unordered_map<int, unordered_set<string>> PKB::getIfControlVarPair() {
	return ifControlStmtMap;
}

unordered_set<int> PKB::getIfWithControlVar() {
	return ifControlStmtSet;
}

unordered_set<int> PKB::getIfWithControlVar(string controlVar) {
	unordered_set<int> emptySet;
	if (ifControlVarMap.count(controlVar))
		return ifControlVarMap[controlVar];
	return emptySet;
}

unordered_map<int, unordered_set<string>> PKB::getWhileControlVarPair() {
	return whileControlStmtMap;
}

unordered_set<int> PKB::getWhileWithControlVar() {
	return whileControlStmtSet;
}

unordered_set<int> PKB::getWhileWithControlVar(string controlVar) {
	unordered_set<int> emptySet;
	if (whileControlVarMap.count(controlVar))
		return whileControlVarMap[controlVar];
	return emptySet;
}

unordered_set<int> PKB::getCallStmtsWithProc(string procName) {
	unordered_set<int> emptySet;
	if (calledStmtMap.count(procName))
		return calledStmtMap[procName];
	return emptySet;
}

unordered_set<int> PKB::getPrintStmtsWithVar(string varName) {
	unordered_set<int> emptySet;
	if (printStmtMap.count(varName))
		return printStmtMap[varName];
	return emptySet;
}

unordered_set<int> PKB::getReadStmtsWithVar(string varName) {
	unordered_set<int> emptySet;
	if (readStmtMap.count(varName))
		return readStmtMap[varName];
	return emptySet;
}

unordered_map<int, unordered_set<int>> PKB::getAffectsMap(bool isTransitive, bool isAffects) {
	vector<unordered_map<string, unordered_set<int>>> modMaps(allStmts.size() + 1);
	unordered_map<int, unordered_map<string, unordered_set<int>>> prevModMap;
	unordered_map<string, unordered_set<int>> *currModMap;
	unordered_map<int, unordered_set<int>> affectsMap, affectedMap, affectsRelationshipMap, resultTMap;
	unordered_set<int> visitedLines, prevLines, visitedNodes, neighbours;
	unordered_set<string> varsModified, varsUsed;
	int currLine, prevLine, maxLine = 0;
	set<int> toBeVisited;
	string varModified;
	queue<int> toBeVisitedNodes;
	bool isNewProc = false;

	// Navigation of nextMap
	if (allStmts.size() == 0)
		return {};
	toBeVisited.insert(1);
	visitedLines.insert(1);
	while (visitedLines.size() != allStmts.size()) {
		// For moving on to the next proc in the code
		if (toBeVisited.size() == 0) {
			toBeVisited.insert(maxLine + 1);
			isNewProc = true;
		}
		// Earlier lines will always be visited first
		currLine = *toBeVisited.begin();
		toBeVisited.erase(toBeVisited.begin());
		visitedLines.insert(currLine);
		if (currLine > maxLine)
			maxLine = currLine;

		// Duplication/merging of modMaps
		if (currLine != 1 && !isNewProc) {
			if (prevMap[currLine].size() == 1) {
				prevLine = *prevMap[currLine].begin();
				modMaps[currLine] = modMaps[prevLine];
			}
			else {
				prevLines = prevMap[currLine];
				// Merge all the prevLines modMaps into the modMap for current line
				for (const auto &previousLine : prevLines) {
					for (const auto &entry : modMaps[previousLine]) {
						for (const auto &line : entry.second) {
							modMaps[currLine].operator[](entry.first).insert(line);
						}
					}
				}
				if (isWhileStmt(currLine)) {
					if (prevModMap.count(currLine)) {
						if (prevModMap[currLine] != modMaps[currLine]) {
							visitedLines.erase(currLine);
						}
					}
					else {
						visitedLines.erase(currLine);
					}
				}
			}
		}
		// If statement is a WHILE statement, check if it has been "visited".
		// Not "visited" implies that while loop has not been processed,
		// have to enter while loop again.
		if (!isWhileStmt(currLine) || visitedLines.count(currLine) == 0) {
			if (nextMap.count(currLine)) {
				for (const auto &nextLine : nextMap[currLine]) {
					toBeVisited.insert(nextLine);
				}
			}
		}
		currModMap = &modMaps[currLine];
		if (isNewProc)
			isNewProc = false;

		// Updating of modMap
		if (isAssignStmt(currLine)) {
			varsUsed = getVarUsedByStmt(currLine);
			for (const auto &varUsed : varsUsed) {
				if (currModMap->count(varUsed)) {
					for (const auto &modLine : currModMap->operator[](varUsed)) {
						affectsMap[modLine].insert(currLine);
						affectedMap[currLine].insert(modLine);
					}
				}
			}
			varModified = getVarModifiedByAssignStmt(currLine);
			currModMap->erase(varModified);
			currModMap->operator[](varModified).insert(currLine);
		} else if(isReadStmt(currLine)) {
			varModified = *getVarModifiedByStmt(currLine).begin();
			currModMap->erase(varModified);
		} else if (isCallStmt(currLine)) {
			varsModified = getVarModifiedByProc(getCallAtStmtNum(currLine));
			for (const auto &variableModified : varsModified) {
				currModMap->erase(varModified);
			}
		} else if(isWhileStmt(currLine)) {
				prevModMap[currLine] = *currModMap;
		}
	}

	if (!isTransitive) {
		if (isAffects)
			return affectsMap;
		return affectedMap;
	}
	if (isAffects)
		affectsRelationshipMap = affectsMap;
	else
		affectsRelationshipMap = affectedMap;
	for (const auto &affectElem : affectsRelationshipMap) {
		for (const auto &neighbour : affectElem.second) {
			toBeVisitedNodes.push(neighbour);
			resultTMap[affectElem.first].insert(neighbour);
		}
		visitedNodes.insert(affectElem.first);
		while(toBeVisitedNodes.size() != 0) {
			neighbours = affectsRelationshipMap[toBeVisitedNodes.front()];
			for (const auto &neighbour : neighbours) {
				if (!visitedNodes.count(neighbour)) {
					toBeVisitedNodes.push(neighbour);
				}
				resultTMap[affectElem.first].insert(neighbour);
			}
			visitedNodes.insert(toBeVisitedNodes.front());
			toBeVisitedNodes.pop();
		}
	}
	return resultTMap;
}

bool PKB::getAffectsBoolean(bool isTransitive, int modifierStmtNum, int userStmtNum) {
	if (isTransitive) {
		unordered_map<int, unordered_set<int>> affectsTMap = getAffectsMap(isTransitive, true);
		if (affectsTMap.count(modifierStmtNum) && affectsTMap[modifierStmtNum].count(userStmtNum))
			return true;
		return false;
	}

	vector<unordered_map<string, int>> modMaps(allStmts.size());
	unordered_map<int, unordered_map<string, int>> prevModMap;
	unordered_map<string, int> currModMap;
	unordered_map<int, unordered_set<int>> affectsMap, affectsTMap;
	unordered_set<int> visitedLines, prevLines, visitedNodes, neighbours;
	unordered_set<string> varsModified, varsUsed;
	int currLine, prevLine, maxLine = 0;
	set<int> toBeVisited;
	string varModified;
	queue<int> toBeVisitedNodes;

	// Navigation of nextMap
	if (allStmts.size() == 0)
		return {};
	toBeVisited.insert(modifierStmtNum);
	visitedLines.insert(modifierStmtNum);
	while (visitedLines.size() != allStmts.size()) {
		// For moving on to the next proc in the code
		if (toBeVisited.size() == 0) {
			toBeVisited.insert(maxLine + 1);
		}
		// Earlier lines will always be visited first
		currLine = *toBeVisited.begin();
		toBeVisited.erase(toBeVisited.begin());
		visitedLines.insert(currLine);
		if (currLine > maxLine)
			maxLine = currLine;

		// Duplication/merging of modMaps
		if (currLine != 1) {
			if (prevMap[currLine].size() == 1) {
				prevLine = *prevMap[currLine].begin();
				modMaps[currLine] = modMaps[prevLine];
			}
			else {
				prevLines = prevMap[currLine];
				// Get rid of previous statement that is not within the while loop
				// Only if it's not the first time reaching the while statement
				if (prevModMap.count(currLine)) {
					for (const auto &previousLine : prevLines) {
						if (previousLine < currLine)
							prevLines.erase(previousLine);
					}
				}
				if (isWhileStmt(currLine)) {
					if (prevModMap.count(currLine)) {
						if (prevModMap[currLine] != modMaps[currLine])
							visitedLines.erase(currLine);
					}
					prevModMap[currLine] = modMaps[currLine];
				}
				// Merge all the prevLines modMaps into the modMap for current line
				for (const auto &previousLine : prevLines) {
					for (const auto &entry : modMaps[previousLine]) {
						modMaps[currLine].insert({entry.first, entry.second});
					}
				}
			}
			// If the while statement loop has been properly processed, don't enter the while loop anymore
			if (!isWhileStmt(currLine) || visitedLines.count(currLine) == 0) {
				for (const auto &nextLine : nextMap[currLine]) {
					toBeVisited.insert(nextLine);
				}
			}
		}
		currModMap = modMaps[currLine];

		// Updating of modMap
		if (isAssignStmt(currLine)) {
			varsUsed = getVarUsedByStmt(currLine);
			varModified = getVarModifiedByAssignStmt(currLine);
			for (const auto &varUsed : varsUsed) {
				if (currModMap.count(varUsed)) {
					affectsMap[currModMap[varUsed]].insert(currLine);
					if (modifierStmtNum != -1 && userStmtNum != -1) {
						if (currModMap[varUsed] == modifierStmtNum && currLine == userStmtNum)
							return true;
					} else if (modifierStmtNum != -1 && userStmtNum == -1) {
						if (currModMap[varUsed] == modifierStmtNum)
							return true;
					} else if (modifierStmtNum == -1 && userStmtNum != -1) {
						if (currLine == userStmtNum)
							return true;
					}
				}
			}
			currModMap.erase(varModified);
			currModMap.insert({varModified, currLine});
		} else if(isReadStmt(currLine)) {
			varModified = *getVarModifiedByStmt(currLine).begin();
			currModMap.erase(varModified);
		} else if (isCallStmt(currLine)) {
			varsModified = getVarUsedByProc(getCallAtStmtNum(currLine));
			for (const auto &variableModified : varsModified) {
				currModMap.erase(variableModified);
			}
		} else if(isWhileStmt(currLine)) {
				if (prevModMap.count(currLine)) {
					if (prevModMap[currLine] != currModMap)
						visitedLines.erase(currLine);
				}
				prevModMap[currLine] = currModMap;
		}
	}
	return false;
}

unordered_set<int> PKB::getAffectsSet(bool isAffects, bool isTransitive, int modifierStmtNum, int userStmtNum) {
	unordered_set<int> resultSet;
	unordered_map<int, unordered_set<int>> affectsRelationshipMap = getAffectsMap(isTransitive, isAffects);
	if (!isTransitive) {
		if (modifierStmtNum == -1 && userStmtNum == -1) {
			for (const auto &affectsElem : affectsRelationshipMap) {
				resultSet.insert(affectsElem.first);
			}
			return resultSet;
		} else if (userStmtNum != -1) {
			return affectsRelationshipMap[userStmtNum];
		} else if (modifierStmtNum != -1) {
			return affectsRelationshipMap[modifierStmtNum];
		}
	}
	if (userStmtNum != -1)
		return affectsRelationshipMap[userStmtNum];
	if (modifierStmtNum != -1)
		return affectsRelationshipMap[modifierStmtNum];
}

bool PKB::isAffects(int modifierStmtNum, int userStmtNum) {
	return getAffectsBoolean(false, modifierStmtNum, userStmtNum);
}

bool PKB::isAffectsT(int modifierStmtNum, int userStmtNum) {
	return getAffectsBoolean(true, modifierStmtNum, userStmtNum);
}

bool PKB::hasAffects() {
	return getAffectsBoolean(false, -1, -1);
}

bool PKB::hasUser(int modifierStmtNum) {
	return getAffectsBoolean(false, modifierStmtNum, -1);
}

bool PKB::hasModifier(int userStmtNum) {
	return getAffectsBoolean(false, -1, userStmtNum);
}

unordered_map<int, unordered_set<int>> PKB::getModifierUserPairs() {
	return getAffectsMap(false, true);
}

unordered_map<int, unordered_set<int>> PKB::getModifierUserTPairs() {
	return getAffectsMap(true, true);
}

unordered_set<int> PKB::getModifierStmts() {
	return getAffectsSet(true, false, -1, -1);
}

unordered_set<int> PKB::getUserStmts() {
	return getAffectsSet(false, false, -1, -1);
}

unordered_set<int> PKB::getModifierOf(int userStmtNum) {
	return getAffectsSet(true, false, -1, userStmtNum);
}

unordered_set<int> PKB::getModifierTOf(int userStmtNum) {
	return getAffectsSet(true, true, -1, userStmtNum);
}

unordered_set<int> PKB::getUserOf(int modifierStmtNum) {
	return getAffectsSet(false, false, modifierStmtNum, -1);
}

unordered_set<int> PKB::getUserTOf(int modifierStmtNum) {
	return getAffectsSet(false, true, modifierStmtNum, -1);
}