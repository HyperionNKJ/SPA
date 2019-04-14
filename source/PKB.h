#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <unordered_set>
#include <unordered_map>
#include "Type.h"

typedef short PROC;

// PKB stores information from parser and also provides API for PQL to query the database.
class PKB {
private:
	std::unordered_set<std::string> varSet, procSet, callSet, calledSet, printSet, readSet, modifiesProcSet, usesProcSet;
	std::unordered_map<std::string, int> varTableByName, procTableByName, callTableByName, printTableByName, readTableByName;
	std::vector<std::string> varTableByIdx, procTableByIdx, callTableByIdx, printTableByIdx, readTableByIdx;
	std::unordered_set<int> allStmts, readStmts, printStmts, whileStmts, ifStmts, assignStmts, constSet, callStmts, ifControlStmtSet, whileControlStmtSet, *getTypedStmtSet(Type type), getAffectsSet(bool isAffects, bool isTransitive, int modifierStmtNum, int userStmtNum), switchStmts, switchControlStmtSet;
	std::unordered_map<int, std::unordered_set<int>> leaderTMap, followerTMap, parentMap, parentTMap, childrenTMap, nextMap, prevMap, getAffectsMap(bool isTransitive, bool isAffects), nextTCache, prevTCache, affectsMap, affectedMap, affectsTMap, affectedTMap;
	std::unordered_map<int, int> leaderMap, followerMap, childrenMap;
	std::unordered_map<int, std::string> callMapByStmtNum, printMapByStmtNum, readMapByStmtNum;
	std::unordered_map<std::string, std::unordered_set<int>> calledStmtMap, printStmtMap, readStmtMap, modifiesByVarMap, usesByVarMap, patternMap, fullPatternMap, ifControlVarMap, whileControlVarMap, switchControlVarMap;
	std::unordered_map<int, std::unordered_set<std::string>> modifiesByStmtNumMap, usesByStmtNumMap, ifControlStmtMap, whileControlStmtMap, switchControlStmtMap;
	std::unordered_map<std::string, std::unordered_set<std::string>> modifiesByProcMap, varModifiedByProcMap, usesByProcMap, varUsedByProcMap, callMap, calledByMap, callsTMap, calledByTMap;
	bool getAffectsBoolean(bool isTransitive, int modifierStmtNum, int userStmtNum), isAffectsComputed = false, isAffectsTComputed = false;
	int smallestAffectsLine = INT_MAX, smallestAffectsTLine = INT_MAX, largestAffectsLine = INT_MIN, largestAffectsTLine = INT_MIN , smallestAffectedLine = INT_MAX, largestAffectedLine = INT_MIN;

public:
	// Frontend APIs
	bool insertVar(std::string VarName);
	bool insertConstant(int constant);
	bool insertProc(std::string procName);
	bool insertStmtType(int stmtNum, Type type);
	bool insertCPRStmtType(int stmtNum, Type type, std::string name);
	bool insertPattern(std::string pattern, int stmtNum);
	bool insertFullPattern(std::string fullPattern, int stmtNum);
	bool insertIfControlVar(int ifStmtNum, std::string varName);
	bool insertWhileControlVar(int whileStmtNum, std::string varName);
	bool insertSwitchControlVar(int switchStmtNum, std::string varName);

	bool setFollows(int leader, int follower);
	bool setFollowsT(int leader, int follower);
	bool setFollowedBy(int leader, int follower);
	bool setFollowedByT(int leader, int follower);

	bool setParent(int parent, int child);
	bool setParentT(int parent, int child);
	bool setChild(int parent, int child);
	bool setChildT(int parent, int child);

	bool setModifies(int stmtNum, std::string varName);
	bool setModifies(std::string procName, std::string varName);

	bool setUses(int stmtNum, std::string varName);
	bool setUses(std::string procName, std::string varName);

	bool setCalls(std::string proc1, std::string proc2);
	bool setCalledBy(std::string proc1, std::string proc2);
	bool setCallsT(std::string proc1, std::string proc2);
	bool setCalledByT(std::string proc1, std::string proc2);

	bool setNext(int prevLine, int nextLine);
	bool setPrevious(int prevLine, int nextLine);

