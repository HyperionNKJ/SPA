#pragma once

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <regex>
#include <vector>
#include <string>
#include <stack>
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
	
	//trackers for parent, follows
	vector<int> parentVector = vector<int>();
	vector<int> currentFollowVector = vector<int>();
	vector<vector<int>> allFollowStack = vector<vector<int>>();
	vector<Container> containerTracker = vector<Container>();
	
	//trackers for calls
	unordered_map<string, int> procCalledByTable;
	
	//trackers for next
	bool firstInProc = false;
	bool firstInElse = false;
	vector<int> lastInIfTracker = vector<int>();
	vector<int> lastInElseTracker = vector<int>();
	int closedIfCount = 0;
	

private:
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

	bool checkProcedure(string);
	int handleProcedure(string);
	
	bool checkAssignment(string);
	bool checkExpr(string);
	bool checkTerm(string);
	bool checkFactor(string);
	int handleAssignment(string);
	
	bool checkRead(string);
	int handleRead(string);
	bool checkPrint(string);
	int handlePrint(string);
	
	bool checkWhile(string);
	int handleWhile(string);
	bool checkIf(string);
	int handleIf(string);
	bool checkElse(string);
	int handleElse(string);
	bool checkCondExpr(string);
	bool checkRelExpr(string);
	bool checkRelFactor(string);

	bool checkCall(string);
	int handleCall(string);

	int handleCloseBracket(string);

	Statement_Key getStatementIntent(string);
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
	unordered_map<string, int> getProcCalledByTable();
	string getErrorMessage();
};