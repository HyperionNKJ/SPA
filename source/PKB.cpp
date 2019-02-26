#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <ctype.h>
#include <unordered_set>
#include <unordered_map>

#include "PKB.h"
#include "../SPA/Type.h"

using namespace std;

// Frontend APIs
bool PKB::insertVar(string varName)
{
	if (!varSet.count(varName))
	{
		varSet.insert(varName);
		varTableByIdx.push_back(varName);
		varTableByName.insert({varName, varTableByIdx.size() - 1});
		return true;
	}
	return false;
}

bool PKB::insertConstant(int constant)
{
	return constSet.insert(constant).second;
}

bool PKB::insertProc(string procName)
{
	if (!procTableByName.count(procName))
	{
		procSet.insert(procName);
		procTableByIdx.push_back(procName);
		procTableByName.insert({procName, procTableByIdx.size() - 1});
		return true;
	}
	return false;
}

bool PKB::insertStmtType(int stmtNum, Type type)
{
	bool isValidStmt;
	unordered_set<int> *typedStmtSet;
	isValidStmt = allStmts.insert(stmtNum).second;

	if (isValidStmt)
	{
		switch (type)
		{
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
		}
		isValidStmt = typedStmtSet->insert(stmtNum).second;
	}

	return isValidStmt;
}

bool PKB::setFollows(int leader, int follower)
{
	return leaderMap.insert({leader, follower}).second;
}

bool PKB::setFollowsT(int leader, int follower)
{
	return leaderTMap[leader].insert(follower).second;
}

bool PKB::setFollowedBy(int leader, int follower)
{
	return followerMap.insert({follower, leader}).second;
}

bool PKB::setFollowedByT(int leader, int follower)
{
	return followerTMap[follower].insert(leader).second;
}

bool PKB::setParent(int parent, int child)
{
	return parentMap[parent].insert(child).second;
}

bool PKB::setParentT(int parent, int child)
{
	return parentTMap[parent].insert(child).second;
}

bool PKB::setChild(int parent, int child)
{
	return childrenMap.insert({child, parent}).second;
}

bool PKB::setChildT(int parent, int child)
{
	return childrenTMap[child].insert(parent).second;
}

bool PKB::setModifies(int stmtNum, string varName)
{
	modifiesByStmtNumMap[stmtNum].insert(varName);
	return modifiesByVarMap[varName].insert(stmtNum).second;
}

bool PKB::setModifies(string procName, string varName)
{
	return modifiesByProcMap[procName].insert(varName).second;
}

bool PKB::setUses(int stmtNum, string varName)
{
	usesByStmtNumMap[stmtNum].insert(varName);
	return usesByVarMap[varName].insert(stmtNum).second;
}

bool PKB::setUses(string procName, string varName)
{
	return usesByProcMap[procName].insert(varName).second;
}

bool PKB::insertAssignStmt(int stmtNum, string var, vector<string> assignmentStmt)
{
	bool isVarConst, isSuccessfulInsert;

	isSuccessfulInsert = assignModifiedVarMap[var].insert(stmtNum).second;
	assignStmtVarMap.insert({stmtNum, var});
	if (isSuccessfulInsert)
	{
		for (const auto &elem : assignmentStmt)
		{
			isVarConst = true;
			for (const char &c : elem)
			{
				if (!isalnum(c))
					isVarConst = false;
				break;
			}
			if (isVarConst)
			{
				assignModifyingVarMap[elem].insert(stmtNum);
				assignUseVarMap[stmtNum].insert(elem);
			}
		}
	}

	return isSuccessfulInsert;
}

// PQL APIs
unordered_set<int> PKB::getAllStmts()
{
	return allStmts;
}

unordered_set<int> PKB::getReadStmts()
{
	return readStmts;
}

unordered_set<int> PKB::getPrintStmts()
{
	return printStmts;
}

unordered_set<int> PKB::getWhileStmts()
{
	return whileStmts;
}

unordered_set<int> PKB::getIfStmts()
{
	return ifStmts;
}

unordered_set<int> PKB::getAssignStmts()
{
	return assignStmts;
}

unordered_set<string> PKB::getAllVariables()
{
	return varSet;
}

unordered_set<int> PKB::getAllConstant()
{
	return constSet;
}

unordered_set<string> PKB::getAllProcedures()
{
	return procSet;
}

unordered_map<string, int> PKB::getVarTable()
{
	return varTableByName;
}

