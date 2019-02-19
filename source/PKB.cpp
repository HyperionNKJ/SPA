#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>

using namespace std;

#include "PKB.h"
// #include "TNode.h"

unordered_set<int> allStmts, readStmts, whileStmts, ifStmts, assignStmts;
unordered_map<int, unordered_set<int>> leaderMap, leaderTMap, followerMap, followerTMap, parentMap, parentTMap, childMap, childTMap;
unordered_map<int, unordered_set<string>> modifiesByStmtNumMap, usesByStmtNumMap;
unordered_map<string, unordered_set<int>> modifiesByVarMap, usesByVarMap;
unordered_map<string, unordered_set<string>> modifiesByProcMap, usesByProcMap;

bool PKB::insertVar(string VarName)
{
	return PKB::varTable.insert(VarName).second;
}

bool PKB::insertConstant(int constant)
{
	return PKB::constTable.insert(constant).second;
}

bool PKB::insertProc(string procName)
{
	return PKB::procTable.insert(procName).second;
}

bool PKB::insertStmtType(int stmtNum, StmtType type)
{
	bool isValidStmt;
	unordered_set<int> typedStmtSet;
	isValidStmt = allStmts.insert(stmtNum).second;
	if (isValidStmt)
	{
		switch (type)
		{
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
		}
		isValidStmt = typedStmtSet.insert(stmtNum).second;
	}
	return isValidStmt;
}

bool PKB::setFollows(int leader, int follower)
{
	return leaderMap[leader].insert(follower).second;
}

bool PKB::setFollowsT(int leader, int follower)
{
	return leaderTMap[leader].insert(follower).second;
}

bool PKB::setFollowedBy(int leader, int follower)
{
	return followerMap[follower].insert(leader).second;
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
	return childMap[child].insert(parent).second;
}

bool PKB::setChildT(int parent, int child)
{
	return childTMap[child].insert(parent).second;
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
