#pragma once

#include <ctype.h>
#include <set>
#include "PKB.h"

// Frontend APIs
bool PKB::insertVar(std::string varName) {
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

bool PKB::insertProc(std::string procName) {
	if (!procTableByName.count(procName)) {
		procSet.insert(procName);
		procTableByIdx.push_back(procName);
		procTableByName.insert({procName, procTableByIdx.size() - 1});
		return true;
	}
	return false;
}

std::unordered_set<int>* PKB::getTypedStmtSet(Type type) {
	std::unordered_set<int> *typedStmtSet;

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
	case SWITCH:
		typedStmtSet = &switchStmts;
		break;
	}

	return typedStmtSet;
}

bool PKB::insertStmtType(int stmtNum, Type type) {
	std::unordered_set<int> *typedStmtSet;
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
	case SWITCH:
		typedStmtSet = &switchStmts;
		break;
	// wrong type
	default:
		return false;
	}
	return typedStmtSet->insert(stmtNum).second && allStmts.insert(stmtNum).second;
}

bool PKB::insertCPRStmtType(int stmtNum, Type type, std::string name) {
	bool isValidStmt;
	std::unordered_set<std::string> *typedNameSet;
	std::unordered_set<int> *typedStmtSet;
	std::unordered_map<std::string, int> *typedTable;
	std::unordered_map<std::string, std::unordered_set<int>> *typedByNameMap;
	std::unordered_map<int, std::string> *typedByStmtNumMap;
	std::vector<std::string> *typedVector;

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

bool PKB::insertPattern(std::string pattern, int stmtNum) {
	return patternMap[pattern].insert(stmtNum).second;
}

bool PKB::insertFullPattern(std::string fullPattern, int stmtNum) {
	return fullPatternMap[fullPattern].insert(stmtNum).second;
}

bool PKB::insertIfControlVar(int ifStmtNum, std::string varName) {
	ifControlStmtSet.insert(ifStmtNum);
	ifControlVarMap[varName].insert(ifStmtNum);
	return ifControlStmtMap[ifStmtNum].insert(varName).second;
}

bool PKB::insertWhileControlVar(int whileStmtNum, std::string varName) {
	whileControlStmtSet.insert(whileStmtNum);
	whileControlVarMap[varName].insert(whileStmtNum);
	return whileControlStmtMap[whileStmtNum].insert(varName).second;
}

bool PKB::insertSwitchControlVar(int switchStmtNum, std::string varName) {
	switchControlStmtSet.insert(switchStmtNum);
	switchControlVarMap[varName].insert(switchStmtNum);
	return switchControlStmtMap[switchStmtNum].insert(varName).second;
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

bool PKB::setModifies(int stmtNum, std::string varName) {
	modifiesByStmtNumMap[stmtNum].insert(varName);
	return modifiesByVarMap[varName].insert(stmtNum).second;
}

bool PKB::setModifies(std::string procName, std::string varName) {
	modifiesProcSet.insert(procName);
	varModifiedByProcMap[varName].insert(procName);
	return modifiesByProcMap[procName].insert(varName).second;
}

bool PKB::setUses(int stmtNum, std::string varName) {
	usesByStmtNumMap[stmtNum].insert(varName);
	return usesByVarMap[varName].insert(stmtNum).second;
}

bool PKB::setUses(std::string procName, std::string varName) {
	usesProcSet.insert(procName);
	varUsedByProcMap[varName].insert(procName);
	return usesByProcMap[procName].insert(varName).second;
}

bool PKB::setCalls(std::string proc1, std::string proc2) {
	callSet.insert(proc1);
	return callMap[proc1].insert(proc2).second;
}

bool PKB::setCalledBy(std::string proc1, std::string proc2) {
	return calledByMap[proc2].insert(proc1).second;
}

bool PKB::setCallsT(std::string proc1, std::string proc2) {
	return callsTMap[proc1].insert(proc2).second;
}

bool PKB::setCalledByT(std::string proc1, std::string proc2) {
	return calledByTMap[proc2].insert(proc1).second;
}

bool PKB::setNext(int prevLine, int nextLine) {
	return nextMap[prevLine].insert(nextLine).second;
}

bool PKB::setPrevious(int prevLine, int nextLine) {
	return prevMap[nextLine].insert(prevLine).second;
}

// PQL APIs
std::unordered_set<int> PKB::getAllStmts() {
	return allStmts;
}

std::unordered_set<int> PKB::getReadStmts() {
	return readStmts;
}

std::unordered_set<int> PKB::getPrintStmts() {
	return printStmts;
}

std::unordered_set<int> PKB::getWhileStmts() {
	return whileStmts;
}

std::unordered_set<int> PKB::getIfStmts() {
	return ifStmts;
}

std::unordered_set<int> PKB::getAssignStmts() {
	return assignStmts;
}

std::unordered_set<std::string> PKB::getAllVariables() {
	return varSet;
}

std::unordered_set<int> PKB::getAllConstant() {
	return constSet;
}

std::unordered_set<std::string> PKB::getAllProcedures() {
	return procSet;
}

std::unordered_set<int> PKB::getCallStmts() {
	return callStmts;
}

std::unordered_set<std::string> PKB::getCallProcNames() {
	return calledSet;
}

std::unordered_set<std::string> PKB::getPrintVarNames() {
	return printSet;
}

std::unordered_set<std::string> PKB::getReadVarNames() {
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

int PKB::getVarIdx(std::string varName) {
	return varTableByName[varName];
}

std::string PKB::getVarAtIdx(int varIdx) {
	return varTableByIdx[varIdx];
}

std::unordered_map<std::string, int> PKB::getVarTable() {
	return varTableByName;
}

int PKB::getProcIdx(std::string procName) {
	return procTableByName[procName];
}

std::string PKB::getProcAtIdx(int procIdx) {
	return procTableByIdx[procIdx];
}

std::unordered_map<std::string, int> PKB::getProcTable() {
	return procTableByName;
}

int PKB::getCallIdx(std::string procName) {
	return callTableByName[procName];
}

std::string PKB::getCallAtIdx(int callIdx) {
	return callTableByIdx[callIdx];
}

std::string PKB::getCallAtStmtNum(int stmtNum) {
	if (callMapByStmtNum.count(stmtNum))
		return callMapByStmtNum[stmtNum];
	return "";
}

int PKB::getReadIdx(std::string varName) {
	return readTableByName[varName];
}

std::string PKB::getReadAtIdx(int readIdx) {
	return readTableByIdx[readIdx];
}

std::string PKB::getReadAtStmtNum(int stmtNum) {
	if (readMapByStmtNum.count(stmtNum))
		return readMapByStmtNum[stmtNum];
	return "";
}

int PKB::getPrintIdx(std::string varName) {
	return printTableByName[varName];
}

std::string PKB::getPrintAtIdx(int printIdx) {
	return printTableByIdx[printIdx];
}

std::string PKB::getPrintAtStmtNum(int stmtNum) {
	if (printMapByStmtNum.count(stmtNum))
		return printMapByStmtNum[stmtNum];
	return "";
}

bool PKB::isModifies(int stmtNum, std::string varName) {
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

std::unordered_set<std::string> PKB::getVarModifiedByStmt(int stmtNum) {
	return modifiesByStmtNumMap[stmtNum];
}

std::unordered_set<int> PKB::getStmtsThatModifiesVar(std::string varName, Type type) {
	std::unordered_set<int> allModifyingStmts = modifiesByVarMap[varName], *typedStmtSet = getTypedStmtSet(type), resultSet;

	for (const auto &elem : allModifyingStmts) {
		if (typedStmtSet->count(elem)) {
			resultSet.insert(elem);
		}
	}

	return resultSet;
}

std::unordered_map<int, std::unordered_set<std::string>> PKB::getModifiesStmtVarPairs(Type type) {
	std::unordered_set<int> *typedStmtSet = getTypedStmtSet(type);
	std::unordered_map<int, std::unordered_set<std::string>> resultSet;

	for (const auto &elem : *typedStmtSet)
		resultSet.insert({elem, modifiesByStmtNumMap[elem]});

	return resultSet;
}

std::unordered_set<int> PKB::getStmtsThatModifiesVar(Type type) {
	std::unordered_set<int> *typedStmtSet = getTypedStmtSet(type);
	std::unordered_set<int> resultSet;

	for (const auto &elem : *typedStmtSet) {
		if (modifiesByStmtNumMap.count(elem))
			resultSet.insert(elem);
	}

	return resultSet;
}

bool PKB::isProcModifies(std::string procName, std::string varName) {
	if (modifiesByProcMap.count(procName) && modifiesByProcMap[procName].count(varName))
		return true;
	return false;
}

bool PKB::doesProcModifies(std::string procName) {
	if (modifiesByProcMap.count(procName))
		return true;
	return false;
}

std::unordered_set<std::string> PKB::getVarModifiedByProc(std::string procName) {
	return modifiesByProcMap[procName];
}

std::unordered_map<std::string, std::unordered_set<std::string>> PKB::getModifiesProcVarPairs() {
	return modifiesByProcMap;
}

std::unordered_set<std::string> PKB:: getProcThatModifiesVar() {
	return modifiesProcSet;
}

std::unordered_set<std::string> PKB::getProcThatModifiesVar(std::string varName) {
	return varModifiedByProcMap[varName];
}

bool PKB::isUses(int stmtNum, std::string varName) {
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

std::unordered_set<std::string> PKB::getVarUsedByStmt(int stmtNum) {
	return usesByStmtNumMap[stmtNum];
}

std::unordered_set<int> PKB::getStmtsThatUsesVar(std::string varName, Type type) {
	std::unordered_set<int> allUsesStmts = usesByVarMap[varName], *typedStmtSet = getTypedStmtSet(type), resultSet;

	for (const auto &elem : allUsesStmts) {
		if (typedStmtSet->count(elem)) {
			resultSet.insert(elem);
		}
	}

	return resultSet;
}

std::unordered_map<int, std::unordered_set<std::string>> PKB::getUsesStmtVarPairs(Type type) {
	std::unordered_set<int> *typedStmtSet = getTypedStmtSet(type);
	std::unordered_map<int, std::unordered_set<std::string>> resultSet;

	for (const auto &elem : *typedStmtSet)
		resultSet.insert({elem, usesByStmtNumMap[elem]});

	return resultSet;
}

std::unordered_set<int> PKB::getStmtsThatUsesVar(Type type) {
	std::unordered_set<int> *typedStmtSet = getTypedStmtSet(type);
	std::unordered_set<int> resultSet;

	for (const auto &elem : *typedStmtSet) {
		if (usesByStmtNumMap.count(elem))
			resultSet.insert(elem);
	}

	return resultSet;
}

bool PKB::isProcUses(std::string procName, std::string varName) {
	if (usesByProcMap.count(procName) && usesByProcMap[procName].count(varName))
		return true;
	return false;
}

bool PKB::doesProcUses(std::string procName) {
	if (usesByProcMap.count(procName))
		return true;
	return false;
}

std::unordered_set<std::string> PKB::getVarUsedByProc(std::string procName) {
	return usesByProcMap[procName];
}

std::unordered_map<std::string, std::unordered_set<std::string>> PKB::getUsesProcVarPairs() {
	return usesByProcMap;
}

std::unordered_set<std::string> PKB::getProcThatUsesVar() {
	return usesProcSet;
}

std::unordered_set<std::string> PKB::getProcThatUsesVar(std::string varName) {
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

std::unordered_map<int, std::unordered_set<int>> PKB::getParentChildrenPairs(Type parentType, Type childrenType) {
	std::unordered_set<int> *parentTypedStmtSet = getTypedStmtSet(parentType), *childrenTypedStmtSet = getTypedStmtSet(childrenType);
	std::unordered_map<int, std::unordered_set<int>> resultMap;

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

std::unordered_map<int, std::unordered_set<int>> PKB::getParentChildrenTPairs(Type parentType, Type childrenType) {
	std::unordered_set<int> *parentTypedStmtSet = getTypedStmtSet(parentType), *childrenTypedStmtSet = getTypedStmtSet(childrenType);
	std::unordered_map<int, std::unordered_set<int>> resultMap;

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

std::unordered_set<int> PKB::getParentStmts(Type parentType) {
	std::unordered_set<int> *typedStmtSet = getTypedStmtSet(parentType), resultSet;

	for (const auto &elem : parentMap) {
		if (typedStmtSet->count(elem.first)) {
			resultSet.insert(elem.first);
		}
	}

	return resultSet;
}

std::unordered_set<int> PKB::getChildrenStmts(Type childrenType) {
	std::unordered_set<int> *typedStmtSet = getTypedStmtSet(childrenType), resultSet;

	for (const auto &elem : childrenMap) {
		if (typedStmtSet->count(elem.first)) {
			resultSet.insert(elem.first);
		}
	}

	return resultSet;
}

int PKB::getParentOf(int stmtNum, Type parentType) {
	std::unordered_set<int> *typedStmtSet = getTypedStmtSet(parentType);

	if (childrenMap.count(stmtNum) && typedStmtSet->count(childrenMap[stmtNum])) {
		return childrenMap[stmtNum];
	}
	return -1;
}

std::unordered_set<int> PKB::getParentTOf(int stmtNum, Type parentType) {
	std::unordered_set<int> *typedStmtSet = getTypedStmtSet(parentType), resultSet;

	if (childrenTMap.count(stmtNum)) {
		for (const auto &elem : childrenTMap[stmtNum]) {
			if (typedStmtSet->count(elem)) {
				resultSet.insert(elem);
			}
		}
	}

	return resultSet;
}

std::unordered_set<int> PKB::getChildrenOf(int stmtNum, Type childrenType) {
	std::unordered_set<int> *typedStmtSet = getTypedStmtSet(childrenType), resultSet;

	if (parentMap.count(stmtNum)) {
		for (const auto &elem : parentMap[stmtNum]) {
			if (typedStmtSet->count(elem)) {
				resultSet.insert(elem);
			}
		}
	}

	return resultSet;
}

std::unordered_set<int> PKB::getChildrenTOf(int stmtNum, Type childrenType) {
	std::unordered_set<int> *typedStmtSet = getTypedStmtSet(childrenType), resultSet;

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

std::unordered_map<int, int> PKB::getLeaderFollowerPairs(Type leaderType, Type followerType) {
	std::unordered_set<int> *leaderTypedStmtSet = getTypedStmtSet(leaderType), *followerTypedStmtSet = getTypedStmtSet(followerType);
	std::unordered_map<int, int> resultMap;

	for (const auto &elem : *leaderTypedStmtSet) {
		if (leaderMap.count(elem) && followerTypedStmtSet->count(leaderMap[elem]))
			resultMap.insert({elem, leaderMap[elem]});
	}

	return resultMap;
}

std::unordered_map<int, std::unordered_set<int>> PKB::getLeaderFollowerTPairs(Type leaderType, Type followerType) {
	std::unordered_set<int> *leaderTypedStmtSet = getTypedStmtSet(leaderType), *followerTypedStmtSet = getTypedStmtSet(followerType);
	std::unordered_map<int, std::unordered_set<int>> resultMap;

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

std::unordered_set<int> PKB::getLeaderStmts(Type leaderType) {
	std::unordered_set<int> *typedStmtSet = getTypedStmtSet(leaderType), resultSet;

	for (const auto &elem : leaderMap) {
		if (typedStmtSet->count(elem.first)) {
			resultSet.insert(elem.first);
		}
	}

	return resultSet;
}

std::unordered_set<int> PKB::getFollowerStmts(Type followerType) {
	std::unordered_set<int> *typedStmtSet = getTypedStmtSet(followerType), resultSet;

	for (const auto &elem : followerMap) {
		if (typedStmtSet->count(elem.first)) {
			resultSet.insert(elem.first);
		}
	}

	return resultSet;
}

int PKB::getLeaderOf(int stmtNum, Type leaderType) {
	std::unordered_set<int> *typedStmtSet = getTypedStmtSet(leaderType);

	if (followerMap.count(stmtNum) && typedStmtSet->count(followerMap[stmtNum])) {
		return followerMap[stmtNum];
	}
	return -1;
}

std::unordered_set<int> PKB::getLeaderTOf(int stmtNum, Type leaderType) {
	std::unordered_set<int> *typedStmtSet = getTypedStmtSet(leaderType), resultSet;

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
	std::unordered_set<int> *typedStmtSet = getTypedStmtSet(followerType);
	int result = -1;

	if (leaderMap.count(stmtNum) && typedStmtSet->count(leaderMap[stmtNum]))
		result = leaderMap[stmtNum];

	return result;
}

std::unordered_set<int> PKB::getFollowerTOf(int stmtNum, Type followerType) {
	std::unordered_set<int> *typedStmtSet = getTypedStmtSet(followerType), resultSet;

	if (leaderTMap.count(stmtNum)) {
		for (const auto &elem : leaderTMap[stmtNum]) {
			if (typedStmtSet->count(elem)) {
				resultSet.insert(elem);
			}
		}
	}

	return resultSet;
}

std::string PKB::getVarModifiedByAssignStmt(int stmtNum) {
	if (modifiesByStmtNumMap.count(stmtNum) && isAssignStmt(stmtNum)) {
		return *modifiesByStmtNumMap[stmtNum].begin();
	}
	return "";
}

std::unordered_set<int> PKB::getAssignStmtsThatModifiesVar(std::string varName) {
	return modifiesByVarMap[varName];
}

std::unordered_set<int> PKB::getAssignStmtsWithSubMatch(std::string subString) {
	return patternMap[subString];
}

std::unordered_set<int> PKB::getAssignStmtsWithExactMatch(std::string exactString) {
	return fullPatternMap[exactString];
}

bool PKB::isCalls(std::string callerName, std::string receiverName) {
	if (callMap.count(callerName) && callMap[callerName].count(receiverName))
		return true;
	return false;
}

bool PKB::isCallsT(std::string callerName, std::string receiverName) {
	if (callsTMap.count(callerName) && callsTMap[callerName].count(receiverName))
		return true;
	return false;
}

bool PKB::hasReceiver(std::string callerName) {
	if (callMap.count(callerName))
		return true;
	return false;
}

bool PKB::hasCaller(std::string receiverName) {
	if (calledByMap.count(receiverName))
		return true;
	return false;
}

std::unordered_map<std::string, std::unordered_set<std::string>> PKB::getCallerReceiverPairs() {
	return callMap;
}

std::unordered_map<std::string, std::unordered_set<std::string>> PKB::getCallerReceiverTPairs() {
	return callsTMap;
}

std::unordered_set<std::string> PKB::getCallerProcedures() {
	return callSet;
}

std::unordered_set<std::string> PKB::getReceiverProcedures() {
	return calledSet;
}

std::unordered_set<std::string> PKB::getCallerOf(std::string receiverName) {
	return calledByMap[receiverName];
}

std::unordered_set<std::string> PKB::getCallerTOf(std::string receiverName) {
	return calledByTMap[receiverName];
}

std::unordered_set<std::string> PKB::getReceiverOf(std::string callerName) {
	return callMap[callerName];
}

std::unordered_set<std::string> PKB::getReceiverTOf(std::string callerName) {
	return callsTMap[callerName];
}

bool PKB::isNext(int prevLineNum, int nextLineNum) {
	if (nextMap.count(prevLineNum) && nextMap[prevLineNum].count(nextLineNum))
		return true;
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

std::unordered_map<int, std::unordered_set<int>> PKB::getPreviousNextPairs(Type previousType, Type nextType) {
	std::unordered_set<int> *prevTypedStmtSet = getTypedStmtSet(previousType), *nextTypedStmtSet = getTypedStmtSet(nextType);
	std::unordered_map<int, std::unordered_set<int>> resultMap;

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

bool PKB::isNextT(int firstLine, int secondLine) {
	std::unordered_set<int> resultSet = getNextTOf(firstLine, STATEMENT);
	return resultSet.count(secondLine) > 0 ? true : false;
}

std::unordered_set<int> PKB::getPreviousLines(Type previousType) {
	std::unordered_set<int> *typedStmtSet = getTypedStmtSet(previousType), resultSet;

	for (const auto&elem : nextMap) {
		if (typedStmtSet->count(elem.first))
			resultSet.insert(elem.first);
	}

	return resultSet;
}

std::unordered_set<int> PKB::getNextLines(Type nextType) {
	std::unordered_set<int> *typedStmtSet = getTypedStmtSet(nextType), resultSet;

	for (const auto&elem : prevMap) {
		if (typedStmtSet->count(elem.first))
			resultSet.insert(elem.first);
	}

	return resultSet;
}

std::unordered_set<int> PKB::getPreviousOf(int nextLineNum, Type previousType) {
	std::unordered_set<int> *typedStmtSet = getTypedStmtSet(previousType), resultSet;

	if (prevMap.count(nextLineNum)) {
		for (const auto&elem : prevMap[nextLineNum]) {
			if (typedStmtSet->count(elem)) {
				resultSet.insert(elem);
			}
		}
	}

	return resultSet;
}

std::unordered_set<int> PKB::getNextOf(int prevLineNum, Type nextType) {
	std::unordered_set<int> *typedStmtSet = getTypedStmtSet(nextType), resultSet;

	if (nextMap.count(prevLineNum)) {
		for (const auto &elem : nextMap[prevLineNum]) {
			if (typedStmtSet->count(elem)) {
				resultSet.insert(elem);
			}
		}
	}

	return resultSet;
}

std::unordered_set<int> PKB::getPreviousTOf(int firstLine, Type stmtType) {
	std::unordered_set<int> visitedLines;
	std::unordered_set<int> *typedStmtSet = getTypedStmtSet(stmtType), resultSet;
	if (prevTCache.count(firstLine) > 0) {
		visitedLines = prevTCache[firstLine];
	}
	else{
		std::queue<int> linesQueue;
		for (auto &elem : prevMap[firstLine]) {
			linesQueue.push(elem);
		}
		while (linesQueue.size() > 0) {
			int currLine = linesQueue.front();
			nextTCache[currLine].insert(firstLine);
			prevTCache[firstLine].insert(currLine);
			linesQueue.pop();
			visitedLines.insert(currLine);
			for (auto &elem : prevMap[currLine]) {
				if (visitedLines.count(elem) < 1) {
					linesQueue.push(elem);
				}
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

std::unordered_set<int> PKB::getNextTOf(int firstLine, Type stmtType) {
	std::unordered_set<int> visitedLines;
	std::unordered_set<int> *typedStmtSet = getTypedStmtSet(stmtType), resultSet;
	if (nextTCache.count(firstLine) > 0) {
		visitedLines = nextTCache[firstLine];
	}
	else {
		std::queue<int> linesQueue;
		for (auto &elem : nextMap[firstLine]) {
			linesQueue.push(elem);
		}
		while (linesQueue.size() > 0) {
			int currLine = linesQueue.front();
			nextTCache[firstLine].insert(currLine);
			prevTCache[currLine].insert(firstLine);
			linesQueue.pop();
			visitedLines.insert(currLine);
			for (auto &elem : nextMap[currLine]) {
				if (visitedLines.count(elem) < 1) {
					linesQueue.push(elem);
				}
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

std::unordered_map<int, std::unordered_set<int>> PKB::getPreviousNextTPairs(Type firstStmtType, Type secondStmtType) {
	std::unordered_map<int, std::unordered_set<int>> resultMap;
	std::unordered_set<int> *typedStmtSet = getTypedStmtSet(firstStmtType);
	for (const auto &elem : *typedStmtSet) {
		std::unordered_set<int> resultSet = getNextTOf(elem, secondStmtType);
		resultMap[elem] = resultSet;
	}
	return resultMap;
}

void PKB::clearNextTCache() {
	nextTCache = {};
	prevTCache = {};
}

std::unordered_map<int, std::unordered_set<std::string>> PKB::getIfControlVarPair() {
	return ifControlStmtMap;
}

std::unordered_set<int> PKB::getIfWithControlVar() {
	return ifControlStmtSet;
}

std::unordered_set<int> PKB::getIfWithControlVar(std::string controlVar) {
	std::unordered_set<int> emptySet;
	if (ifControlVarMap.count(controlVar))
		return ifControlVarMap[controlVar];
	return emptySet;
}

std::unordered_map<int, std::unordered_set<std::string>> PKB::getWhileControlVarPair() {
	return whileControlStmtMap;
}

std::unordered_set<int> PKB::getWhileWithControlVar() {
	return whileControlStmtSet;
}

std::unordered_set<int> PKB::getWhileWithControlVar(std::string controlVar) {
	std::unordered_set<int> emptySet;
	if (whileControlVarMap.count(controlVar))
		return whileControlVarMap[controlVar];
	return emptySet;
}

std::unordered_set<int> PKB::getCallStmtsWithProc(std::string procName) {
	std::unordered_set<int> emptySet;
	if (calledStmtMap.count(procName))
		return calledStmtMap[procName];
	return emptySet;
}

std::unordered_set<int> PKB::getPrintStmtsWithVar(std::string varName) {
	std::unordered_set<int> emptySet;
	if (printStmtMap.count(varName))
		return printStmtMap[varName];
	return emptySet;
}

std::unordered_set<int> PKB::getReadStmtsWithVar(std::string varName) {
	std::unordered_set<int> emptySet;
	if (readStmtMap.count(varName))
		return readStmtMap[varName];
	return emptySet;
}

std::unordered_map<int, std::unordered_set<int>> PKB::getAffectsMap(bool isTransitive, bool isAffects) {
	if ((isAffectsComputed || isAffectsTComputed) && !isTransitive) {
		if (isAffects)
			return affectsMap;
		return affectedMap;
	} else if (isAffectsTComputed && isTransitive) {
		if (isAffects)
			return affectsTMap;
		return affectedTMap;
	}

	std::vector<std::unordered_map<std::string, std::unordered_set<int>>> modMaps(allStmts.size() + 1);
	std::unordered_map<int, std::unordered_map<std::string, std::unordered_set<int>>> prevModMap;
	std::unordered_map<std::string, std::unordered_set<int>> *currModMap;
	std::unordered_map<int, std::unordered_set<int>> affectsRelationshipMap, *resultTMap;
	std::unordered_set<int> visitedLines, prevLines, visitedNodes, neighbours;
	std::unordered_set<std::string> varsModified, varsUsed;
	int currLine, prevLine, maxLine = 0;
	std::set<int> toBeVisited;
	std::string varModified;
	std::queue<int> toBeVisitedNodes;
	bool isNewProc = true;

	if (!isAffectsComputed || !isTransitive) {
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
			if (!allStmts.count(currLine))
				break;
			toBeVisited.erase(toBeVisited.begin());
			visitedLines.insert(currLine);
			if (currLine > maxLine)
				maxLine = currLine;

			// New modMap each time pkb traverses to the currLine
			// So outdated values do not stay in modMap
			modMaps[currLine] = {};

			// Duplication/merging of modMaps
			if (!isNewProc) {
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
				}
			}

			// Compare prevModMap for while statement
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
							if (smallestAffectsLine > modLine)
								smallestAffectsLine = modLine;
							if (largestAffectsLine < modLine)
								largestAffectsLine = modLine;
							if (smallestAffectedLine > currLine)
								smallestAffectedLine = currLine;
							if (largestAffectedLine < currLine)
								largestAffectedLine = modLine;
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
					currModMap->erase(variableModified);
				}
			} else if(isWhileStmt(currLine)) {
					prevModMap[currLine] = *currModMap;
			}
		}
		isAffectsComputed = true;
	}

	if (!isTransitive) {
		if (isAffects)
			return affectsMap;
		return affectedMap;
	}
	if (isAffects) {
		affectsRelationshipMap = affectsMap;
		resultTMap = &affectsTMap;
	}
	else {
		affectsRelationshipMap = affectedMap;
		resultTMap = &affectedTMap;
	}
	for (const auto &affectElem : affectsRelationshipMap) {
		if (affectElem.first < smallestAffectsTLine)
			smallestAffectsTLine = affectElem.first;
		if (affectElem.first > largestAffectsTLine)
			largestAffectsTLine = affectElem.first;
		visitedNodes = {};
		for (const auto &neighbour : affectElem.second) {
			toBeVisitedNodes.push(neighbour);
			resultTMap->operator[](affectElem.first).insert(neighbour);
		}
		visitedNodes.insert(affectElem.first);
		while(toBeVisitedNodes.size() != 0) {
			if (visitedNodes.count(toBeVisitedNodes.front())) {
				for (const auto &neighbourT : resultTMap->operator[](toBeVisitedNodes.front())) {
					resultTMap->operator[](affectElem.first).insert(neighbourT);
				}
				resultTMap->operator[](affectElem.first).insert(toBeVisitedNodes.front());
				toBeVisitedNodes.pop();
				continue;
			}
			neighbours = affectsRelationshipMap[toBeVisitedNodes.front()];
			for (const auto &neighbour : neighbours) {
				if (!visitedNodes.count(neighbour)) {
					toBeVisitedNodes.push(neighbour);
					resultTMap->operator[](toBeVisitedNodes.front()).insert(neighbour);
				}
				else {
					for (const auto &neighbourT : resultTMap->operator[](neighbour)) {
						resultTMap->operator[](toBeVisitedNodes.front()).insert(neighbourT);
						resultTMap->operator[](affectElem.first).insert(neighbourT);
					}
					resultTMap->operator[](toBeVisitedNodes.front()).insert(neighbour);
				}
				resultTMap->operator[](affectElem.first).insert(neighbour);
			}
			visitedNodes.insert(toBeVisitedNodes.front());
			toBeVisitedNodes.pop();
		}
	}
	isAffectsTComputed = true;
	return *resultTMap;
}

bool PKB::getAffectsBoolean(bool isTransitive, int modifierStmtNum, int userStmtNum) {
	if (modifierStmtNum != -1) {
		if (!isTransitive && modifierStmtNum >= smallestAffectsLine && modifierStmtNum <= largestAffectsLine) {
			if (userStmtNum != - 1 && userStmtNum >= smallestAffectedLine && userStmtNum <= largestAffectedLine) {
				if (affectsMap[modifierStmtNum].count(userStmtNum))
					return true;
				return false;
			} else if (userStmtNum == -1) {
				if (affectsMap.count(modifierStmtNum))
					return true;
				return false;
			}
		} else if (modifierStmtNum >= smallestAffectsTLine && modifierStmtNum <= largestAffectsTLine){
			if (userStmtNum != - 1 && userStmtNum >= smallestAffectsTLine && userStmtNum <= largestAffectsTLine) {
				if (affectsTMap[modifierStmtNum].count(userStmtNum))
					return true;
				return false;
			} else if (userStmtNum == -1) {
				if (affectsTMap.count(modifierStmtNum))
					return true;
				return false;
			}
		}
	} else if (userStmtNum != -1) {
			if (!isTransitive) {
				if (userStmtNum >= smallestAffectedLine && userStmtNum <= largestAffectedLine) {
					if (affectedMap.count(userStmtNum))
						return true;
					return false;
				}
			} else {
				if (userStmtNum >= smallestAffectsTLine && userStmtNum <= largestAffectsTLine) {
					if (affectedTMap.count(userStmtNum))
						return true;
					return false;
				}
			}
		}

	std::vector<std::unordered_map<std::string, std::unordered_set<int>>> modMaps(allStmts.size() + 1);
	std::unordered_map<int, std::unordered_map<std::string, std::unordered_set<int>>> prevModMap;
	std::unordered_map<std::string, std::unordered_set<int>> *currModMap;
	std::unordered_map<int, std::unordered_set<int>> affectsRelationshipMap, *resultTMap;
	std::unordered_set<int> visitedLines, prevLines, visitedNodes, neighbours;
	std::unordered_set<std::string> varsModified, varsUsed;
	int currLine, prevLine, maxLine = 0;
	std::set<int> toBeVisited;
	std::string varModified;
	std::queue<int> toBeVisitedNodes;
	bool isNewProc = false;

	// Navigation of nextMap
	if (allStmts.size() == 0)
		return false;
	if (modifierStmtNum != -1) {
		toBeVisited.insert(modifierStmtNum);
		visitedLines.insert(modifierStmtNum);
	}
	else {
		toBeVisited.insert(1);
		visitedLines.insert(1);
	}
	while (visitedLines.size() != allStmts.size()) {
		// For moving on to the next proc in the code
		if (toBeVisited.size() == 0) {
			toBeVisited.insert(maxLine + 1);
			isNewProc = true;
		}
		// Earlier lines will always be visited first
		currLine = *toBeVisited.begin();
		if (!allStmts.count(currLine))
			break;
		toBeVisited.erase(toBeVisited.begin());
		visitedLines.insert(currLine);
		if (currLine > maxLine)
			maxLine = currLine;

		// Check if pkb has travsersed past userStmtNum
		if (userStmtNum != -1 && currLine > userStmtNum && getParentTOf(currLine, WHILE).size() == 0) {
			if (isTransitive)
				break;
			if (modifierStmtNum != -1) {
				if (affectsMap.count(modifierStmtNum) && affectsMap[modifierStmtNum].count(userStmtNum))
					return true;
				return false;
			}
			else {
				if (affectedMap.count(userStmtNum) && affectedMap[userStmtNum].count(modifierStmtNum))
					return true;
				return false;
			}
		}

		// New modMap each time pkb traverses to the currLine
		// So outdated values do not stay in modMap
		modMaps[currLine] = {};

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
						if (smallestAffectsLine > modLine)
							smallestAffectsLine = modLine;
						if (largestAffectsLine < modLine)
							largestAffectsLine = modLine;
						if (smallestAffectedLine > currLine)
							smallestAffectedLine = currLine;
						if (largestAffectedLine < currLine)
							largestAffectedLine = modLine;
						if (modifierStmtNum != -1 && userStmtNum != -1) {
							if (modLine == modifierStmtNum && currLine == userStmtNum)
								return true;
						} else if (modifierStmtNum != -1 && userStmtNum == -1) {
							if (modLine == modifierStmtNum)
								return true;
						} else if (modifierStmtNum == -1 && userStmtNum != -1) {
							if (currLine == userStmtNum)
								return true;
						} else {
							return true;
						}
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
				currModMap->erase(variableModified);
			}
		} else if(isWhileStmt(currLine)) {
				prevModMap[currLine] = *currModMap;
		}
	}

	if ((modifierStmtNum == 1 || modifierStmtNum == -1) && (userStmtNum == allStmts.size()) || userStmtNum == -1) {
		isAffectsComputed = true;
	}

	if (!isTransitive) {
		return false;
	}
	if (modifierStmtNum != -1 || userStmtNum == -1) {
		affectsRelationshipMap = affectsMap;
		resultTMap = &affectsTMap;
	}
	else {
		affectsRelationshipMap = affectedMap;
		resultTMap = &affectedTMap;
	}
	for (const auto &affectElem : affectsRelationshipMap) {
		if (affectElem.first < smallestAffectsTLine)
			smallestAffectsTLine = affectElem.first;
		if (affectElem.first > largestAffectsTLine)
			largestAffectsTLine = affectElem.first;
		visitedNodes = {};
		for (const auto &neighbour : affectElem.second) {
			toBeVisitedNodes.push(neighbour);
			resultTMap->operator[](affectElem.first).insert(neighbour);
		}
		visitedNodes.insert(affectElem.first);
		while(toBeVisitedNodes.size() != 0) {
			if (visitedNodes.count(toBeVisitedNodes.front())) {
				for (const auto &neighbourT : resultTMap->operator[](toBeVisitedNodes.front())) {
					resultTMap->operator[](affectElem.first).insert(neighbourT);
				}
				resultTMap->operator[](affectElem.first).insert(toBeVisitedNodes.front());
				toBeVisitedNodes.pop();
				continue;
			}
			neighbours = affectsRelationshipMap[toBeVisitedNodes.front()];
			for (const auto &neighbour : neighbours) {
				if (!visitedNodes.count(neighbour)) {
					toBeVisitedNodes.push(neighbour);
					resultTMap->operator[](toBeVisitedNodes.front()).insert(neighbour);
				}
				else {
					for (const auto &neighbourT : resultTMap->operator[](neighbour)) {
						resultTMap->operator[](toBeVisitedNodes.front()).insert(neighbourT);
						resultTMap->operator[](affectElem.first).insert(neighbourT);
					}
					resultTMap->operator[](toBeVisitedNodes.front()).insert(neighbour);
				}
				resultTMap->operator[](affectElem.first).insert(neighbour);
			}
			visitedNodes.insert(toBeVisitedNodes.front());
			toBeVisitedNodes.pop();
		}
	}

	if ((modifierStmtNum == 1 || modifierStmtNum == -1) && (userStmtNum == allStmts.size()) || userStmtNum == -1) {
		isAffectsTComputed = true;
	}

	if (modifierStmtNum != -1) {
		if (userStmtNum == -1) {
			if (resultTMap->count(modifierStmtNum))
				return true;
			return false;
		} else {
			if (resultTMap->operator[](modifierStmtNum).count(userStmtNum))
				return true;
			return false;
		}
	} else {
		if (userStmtNum == -1) {
			if (resultTMap->size())
				return true;
			return false;
		} else {
			if (resultTMap->operator[](userStmtNum).size())
				return true;
			return false;
		}
	}
	return false;
}

std::unordered_set<int> PKB::getAffectsSet(bool isAffects, bool isTransitive, int modifierStmtNum, int userStmtNum) {
	std::unordered_set<int> resultSet;
	std::unordered_map<int, std::unordered_set<int>> affectsRelationshipMap = getAffectsMap(isTransitive, isAffects);
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
	return {};
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

std::unordered_map<int, std::unordered_set<int>> PKB::getModifierUserPairs() {
	return getAffectsMap(false, true);
}

std::unordered_map<int, std::unordered_set<int>> PKB::getModifierUserTPairs() {
	return getAffectsMap(true, true);
}

std::unordered_set<int> PKB::getModifierStmts() {
	return getAffectsSet(true, false, -1, -1);
}

std::unordered_set<int> PKB::getUserStmts() {
	return getAffectsSet(false, false, -1, -1);
}

std::unordered_set<int> PKB::getModifierOf(int userStmtNum) {
	return getAffectsSet(false, false, -1, userStmtNum);
}

std::unordered_set<int> PKB::getModifierTOf(int userStmtNum) {
	return getAffectsSet(false, true, -1, userStmtNum);
}

std::unordered_set<int> PKB::getUserOf(int modifierStmtNum) {
	return getAffectsSet(true, false, modifierStmtNum, -1);
}

std::unordered_set<int> PKB::getUserTOf(int modifierStmtNum) {
	return getAffectsSet(true, true, modifierStmtNum, -1);
}

void PKB::clearAffectsCache() {
	smallestAffectsLine = INT_MAX;
	largestAffectsLine = INT_MIN;
	smallestAffectedLine = INT_MAX;
	largestAffectedLine = INT_MIN;
	affectsMap = {};
	affectedMap = {};
	affectsTMap = {};
	affectedTMap = {};
	isAffectsComputed = false;
	isAffectsTComputed = false;
}

std::unordered_set<int> PKB::getSwitchStmts() {
	return switchStmts;
}

bool PKB::isSwitchStmt(int stmtNum) {
	if (switchStmts.count(stmtNum))
		return true;
	return false;
}

std::unordered_map<int, std::unordered_set<std::string>> PKB::getSwitchControlVarPair() {
	return switchControlStmtMap;
}

std::unordered_set<int> PKB::getSwitchWithControlVar(std::string controlVar) {
	return switchControlVarMap[controlVar];
}