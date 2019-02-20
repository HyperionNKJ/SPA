#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include "../SPA/Type.h" // ### Included global enum header file.  ### Changed all StmtType -> Type in this file

using namespace std;
typedef short PROC;

// class TNode;

class VarTable; // no need to #include "VarTable.h" as all I need is pointer

class PKB
{
public:
	// Frontend APIs
	static unordered_set<string> varTable, procTable;
	static unordered_set<int> constTable;

	static bool insertVar(string VarName);
	static bool insertConstant(int constant);
	static bool insertProc(string procName);
	static bool insertStmtType(int stmtNum, Type type); 

	bool setFollows(int leader, int follower);
	bool setFollowsT(int leader, int follower);
	bool setFollowedBy(int leader, int follower);
	bool setFollowedByT(int leader, int follower);

	bool setParent(int parent, int child);
	bool setParentT(int parent, int child);
	bool setChild(int parent, int child);
	bool setChildT(int parent, int child);

	bool setModifies(int stmtNum, string varName);
	bool setModifies(string procName, string varName);

	bool setUses(int stmtNum, string varName);
	bool setUses(string procName, string varName);

	bool insertAssignStmt(int stmtNum, string variable, string assignmentStmt);

	// PQL APIs
	unordered_set<int> getAllStmts(); // ### Added new getter API
	unordered_set<int> getReadStmts();
	unordered_set<int> getPrintStmts();
	unordered_set<int> getWhileStmts();
	unordered_set<int> getIfStmts();
	unordered_set<int> getAssignStmts();
	unordered_set<int> getAllVariables(); // ### Added new getter API
	unordered_set<int> getAllConstant(); // ### Added new getter API
	unordered_set<int> getAllProcedures(); // ### Added new getter API
	bool isReadStmt(int stmtNum);
	bool isWhileStmt(int stmtNum);
	bool isIfStmt(int stmtNum);
	bool isAssignStmt(int stmtNum);
	bool isPrintStmt(int stmtNum);

	int getVarIdx(string varName);
	string getVarAtIdx(int varIndex); // ### Added new API to get variable given its index

	bool isModifies(int stmtNum, string varName);
	bool doesStmtModifies(int stmtNum); // ### Changed parameter name from stmtNumber -> stmtNum
	unordered_set<string> getVarModifiedByStmt(int stmtNum);
	unordered_set<int> getStmtsThatModifiesVar(string varName, Type type);
	unordered_map<int, unordered_set<string>> getModifiesStmtVarPairs(Type type);
	unordered_set<int> getStmtsThatModifiesVar(Type type);

	bool isUses(int stmtNum, string varName);
	bool doesStmtUses(int stmtNum);
	unordered_set<string> getVarUsedByStmt(int stmtNum);
	unordered_set<int> getStmtsThatUsesVar(string varName, Type type);
	unordered_map<int, unordered_set<string>> getUsesStmtVarPairs(Type type);
	unordered_set<int> getStmtsThatUsesVar(Type type);

	bool isParent(int stmtNum1, int stmtNum2);
	bool isParentT(int stmtNum1, int stmtNum2);
	bool hasChildren(int stmtNum);
	bool hasParent(int stmtNum);
	unordered_map<int, unordered_set<int>> getParentChildrenPairs(Type parentType, Type childrenType);
	unordered_map<int, unordered_set<int>> getParentChildrenTPairs(Type parentType, Type childrenType);
	unordered_set<int> getParentStmts(Type parentType);
	unordered_set<int> getChildrenStmts(Type childrenType);
	int getParentOf(int stmtNum, Type parentType);
	unordered_set<int> getParentTOf(int stmtNum, Type parentType);
	unordered_set<int> getChildrenOf(int stmtNum, Type childrenType); // ### Renamed from getChildrenrenOf 
	unordered_set<int> getChildrenTOf(int stmtNum, Type childrenType); // ### Added missing API

	bool isFollows(int stmtNum1, int stmtNum2);
	bool isFollowsT(int stmtNum1, int stmtNum2);
	bool hasFollower(int stmtNum);
	bool hasLeader(int stmtNum); // ### Added missing API
	unordered_map<int, int> getLeaderFollowerPairs(Type leaderType, Type followerType); 
	unordered_map<int, unordered_set<int>> getLeaderFollowerTPairs(Type leaderType, Type followerType);
	unordered_set<int> getLeaderStmts(Type leaderType);
	unordered_set<int> getFollowerStmts(Type followerType);
	int getLeaderOf(int stmtNum, Type leaderType);
	unordered_set<int> getLeaderTOf(int stmtNum, Type leaderType);
	int getFollowerOf(int stmtNum, Type followerType);
	unordered_set<int> getFollowerTOf(int stmtNum, Type followerType);

	unordered_set<int> getAssignStmtsThatUse(string entity);
	bool doesAssignStmtUse(int stmtNum, string entity);
	string getVarModifiedByAssignStmt(int stmtNum);
	unordered_set<int> getAssignStmtsThatModifiesVar(string varName);
};