	// PQL APIs
	std::unordered_set<int> getAllStmts();
	std::unordered_set<int> getReadStmts();
	std::unordered_set<int> getPrintStmts();
	std::unordered_set<int> getWhileStmts();
	std::unordered_set<int> getIfStmts();
	std::unordered_set<int> getAssignStmts();
	std::unordered_set<std::string> getAllVariables();
	std::unordered_set<int> getAllConstant();
	std::unordered_set<std::string> getAllProcedures();
	std::unordered_set<int> getCallStmts();
	std::unordered_set<std::string> getCallProcNames();
	std::unordered_set<std::string> getPrintVarNames();
	std::unordered_set<std::string> getReadVarNames();

	bool isReadStmt(int stmtNum);
	bool isWhileStmt(int stmtNum);
	bool isIfStmt(int stmtNum);
	bool isAssignStmt(int stmtNum);
	bool isPrintStmt(int stmtNum);
	bool isCallStmt(int stmtNum);

	int getVarIdx(std::string varName);
	std::string getVarAtIdx(int varIdx);
	std::unordered_map<std::string, int> getVarTable();

	int getProcIdx(std::string procName);
	std::string getProcAtIdx(int procIdx);
	std::unordered_map<std::string, int> getProcTable();

	int getCallIdx(std::string procName);
	std::string getCallAtIdx(int callIdx);
	std::string getCallAtStmtNum(int stmtNum);

	int getReadIdx(std::string varName);
	std::string getReadAtIdx(int readIdx);
	std::string getReadAtStmtNum(int stmtNum);

	int getPrintIdx(std::string varName);
	std::string getPrintAtIdx(int idx);
	std::string getPrintAtStmtNum(int stmtNum);

	bool isModifies(int stmtNum, std::string varName);
	bool doesStmtModifies(int stmtNum);
	std::unordered_set<std::string> getVarModifiedByStmt(int stmtNum);
	std::unordered_set<int> getStmtsThatModifiesVar(std::string varName, Type type);
	std::unordered_map<int, std::unordered_set<std::string>> getModifiesStmtVarPairs(Type type);
	std::unordered_set<int> getStmtsThatModifiesVar(Type type);

	bool isProcModifies(std::string procName, std::string varName);
	bool doesProcModifies(std::string procName);
	std::unordered_set<std::string> getVarModifiedByProc(std::string procName);
	std::unordered_map<std::string, std::unordered_set<std::string>> getModifiesProcVarPairs();
	std::unordered_set<std::string> getProcThatModifiesVar();
	std::unordered_set<std::string> getProcThatModifiesVar(std::string varName);

	bool isUses(int stmtNum, std::string varName);
	bool doesStmtUses(int stmtNum);
	std::unordered_set<std::string> getVarUsedByStmt(int stmtNum);
	std::unordered_set<int> getStmtsThatUsesVar(std::string varName, Type type);
	std::unordered_map<int, std::unordered_set<std::string>> getUsesStmtVarPairs(Type type);
	std::unordered_set<int> getStmtsThatUsesVar(Type type);

	bool isProcUses(std::string procName, std::string varName);
	bool doesProcUses(std::string procName);
	std::unordered_set<std::string> getVarUsedByProc(std::string procName);
	std::unordered_map<std::string, std::unordered_set<std::string>> getUsesProcVarPairs();
	std::unordered_set<std::string> getProcThatUsesVar();
	std::unordered_set<std::string> getProcThatUsesVar(std::string varName);

	bool isParent(int stmtNum1, int stmtNum2);
	bool isParentT(int stmtNum1, int stmtNum2);
	bool hasChildren(int stmtNum);
	bool hasParent(int stmtNum);
	std::unordered_map<int, std::unordered_set<int>> getParentChildrenPairs(Type parentType, Type childrenType);
	std::unordered_map<int, std::unordered_set<int>> getParentChildrenTPairs(Type parentType, Type childrenType);
	std::unordered_set<int> getParentStmts(Type parentType);
	std::unordered_set<int> getChildrenStmts(Type childrenType);
	int getParentOf(int stmtNum, Type parentType);
	std::unordered_set<int> getParentTOf(int stmtNum, Type parentType);
	std::unordered_set<int> getChildrenOf(int stmtNum, Type childrenType);
	std::unordered_set<int> getChildrenTOf(int stmtNum, Type childrenType);

