#pragma once

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <regex>
#include <vector>
#include <string>
#include <stack>
#include <utility>
#include "PKB.h"
#include "Type.h"
#include "DesignExtractor.h"

enum Container { WHILEC, IFC, ELSEC, NONEC };
enum Statement_Key {
	KEY_PROCEDURE, KEY_ASSIGN, KEY_IF, KEY_ELSE, KEY_WHILE,
	KEY_READ, KEY_PRINT, KEY_CLOSE_BRACKET, KEY_CALL, KEY_ERROR
};

class Parser {
	PKB * pkb;
	DesignExtractor de;

	string errorMessage = "";
	bool withinProcedure = false;
	bool emptyProcedure = true;
	bool expectElse = false;
	string currProcedure;
	vector<string> sourceCode = vector<string>();
	int statementNumber = 1;
	unordered_set<string> procNames = unordered_set<string>();
	
	//trackers for parent, follows
	vector<int> parentVector = vector<int>();
	vector<int> currentFollowVector = vector<int>();
	vector<vector<int>> allFollowStack = vector<vector<int>>();
	vector<Container> containerTracker = vector<Container>();
	
	//trackers for calls
	unordered_map<int, string> procCalledByTable;
	
	//trackers for next
	bool firstInProc = false;
	bool firstInElse = false;
	vector<pair<int, int>>  lastInIfElseTracker = vector<pair<int, int>>();
	int closedIfCount = 0;
	int lastStmtInFlow = -1;

private:
	bool checkProcedure(string);
	bool checkAssignment(string);
	bool checkExpr(string);
	bool checkTerm(string);
	bool checkFactor(string);
	bool checkRead(string);
	bool checkPrint(string);
	bool checkWhile(string);
	bool checkIf(string);
	bool checkElse(string);
	bool checkCondExpr(string);
	bool checkRelExpr(string);
	bool checkRelFactor(string);
	bool checkCall(string);

	Statement_Key getStatementIntent(string);

	bool setParent(int);
	bool setModifies(int, string, string);
	bool setUses(int, string, string);
	bool setCallUsesModifies();
	bool setProcIndirectUsesModifies();
	bool setFollow(int);
	bool setNext(int, Container);
	bool setCalls(string, string);
	bool setCallsT();

	vector<string> loadFile(string);

	string leftTrim(string, string);
	string rightTrim(string, string);
	bool isValidVarName(string);
	bool isValidConstant(string);
	vector<string> tokeniseString(string, string);
public:
	int handleProcedure(string);
	int handleAssignment(string);
	int handleRead(string);
	int handlePrint(string);
	int handleWhile(string);
	int handleIf(string);
	int handleElse(string);
	int handleCall(string);
	int handleCloseBracket(string);

	int parse(string, PKB&);

	//setter/getter functions for testing
	void setPKB(PKB*);
	void setStatementNumber(int);
	void setCurrentFollowVector(vector<int>);
	void setParentVector(vector<int>);
	void setAllFollowStack(vector<vector<int>>);

	int getStatementNumber();
	vector<int> getParentVector();
	vector<int> getCurrentFollowVector();
	vector<vector<int>> getAllFollowStack();
	string getCurrentProcedure();
	unordered_map<int, string> getProcCalledByTable();
	string getErrorMessage();
};