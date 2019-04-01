#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <unordered_set>
#include <unordered_map>
#include "Type.h"

using namespace std;
typedef short PROC;

class PKB {
private:
	unordered_set<string> varSet, procSet, callSet, calledSet, printSet, readSet, modifiesProcSet, usesProcSet;
	unordered_map<string, int> varTableByName, procTableByName, callTableByName, printTableByName, readTableByName;
	vector<string> varTableByIdx, procTableByIdx, callTableByIdx, printTableByIdx, readTableByIdx;
	unordered_set<int> allStmts, readStmts, printStmts, whileStmts, ifStmts, assignStmts, constSet, callStmts, ifControlStmtSet, whileControlStmtSet, *getTypedStmtSet(Type type);
	unordered_map<int, unordered_set<int>> leaderTMap, followerTMap, parentMap, parentTMap, childrenTMap, nextMap, prevMap;
	unordered_map<int, int> leaderMap, followerMap, childrenMap;
	unordered_map<string, unordered_set<int>> calledStmtMap, printStmtMap, readStmtMap, modifiesByVarMap, usesByVarMap, patternMap, fullPatternMap, ifControlVarMap, whileControlVarMap;
	unordered_map<int, unordered_set<string>> modifiesByStmtNumMap, usesByStmtNumMap, ifControlStmtMap, whileControlStmtMap;
	unordered_map<string, unordered_set<string>> modifiesByProcMap, varModifiedByProcMap, usesByProcMap, varUsedByProcMap, callMap, calledByMap, callsTMap, calledByTMap;

public:
	// Frontend APIs
	bool insertVar(string VarName);
	bool insertConstant(int constant);
	bool insertProc(string procName);
	bool insertStmtType(int stmtNum, Type type);
	bool insertCPRStmtType(int stmtNum, Type type, string name);
	bool insertPattern(string pattern, int stmtNum);
	bool insertFullPattern(string fullPattern, int stmtNum);
	bool insertIfControlVar(int ifStmtNum, string varName);
	bool insertWhileControlVar(int whileStmtNum, string varName);

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
	unordered_set<int> getCallStmts();
	unordered_set<string> getCallProcNames();
	unordered_set<string> getPrintVarNames();
	unordered_set<string> getReadVarNames();

	bool isReadStmt(int stmtNum);
	bool isWhileStmt(int stmtNum);
	bool isIfStmt(int stmtNum);
	bool isAssignStmt(int stmtNum);
	bool isPrintStmt(int stmtNum);
	bool isCallStmt(int stmtNum);

	int getVarIdx(string varName);
	string getVarAtIdx(int varIdx);
	unordered_map<string, int> getVarTable();

	int getProcIdx(string procName);
	string getProcAtIdx(int procIdx);
	unordered_map<string, int> getProcTable();

	int getCallIdx(string procName);
	string getCallAtIdx(int callIdx);

	int getReadIdx(string varName);
	string getReadAtIdx(int readIdx);

	int getPrintIdx(string varName);
	string getPrintAtIdx(int idx);

	bool isModifies(int stmtNum, string varName);
	bool doesStmtModifies(int stmtNum);
	unordered_set<string> getVarModifiedByStmt(int stmtNum);
	unordered_set<int> getStmtsThatModifiesVar(string varName, Type type);
	unordered_map<int, unordered_set<string>> getModifiesStmtVarPairs(Type type);
	unordered_set<int> getStmtsThatModifiesVar(Type type);

	bool isProcModifies(string procName, string varName);
	bool doesProcModifies(string procName);
	unordered_set<string> getVarModifiedByProc(string procName);
	unordered_map<string, unordered_set<string>> getModifiesProcVarPairs();
	unordered_set<string> getProcThatModifiesVar();
	unordered_set<string> getProcThatModifiesVar(string varName);

	bool isUses(int stmtNum, string varName);
	bool doesStmtUses(int stmtNum);
	unordered_set<string> getVarUsedByStmt(int stmtNum);
	unordered_set<int> getStmtsThatUsesVar(string varName, Type type);
	unordered_map<int, unordered_set<string>> getUsesStmtVarPairs(Type type);
	unordered_set<int> getStmtsThatUsesVar(Type type);

	bool isProcUses(string procName, string varName);
	bool doesProcUses(string procName);
	unordered_set<string> getVarUsedByProc(string procName);
	unordered_map<string, unordered_set<string>> getUsesProcVarPairs();
	unordered_set<string> getProcThatUsesVar();
	unordered_set<string> getProcThatUsesVar(string varName);

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

	string getVarModifiedByAssignStmt(int stmtNum);
	unordered_set<int> getAssignStmtsThatModifiesVar(string varName);
	unordered_set<int> getAssignStmtsWithSubMatch(string subString);
	unordered_set<int> getAssignStmtsWithExactMatch(string exactString);

	bool isCalls(string callerName, string receiverName);
	bool isCallsT(string callerName, string receiverName);
	bool hasReceiver(string callerName);
	bool hasCaller(string receiverName);
	unordered_map<string, unordered_set<string>> getCallerReceiverPairs();
	unordered_map<string, unordered_set<string>> getCallerReceiverTPairs();
	unordered_set<string> getCallerProcedures();
	unordered_set<string> getReceiverProcedures();
	unordered_set<string> getCallerOf(string receiverName);
	unordered_set<string> getCallerTOf(string receiverName);
	unordered_set<string> getReceiverOf(string callerName);
	unordered_set<string> getReceiverTOf(string callerName);

	bool isNext(int prevLineNum, int nextLineNum);
	bool isNextT(int prevLineNum, int nextLineNum);
	bool hasNext(int prevLineNum);
	bool hasPrevious(int nextLineNum);
	bool hasNextT();
	unordered_map<int, unordered_set<int>> getPreviousNextPairs(Type previousType, Type nextType);
	unordered_map<int, unordered_set<int>> getPreviousNextTPairs(Type previousType, Type nextType);
	unordered_set<int> getPreviousLines(Type previousType);
	unordered_set<int> getNextLines(Type nextType);
	unordered_set<int> getPreviousOf(int nextLineNum, Type previousType);
	unordered_set<int> getPreviousTOf(int nextLineNum, Type previousType);
	unordered_set<int> getNextOf(int prevLineNum, Type nextType);
	unordered_set<int> getNextTOf(int prevLineNum, Type nextType);

	unordered_map<int, unordered_set<string>> getIfControlVarPair();
	unordered_set<int> getIfWithControlVar();
	unordered_set<int> getIfWithControlVar(string controlVar);

	unordered_map<int, unordered_set<string>> getWhileControlVarPair();
	unordered_set<int> getWhileWithControlVar();
	unordered_set<int> getWhileWithControlVar(string controlVar);

	unordered_set<int> getCallStmtsWithProc(string procName);
	unordered_set<int> getPrintStmtsWithVar(string varName);
	unordered_set<int> getReadStmtsWithVar(string varName);
};