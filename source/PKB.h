#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include "Type.h"

using namespace std;
typedef short PROC;

class PKB
{
private:
	unordered_set<string> varSet, procSet;
	unordered_map<string, int> varTableByName, procTableByName;
	vector<string> varTableByIdx, procTableByIdx;
	unordered_set<int> allStmts, readStmts, printStmts, whileStmts, ifStmts, assignStmts, constSet;
	unordered_map<int, unordered_set<int>> leaderTMap, followerTMap, parentMap, parentTMap, childrenTMap;
	unordered_map<int, int> leaderMap, followerMap, childrenMap, nextMap, prevMap;
	unordered_map<string, unordered_set<int>> assignModifyingVarMap, assignModifiedVarMap, modifiesByVarMap, usesByVarMap;
	unordered_map<int, string> assignStmtVarMap;
	unordered_map<int, unordered_set<string>> assignUseVarMap, modifiesByStmtNumMap, usesByStmtNumMap;
	unordered_map<string, string> callMap, calledByMap;
	unordered_map<string, unordered_set<string>> modifiesByProcMap, usesByProcMap, callsTMap, calledByTMap;

public:
	// Frontend APIs
	bool insertVar(string VarName);
	bool insertConstant(int constant);
	bool insertProc(string procName);
	bool insertStmtType(int stmtNum, Type type);
	bool insertCPRStmtType(int stmtNum, Type type, string name);

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

	bool insertAssignStmt(int stmtNum, string var, vector<string> assignmentStmt);

	bool setCalls(string proc1, string proc2);
	bool setCalledBy(string proc1, string proc2);
	bool setCallsT(string proc1, string proc2);
	bool setCalledByT(string proc1, string proc2);

	bool setNext(int prevLine, int nextLine);
	bool setPrevious(int prevLine, int nextLine);

	// PQL APIs
	unordered_set<int> getAllStmts();
	unordered_set<int> getReadStmts();
	unordered_set<int> getPrintStmts();
	unordered_set<int> getWhileStmts();
	unordered_set<int> getIfStmts();
	unordered_set<int> getAssignStmts();
	unordered_set<string> getAllVariables();
	unordered_set<int> getAllConstant();
	unordered_set<string> getAllProcedures();
	unordered_map<string, int> getVarTable();

	bool isReadStmt(int stmtNum);
	bool isWhileStmt(int stmtNum);
	bool isIfStmt(int stmtNum);
	bool isAssignStmt(int stmtNum);
	bool isPrintStmt(int stmtNum);

	int getVarIdx(string varName);
	string getVarAtIdx(int varIdx);
	int getProcIdx(string procName);
	string getProcAtIdx(int procIdx);

	bool isModifies(int stmtNum, string varName);
	bool doesStmtModifies(int stmtNum);
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
	unordered_set<int> getChildrenOf(int stmtNum, Type childrenType);
	unordered_set<int> getChildrenTOf(int stmtNum, Type childrenType);

	bool isFollows(int stmtNum1, int stmtNum2);
	bool isFollowsT(int stmtNum1, int stmtNum2);
	bool hasFollower(int stmtNum);
	bool hasLeader(int stmtNum);
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

	int getCallIdx(string procName);
	string getCallAtIdx(int idx);
	int getReadIdx(string varName);
	string getReadAtIdx(int idx);
	int getPrintIdx(string varName);
	string getPrintAtIdx(int idx);

	unordered_set<int> getCallStmts();
	unordered_set<string> getCallProcNames();
	unordered_set<string> getReadVarNames();
	unordered_set<string> getPrintVarNames();

	bool isProcModifies(string procName, string varName);
	bool doesProcModifies(string procName);
	unordered_set<string> getVarModifiedByProc(string procName);
	unordered_map<string, unordered_set<string>> getModifiesProcVarPairs();
	unordered_set<string> getProcThatModifiesVar();
	unordered_set<string> getProcThatModifiesVar(string varName);
	unordered_map<string, int> getProcUsesTable();

	bool isProcUses(string procName, string varName);
	bool doesProcUses(string procName);
	unordered_set<string> getVarUsedByProc(string procName);
	unordered_map<string, unordered_set<string>> getUsesProcVarPairs();
	unordered_set<string> getProcThatUsesVar();
	unordered_set<string> getProcThatUsesVar(string varName);

	bool isNextT(int, int);
	bool hasNextT(int);
	bool hasPreviousT(int);
	unordered_set<int> getNextTOf(int, Type);
	unordered_set<int> getPreviousTOf(int, Type);
	unordered_map<int, unordered_set<int>> getPreviousNextTPairs(Type, Type);
};