bool PKB::isReadStmt(int stmtNum)
{
	if (readStmts.count(stmtNum))
	{
		return true;
	}
	return false;
}

bool PKB::isWhileStmt(int stmtNum)
{
	if (whileStmts.count(stmtNum))
	{
		return true;
	}
	return false;
}

bool PKB::isIfStmt(int stmtNum)
{
	if (ifStmts.count(stmtNum))
	{
		return true;
	}
	return false;
}

bool PKB::isAssignStmt(int stmtNum)
{
	if (assignStmts.count(stmtNum))
	{
		return true;
	}
	return false;
}

bool PKB::isPrintStmt(int stmtNum)
{
	if (printStmts.count(stmtNum))
	{
		return true;
	}
	return false;
}

int PKB::getVarIdx(string varName)
{
	return varTableByName[varName];
}

string PKB::getVarAtIdx(int varIdx)
{
	return varTableByIdx[varIdx];
}

int PKB::getProcIdx(string procName)
{
	return procTableByName[procName];
}

string PKB::getProcAtIdx(int procIdx)
{
	return procTableByIdx[procIdx];
}

bool PKB::isModifies(int stmtNum, string varName)
{
	if (allStmts.count(stmtNum))
	{
		if (modifiesByStmtNumMap[stmtNum].count(varName))
			return true;
	}
	return false;
}

bool PKB::doesStmtModifies(int stmtNum)
{
	if (allStmts.count(stmtNum))
	{
		if (modifiesByStmtNumMap[stmtNum].size())
			return true;
	}
	return false;
}

unordered_set<string> PKB::getVarModifiedByStmt(int stmtNum)
{
	return modifiesByStmtNumMap[stmtNum];
}

