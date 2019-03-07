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

using namespace std;

class Parser {
	PKB * pkb;
	DesignExtractor de;
	bool withinProcedure = false;
	bool emptyProcedure = true;
	bool expectElse = false;
	vector<string> sourceCode = vector<string>();
	int statementNumber = 1;
	vector<int> parentVector = vector<int>();
	vector<int> currentFollowVector = vector<int>();
	vector<vector<int>> allFollowStack = vector<vector<int>>();
	vector<int> containerTracker = vector<int>();
	string currProcedure;

private:
	bool setParent(int);
	bool setModifies(int, string);
	bool setUses(int, string);
	bool setFollow(int);
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

	int getStatementIntent(string);
	int parse(string, PKB&);
	vector<string> loadFile(string);

	string leftTrim(string, string);
	string rightTrim(string, string);
	bool isValidVarName(string);
	bool isValidConstant(string);
	vector<string> tokeniseString(string, string);

	//setter/getter functions for testing
	void setPKB(PKB*);
	void setStatementNumber(int);
	int getStatementNumber();
	void setCurrentFollowVector(vector<int>);
	void setParentVector(vector<int>);
	vector<int> getParentVector();
	vector<int> getCurrentFollowVector();
	void setAllFollowStack(vector<vector<int>>);
	vector<vector<int>> getAllFollowStack();
	string getCurrentProcedure();
};