	bool isFollows(int stmtNum1, int stmtNum2);
	bool isFollowsT(int stmtNum1, int stmtNum2);
	bool hasFollower(int stmtNum);
	bool hasLeader(int stmtNum);
	std::unordered_map<int, int> getLeaderFollowerPairs(Type leaderType, Type followerType);
	std::unordered_map<int, std::unordered_set<int>> getLeaderFollowerTPairs(Type leaderType, Type followerType);
	std::unordered_set<int> getLeaderStmts(Type leaderType);
	std::unordered_set<int> getFollowerStmts(Type followerType);
	int getLeaderOf(int stmtNum, Type leaderType);
	std::unordered_set<int> getLeaderTOf(int stmtNum, Type leaderType);
	int getFollowerOf(int stmtNum, Type followerType);
	std::unordered_set<int> getFollowerTOf(int stmtNum, Type followerType);

	std::string getVarModifiedByAssignStmt(int stmtNum);
	std::unordered_set<int> getAssignStmtsThatModifiesVar(std::string varName);
	std::unordered_set<int> getAssignStmtsWithSubMatch(std::string subString);
	std::unordered_set<int> getAssignStmtsWithExactMatch(std::string exactString);

	bool isCalls(std::string callerName, std::string receiverName);
	bool isCallsT(std::string callerName, std::string receiverName);
	bool hasReceiver(std::string callerName);
	bool hasCaller(std::string receiverName);
	std::unordered_map<std::string, std::unordered_set<std::string>> getCallerReceiverPairs();
	std::unordered_map<std::string, std::unordered_set<std::string>> getCallerReceiverTPairs();
	std::unordered_set<std::string> getCallerProcedures();
	std::unordered_set<std::string> getReceiverProcedures();
	std::unordered_set<std::string> getCallerOf(std::string receiverName);
	std::unordered_set<std::string> getCallerTOf(std::string receiverName);
	std::unordered_set<std::string> getReceiverOf(std::string callerName);
	std::unordered_set<std::string> getReceiverTOf(std::string callerName);

	bool isNext(int prevLineNum, int nextLineNum);
	bool isNextT(int prevLineNum, int nextLineNum);
	bool hasNext(int prevLineNum);
	bool hasPrevious(int nextLineNum);
	bool hasNextT();
	std::unordered_map<int, std::unordered_set<int>> getPreviousNextPairs(Type previousType, Type nextType);
	std::unordered_map<int, std::unordered_set<int>> getPreviousNextTPairs(Type previousType, Type nextType);
	std::unordered_set<int> getPreviousLines(Type previousType);
	std::unordered_set<int> getNextLines(Type nextType);
	std::unordered_set<int> getPreviousOf(int nextLineNum, Type previousType);
	std::unordered_set<int> getPreviousTOf(int nextLineNum, Type previousType);
	std::unordered_set<int> getNextOf(int prevLineNum, Type nextType);
	std::unordered_set<int> getNextTOf(int prevLineNum, Type nextType);
	void clearNextTCache();

	std::unordered_map<int, std::unordered_set<std::string>> getIfControlVarPair();
	std::unordered_set<int> getIfWithControlVar();
	std::unordered_set<int> getIfWithControlVar(std::string controlVar);

	std::unordered_map<int, std::unordered_set<std::string>> getWhileControlVarPair();
	std::unordered_set<int> getWhileWithControlVar();
	std::unordered_set<int> getWhileWithControlVar(std::string controlVar);

	std::unordered_set<int> getCallStmtsWithProc(std::string procName);
	std::unordered_set<int> getPrintStmtsWithVar(std::string varName);
	std::unordered_set<int> getReadStmtsWithVar(std::string varName);

	bool isAffects(int modifierStmtNum, int userStmtNum);
	bool isAffectsT(int modifierStmtNum, int userStmtNum);
	bool hasAffects();
	bool hasUser(int modifierStmtNum);
	bool hasModifier(int userStmtNum);
	std::unordered_map<int, std::unordered_set<int>> getModifierUserPairs();
	std::unordered_map<int, std::unordered_set<int>> getModifierUserTPairs();
	std::unordered_set<int> getModifierStmts();
	std::unordered_set<int> getUserStmts();
	std::unordered_set<int> getModifierOf(int userStmtNum);
	std::unordered_set<int> getModifierTOf(int userStmtNum);
	std::unordered_set<int> getUserOf(int modifierStmtNum);
	std::unordered_set<int> getUserTOf(int modifierStmtNum);
	void clearAffectsCache();

	std::unordered_set<int> getSwitchStmts();
	bool isSwitchStmt(int stmtNum);
	std::unordered_map<int, std::unordered_set<std::string>> getSwitchControlVarPair();
	std::unordered_set<int> getSwitchWithControlVar(std::string controlVar);
};