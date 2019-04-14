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

enum Container { WHILEC, IFC, ELSEC, NONEC, SWITCHC };
enum Statement_Key {
	KEY_PROCEDURE, KEY_ASSIGN, KEY_IF, KEY_ELSE, KEY_WHILE,
	KEY_READ, KEY_PRINT, KEY_CLOSE_BRACKET, KEY_CALL, KEY_ERROR, KEY_SWITCH, KEY_SWITCHCASE
};

class Parser {
	PKB * pkb;
	DesignExtractor de;

	string errorMessage = "";
	bool withinProcedure = false;
	bool emptyProcedure = true;
	bool expectElse = false;
	bool withinSwitch = false;
	bool expectStatement = false;
	bool firstCase = false;
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
	unordered_map<int, vector<int>> callParentTable;
	
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
	bool checkSwitch(string);
	bool checkSwitchCase(string);

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
	//Takes in a string representing a procedure source statement. Validates and extracts relationships and information.
	//Returns 0 on successful validation and handling, -1 otherwise.
	int handleProcedure(string);
	//Takes in a string representing an assignment source statement. Validates and extracts relationships and information.
	//Returns 0 on successful validation and handling, -1 otherwise.
	int handleAssignment(string);
	//Takes in a string representing a read source statement. Validates and extracts relationships and information.
	//Returns 0 on successful validation and handling, -1 otherwise.
	int handleRead(string);
	//Takes in a string representing a print source statement. Validates and extracts relationships and information.
	//Returns 0 on successful validation and handling, -1 otherwise.
	int handlePrint(string);
	//Takes in a string representing a while source statement. Validates and extracts relationships and information.
	//Returns 0 on successful validation and handling, -1 otherwise.
	int handleWhile(string);
	//Takes in a string representing an if source statement. Validates and extracts relationships and information.
	//Returns 0 on successful validation and handling, -1 otherwise.
	int handleIf(string);
	//Takes in a string representing an else source statement. Validates and extracts relationships and information.
	//Returns 0 on successful validation and handling, -1 otherwise.
	int handleElse(string);
	//Takes in a string representing a call source statement. Validates and extracts relationships and information.
	//Returns 0 on successful validation and handling, -1 otherwise.
	int handleCall(string);
	//Takes in a string representing a switch source statement. Validates and extracts relationships and information.
	//Returns 0 on successful validation and handling, -1 otherwise.
	int handleSwitch(string);
	//Takes in a string representing a case in switch source statement. Validates and extracts relationships and information.
	//Returns 0 on successful validation and handling, -1 otherwise.
	int handleSwitchCase(string);
	//Takes in a string representing a closed curly bracket source statement. Validates and extracts relationships and information.
	//Returns 0 on successful validation and handling, -1 otherwise.
	int handleCloseBracket(string);

	//Takes in a string containing the filename of the source to parse, and a reference to PKB to populate
	//Returns 0 on successful parse, -1 otherwise.
	int parse(string, PKB&);

	//setter/getter functions for testing
	void setPKB(PKB*);
	void setStatementNumber(int);
	void setCurrentFollowVector(vector<int>);
	void setParentVector(vector<int>);
	void setAllFollowStack(vector<vector<int>>);
	void setWithinProcedure(bool);

	int getStatementNumber();
	vector<int> getParentVector();
	vector<int> getCurrentFollowVector();
	vector<vector<int>> getAllFollowStack();
	string getCurrentProcedure();
	unordered_set<string> getProcNames();
	unordered_map<int, string> getProcCalledByTable();
	string getErrorMessage();
};