unordered_set<int> PKB::getStmtsThatModifiesVar(string varName, Type type)
{
	unordered_set<int> allModifyingStmts = modifiesByVarMap[varName], typedStmtSet, resultSet;

	switch (type)
	{
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

	for (const auto &elem : allModifyingStmts)
	{
		if (typedStmtSet.count(elem))
		{
			resultSet.insert(elem);
		}
	}

	return resultSet;
}

unordered_map<int, unordered_set<string>> PKB::getModifiesStmtVarPairs(Type type)
{
	unordered_set<int> typedStmtSet;
	unordered_map<int, unordered_set<string>> resultSet;

	switch (type)
	{
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

	for (const auto &elem : typedStmtSet)
		resultSet.insert({elem, modifiesByStmtNumMap[elem]});

	return resultSet;
}

unordered_set<int> PKB::getStmtsThatModifiesVar(Type type)
{
	unordered_set<int> typedStmtSet;
	unordered_set<int> resultSet;

	switch (type)
	{
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

	for (const auto &elem : typedStmtSet)
	{
		if (modifiesByStmtNumMap.count(elem))
			resultSet.insert(elem);
	}

	return resultSet;
}

bool PKB::isUses(int stmtNum, string varName)
{
	if (allStmts.count(stmtNum))
	{
		if (usesByStmtNumMap[stmtNum].count(varName))
			return true;
	}
	return false;
}

bool PKB::doesStmtUses(int stmtNum)
{
	if (allStmts.count(stmtNum))
	{
		if (usesByStmtNumMap[stmtNum].size())
			return true;
	}
	return false;
}

unordered_set<string> PKB::getVarUsedByStmt(int stmtNum)
{
	return usesByStmtNumMap[stmtNum];
}

unordered_set<int> PKB::getStmtsThatUsesVar(string varName, Type type)
{
	unordered_set<int> allUsesStmts = usesByVarMap[varName], typedStmtSet, resultSet;

	switch (type)
	{
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

	for (const auto &elem : allUsesStmts)
	{
		if (typedStmtSet.count(elem))
		{
			resultSet.insert(elem);
		}
	}

	return resultSet;
}

unordered_map<int, unordered_set<string>> PKB::getUsesStmtVarPairs(Type type)
{
	unordered_set<int> typedStmtSet;
	unordered_map<int, unordered_set<string>> resultSet;

	switch (type)
	{
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

	for (const auto &elem : typedStmtSet)
		resultSet.insert({elem, usesByStmtNumMap[elem]});

	return resultSet;
}

unordered_set<int> PKB::getStmtsThatUsesVar(Type type)
{
	unordered_set<int> typedStmtSet;
	unordered_set<int> resultSet;

	switch (type)
	{
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

	for (const auto &elem : typedStmtSet)
	{
		if (usesByStmtNumMap.count(elem))
			resultSet.insert(elem);
	}

	return resultSet;
}

bool PKB::isParent(int stmtNum1, int stmtNum2)
{
	if (parentMap.count(stmtNum1) && parentMap[stmtNum1].count(stmtNum2))
		return true;
	return false;
}

bool PKB::isParentT(int stmtNum1, int stmtNum2)
{
	if (parentTMap.count(stmtNum1) && parentTMap[stmtNum1].count(stmtNum2))
		return true;
	return false;
}

bool PKB::hasChildren(int stmtNum)
{
	if (parentMap.count(stmtNum))
		return true;
	return false;
}

bool PKB::hasParent(int stmtNum)
{
	if (childrenMap.count(stmtNum))
		return true;
	return false;
}

unordered_map<int, unordered_set<int>> PKB::getParentChildrenPairs(Type parentType, Type childrenType)
{
	unordered_set<int> parentTypedStmtSet, childrenTypedStmtSet;
	unordered_map<int, unordered_set<int>> resultMap;

	switch (parentType)
	{
	case STATEMENT:
		parentTypedStmtSet = allStmts;
		break;
	case READ:
		parentTypedStmtSet = readStmts;
		break;
	case PRINT:
		parentTypedStmtSet = printStmts;
		break;
	case WHILE:
		parentTypedStmtSet = whileStmts;
		break;
	case IF:
		parentTypedStmtSet = ifStmts;
		break;
	case ASSIGN:
		parentTypedStmtSet = assignStmts;
		break;
	}
	switch (childrenType)
	{
	case STATEMENT:
		childrenTypedStmtSet = allStmts;
		break;
	case READ:
		childrenTypedStmtSet = readStmts;
		break;
	case PRINT:
		childrenTypedStmtSet = printStmts;
		break;
	case WHILE:
		childrenTypedStmtSet = whileStmts;
		break;
	case IF:
		childrenTypedStmtSet = ifStmts;
		break;
	case ASSIGN:
		childrenTypedStmtSet = assignStmts;
		break;
	}
	for (const auto &elem : parentTypedStmtSet)
	{
		if (parentMap.count(elem))
		{
			for (const auto &elem2 : parentMap[elem])
			{
				if (childrenTypedStmtSet.count(elem2))
				{
					resultMap[elem].insert(elem2);
				}
			}
		}
	}

	return resultMap;
}

unordered_map<int, unordered_set<int>> PKB::getParentChildrenTPairs(Type parentType, Type childrenType)
{
	unordered_set<int> parentTypedStmtSet, childrenTypedStmtSet;
	unordered_map<int, unordered_set<int>> resultMap;

	switch (parentType)
	{
	case STATEMENT:
		parentTypedStmtSet = allStmts;
		break;
	case READ:
		parentTypedStmtSet = readStmts;
		break;
	case PRINT:
		parentTypedStmtSet = printStmts;
		break;
	case WHILE:
		parentTypedStmtSet = whileStmts;
		break;
	case IF:
		parentTypedStmtSet = ifStmts;
		break;
	case ASSIGN:
		parentTypedStmtSet = assignStmts;
		break;
	}
	switch (childrenType)
	{
	case STATEMENT:
		childrenTypedStmtSet = allStmts;
		break;
	case READ:
		childrenTypedStmtSet = readStmts;
		break;
	case PRINT:
		childrenTypedStmtSet = printStmts;
		break;
	case WHILE:
		childrenTypedStmtSet = whileStmts;
		break;
	case IF:
		childrenTypedStmtSet = ifStmts;
		break;
	case ASSIGN:
		childrenTypedStmtSet = assignStmts;
		break;
	}
	for (const auto &elem : parentTypedStmtSet)
	{
		if (parentTMap.count(elem))
		{
			for (const auto &elem2 : parentTMap[elem])
			{
				if (childrenTypedStmtSet.count(elem2))
				{
					resultMap[elem].insert(elem2);
				}
			}
		}
	}

	return resultMap;
}

unordered_set<int> PKB::getParentStmts(Type parentType)
{
	unordered_set<int> typedStmtSet, resultSet;

	switch (parentType)
	{
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

	for (const auto &elem : parentMap)
	{
		if (typedStmtSet.count(elem.first))
		{
			resultSet.insert(elem.first);
		}
	}

	return resultSet;
}

unordered_set<int> PKB::getChildrenStmts(Type childrenType)
{
	unordered_set<int> typedStmtSet, resultSet;

	switch (childrenType)
	{
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

	for (const auto &elem : childrenMap)
	{
		if (typedStmtSet.count(elem.first))
		{
			resultSet.insert(elem.first);
		}
	}

	return resultSet;
}

int PKB::getParentOf(int stmtNum, Type parentType)
{
	unordered_set<int> typedStmtSet;

	switch (parentType)
	{
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

	if (childrenMap.count(stmtNum) && typedStmtSet.count(childrenMap[stmtNum]))
	{
		return childrenMap[stmtNum];
	}
	return -1;
}

unordered_set<int> PKB::getParentTOf(int stmtNum, Type parentType)
{
	unordered_set<int> typedStmtSet, resultSet;

	switch (parentType)
	{
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

	if (childrenTMap.count(stmtNum))
	{
		for (const auto &elem : childrenTMap[stmtNum])
		{
			if (typedStmtSet.count(stmtNum))
			{
				resultSet.insert(stmtNum);
			}
		}
	}

	return resultSet;
}

unordered_set<int> PKB::getChildrenOf(int stmtNum, Type childrenType)
{
	unordered_set<int> typedStmtSet, resultSet;

	switch (childrenType)
	{
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

	if (parentMap.count(stmtNum))
	{
		for (const auto &elem : parentMap[stmtNum])
		{
			if (typedStmtSet.count(stmtNum))
			{
				resultSet.insert(stmtNum);
			}
		}
	}

	return resultSet;
}

unordered_set<int> PKB::getChildrenTOf(int stmtNum, Type childrenType)
{
	unordered_set<int> typedStmtSet, resultSet;

	switch (childrenType)
	{
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

	if (parentTMap.count(stmtNum))
	{
		for (const auto &elem : parentTMap[stmtNum])
		{
			if (typedStmtSet.count(stmtNum))
			{
				resultSet.insert(stmtNum);
			}
		}
	}

	return resultSet;
}

bool PKB::isFollows(int stmtNum1, int stmtNum2)
{
	if (leaderMap.count(stmtNum1) && leaderMap[stmtNum1] == stmtNum2)
		return true;
	return false;
}

bool PKB::isFollowsT(int stmtNum1, int stmtNum2)
{
	if (leaderTMap.count(stmtNum1) && leaderTMap[stmtNum1].count(stmtNum2))
		return true;
	return false;
}

bool PKB::hasFollower(int stmtNum)
{
	if (leaderMap.count(stmtNum))
		return true;
	return false;
}

bool PKB::hasLeader(int stmtNum)
{
	if (followerMap.count(stmtNum))
		return true;
	return false;
}

unordered_map<int, int> PKB::getLeaderFollowerPairs(Type leaderType, Type followerType)
{
	unordered_set<int> leaderTypedStmtSet, followerTypedStmtSet;
	unordered_map<int, int> resultMap;

	switch (leaderType)
	{
	case STATEMENT:
		leaderTypedStmtSet = allStmts;
		break;
	case READ:
		leaderTypedStmtSet = readStmts;
		break;
	case PRINT:
		leaderTypedStmtSet = printStmts;
		break;
	case WHILE:
		leaderTypedStmtSet = whileStmts;
		break;
	case IF:
		leaderTypedStmtSet = ifStmts;
		break;
	case ASSIGN:
		leaderTypedStmtSet = assignStmts;
		break;
	}
	switch (followerType)
	{
	case STATEMENT:
		followerTypedStmtSet = allStmts;
		break;
	case READ:
		followerTypedStmtSet = readStmts;
		break;
	case PRINT:
		followerTypedStmtSet = printStmts;
		break;
	case WHILE:
		followerTypedStmtSet = whileStmts;
		break;
	case IF:
		followerTypedStmtSet = ifStmts;
		break;
	case ASSIGN:
		followerTypedStmtSet = assignStmts;
		break;
	}
	for (const auto &elem : leaderTypedStmtSet)
	{
		if (leaderMap.count(elem) && followerTypedStmtSet.count(leaderMap[elem]))
			resultMap.insert({elem, leaderMap[elem]});
	}

	return resultMap;
}

unordered_map<int, unordered_set<int>> PKB::getLeaderFollowerTPairs(Type leaderType, Type followerType)
{
	unordered_set<int> leaderTypedStmtSet, followerTypedStmtSet;
	unordered_map<int, unordered_set<int>> resultMap;

	switch (leaderType)
	{
	case STATEMENT:
		leaderTypedStmtSet = allStmts;
		break;
	case READ:
		leaderTypedStmtSet = readStmts;
		break;
	case PRINT:
		leaderTypedStmtSet = printStmts;
		break;
	case WHILE:
		leaderTypedStmtSet = whileStmts;
		break;
	case IF:
		leaderTypedStmtSet = ifStmts;
		break;
	case ASSIGN:
		leaderTypedStmtSet = assignStmts;
		break;
	}
	switch (followerType)
	{
	case STATEMENT:
		followerTypedStmtSet = allStmts;
		break;
	case READ:
		followerTypedStmtSet = readStmts;
		break;
	case PRINT:
		followerTypedStmtSet = printStmts;
		break;
	case WHILE:
		followerTypedStmtSet = whileStmts;
		break;
	case IF:
		followerTypedStmtSet = ifStmts;
		break;
	case ASSIGN:
		followerTypedStmtSet = assignStmts;
		break;
	}
	for (const auto &elem : leaderTypedStmtSet)
	{
		if (leaderTMap.count(elem))
		{
			for (const auto &elem2 : leaderTMap[elem])
			{
				if (followerTypedStmtSet.count(elem2))
				{
					resultMap[elem].insert(elem2);
				}
			}
		}
	}

	return resultMap;
}

unordered_set<int> PKB::getLeaderStmts(Type leaderType)
{
	unordered_set<int> typedStmtSet, resultSet;

	switch (leaderType)
	{
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

	for (const auto &elem : leaderMap)
	{
		if (typedStmtSet.count(elem.first))
		{
			resultSet.insert(elem.first);
		}
	}

	return resultSet;
}

unordered_set<int> PKB::getFollowerStmts(Type followerType)
{
	unordered_set<int> typedStmtSet, resultSet;

	switch (followerType)
	{
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

	for (const auto &elem : followerMap)
	{
		if (typedStmtSet.count(elem.first))
		{
			resultSet.insert(elem.first);
		}
	}

	return resultSet;
}

int PKB::getLeaderOf(int stmtNum, Type leaderType)
{
	unordered_set<int> typedStmtSet;

	switch (leaderType)
	{
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

	if (followerMap.count(stmtNum) && typedStmtSet.count(followerMap[stmtNum]))
	{
		return followerMap[stmtNum];
	}
	return -1;
}

unordered_set<int> PKB::getLeaderTOf(int stmtNum, Type leaderType)
{
	unordered_set<int> typedStmtSet, resultSet;

	switch (leaderType)
	{
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

	if (followerTMap.count(stmtNum))
	{
		for (const auto &elem : followerTMap[stmtNum])
		{
			if (typedStmtSet.count(stmtNum))
			{
				resultSet.insert(stmtNum);
			}
		}
	}

	return resultSet;
}

int PKB::getFollowerOf(int stmtNum, Type followerType)
{
	unordered_set<int> typedStmtSet;
	int result = -1;

	switch (followerType)
	{
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

	if (leaderMap.count(stmtNum) && typedStmtSet.count(leaderMap[stmtNum]))
		result = leaderMap[stmtNum];

	return result;
}

unordered_set<int> PKB::getFollowerTOf(int stmtNum, Type followerType)
{
	unordered_set<int> typedStmtSet, resultSet;

	switch (followerType)
	{
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

	if (leaderTMap.count(stmtNum))
	{
		for (const auto &elem : leaderTMap[stmtNum])
		{
			if (typedStmtSet.count(elem))
			{
				resultSet.insert(elem);
			}
		}
	}

	return resultSet;
}

unordered_set<int> PKB::getAssignStmtsThatUse(string entity)
{
	return assignModifyingVarMap[entity];
}

bool PKB::doesAssignStmtUse(int stmtNum, string entity)
{
	if (assignModifyingVarMap.count(entity) && assignModifyingVarMap[entity].count(stmtNum))
		return true;
	return false;
}

string PKB::getVarModifiedByAssignStmt(int stmtNum)
{
	if (assignStmtVarMap.count(stmtNum))
	{
		return assignStmtVarMap[stmtNum];
	}
	return "";
}

unordered_set<int> PKB::getAssignStmtsThatModifiesVar(string varName)
{
	return assignModifiedVarMap[varName];
}
