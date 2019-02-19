#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>

using namespace std;
typedef short PROC;
enum StmtType
{
	ALL,
	READ,
	WHILE,
	IF,
	ASSIGN
};

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
	static bool insertStmtType(int stmtNum, StmtType type);

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
	unordered_set<int> getReadStmts();
	unordered_set<int> getWhileStmts();
	unordered_set<int> getIfStmts();
	unordered_set<int> getAssignStmts();
	unordered_set<int> getPrintStmts();
	bool isReadStmt(int stmtNum);
	bool isWhileStmt(int stmtNum);
	bool isIfStmt(int stmtNum);
	bool isAssignStmt(int stmtNum);
	bool isPrintStmt(int stmtNum);

	int getVarIdx(string varName);
	bool isModifies(int stmtNum, string varName);
	bool doesStmtModifies(int stmtNumber);
	unordered_set<string> getVarModifiedByStmt(int stmtNum);
	unordered_set<int> getStmtsThatModifiesVar(string varName, StmtType type);
	unordered_map<int, unordered_set<string>> getModifiesStmtVarPairs(StmtType type);
	unordered_set<int> getStmtsThatModifiesVar(StmtType type);

	bool isUses(int stmtNum, string varName);
	bool doesStmtUses(int stmtNum);
	unordered_set<string> getVariablesUsedByStmt(int stmtNum);
	unordered_set<int> getStmtsThatUsesVar(string varName, StmtType type);
	unordered_map<int, unordered_set<string>> getUsesStmtVarPairs(StmtType type);
	unordered_set<int> getStmtsThatUsesVar(StmtType);

	bool isParent(int stmtNum1, int stmtNum2);
	bool isParentT(int stmtNum1, int stmtNum2);
	bool hasChildren(int stmtNum);
	bool hasParent(int stmtNum);
	unordered_map<int, unordered_set<int>> getParentChildrenPairs(StmtType parentType, StmtType childrenType);
	unordered_map<int, unordered_set<int>> getParentChildrenTPairs(StmtType parentType, StmtType childrenType);
	unordered_set<int> getParentStmts(StmtType parentType);
	unordered_set<int> getChildrenStmts(StmtType childrenType);
	int getParentOf(int stmtNum, StmtType parentType);
	unordered_set<int> getParentTOf(int stmtNum, StmtType parentType);
	unordered_set<int> getChildrenrenTOf(int stmtNum, StmtType childrenType);

	bool isFollows(int stmtNum1, int stmtNum2);
	bool isFollowsT(int stmtNum1, int stmtNum2);
	bool hasFollower(int stmtNum);
	unordered_map<int, int> getLeaderFollowerPairs(StmtType LeaderType, StmtType FollowerType);
	unordered_map<int, unordered_set<int>> getLeaderFollowerTPairs(StmtType LeaderType, StmtType FollowerType);
	unordered_set<int> getLeaderStmts(StmtType leaderType);
	unordered_set<int> getFollowerStmts(StmtType followerType);
	int getLeaderOf(int stmtNum, StmtType leaderType);
	unordered_set<int> getLeaderTOf(int stmtNum, StmtType leaderType);
	int getFollowerOf(int stmtNum, StmtType followerType);
	unordered_set<int> getFollowerTOf(int stmtNum, StmtType followerType);

	unordered_set<int> getAssignStmtsThatUse(string entity);
	bool doesAssignStmtUse(int stmtNum, string entity);
	string getVarModifiedByAssignStmt(int stmtNum);
	unordered_set<int> getAssignStmtsThatModifiesVar(string varName);
};