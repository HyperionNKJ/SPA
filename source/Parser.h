#pragma once

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <regex>
#include <vector>
#include <string>
#include <stack>
#include <utility>
#include "Type.h"
#include "DesignExtractor.h"
#include "PKB.h"

enum Container { WHILEC, IFC, ELSEC, NONEC, SWITCHC };
enum Statement_Key {
	KEY_PROCEDURE, KEY_ASSIGN, KEY_IF, KEY_ELSE, KEY_WHILE,
	KEY_READ, KEY_PRINT, KEY_CLOSE_BRACKET, KEY_CALL, KEY_ERROR, KEY_SWITCH, KEY_SWITCHCASE
};

class Parser {
	PKB * pkb;
	DesignExtractor de;

	std::string errorMessage = "";
	bool withinProcedure = false;
	bool emptyProcedure = true;
	bool expectElse = false;
	bool withinSwitch = false;
	bool expectStatement = false;
	bool expectDefault = false;
	bool firstCase = false;
	std::string currProcedure;
	std::vector<std::string> sourceCode = std::vector<std::string>();
	int statementNumber = 1;
	std::unordered_set<std::string> procNames = std::unordered_set<std::string>();
	
	//trackers for parent, follows
	std::vector<int> parentVector = std::vector<int>();
	std::vector<int> currentFollowVector = std::vector<int>();
	std::vector<std::vector<int>> allFollowStack = std::vector<std::vector<int>>();
	std::vector<Container> containerTracker = std::vector<Container>();
	
	//trackers for calls
	std::unordered_map<int, std::string> procCalledByTable;
	std::unordered_map<int, std::vector<int>> callParentTable;
	
	//trackers for next
	bool firstInProc = false;
	bool firstInElse = false;
	std::vector<std::pair<int, int>>  lastInIfElseTracker = std::vector<std::pair<int, int>>();
	int closedIfCount = 0;
	int lastStmtInFlow = -1;

private:
	bool checkProcedure(std::string);
	bool checkAssignment(std::string);
	bool checkExpr(std::string);
	bool checkTerm(std::string);
	bool checkFactor(std::string);
	bool checkRead(std::string);
	bool checkPrint(std::string);
	bool checkWhile(std::string);
	bool checkIf(std::string);
	bool checkElse(std::string);
	bool checkCondExpr(std::string);
	bool checkRelExpr(std::string);
	bool checkRelFactor(std::string);
	bool checkCall(std::string);
	bool checkSwitch(std::string);
	bool checkSwitchCase(std::string);

	Statement_Key getStatementIntent(std::string);

	bool setParent(int);
	bool setModifies(int, std::string, std::string);
	bool setUses(int, std::string, std::string);
	bool setCallUsesModifies();
	bool setProcIndirectUsesModifies();
	bool setFollow(int);
	bool setNext(int, Container);
	bool setCalls(std::string, std::string);
	bool setCallsT();

	std::vector<std::string> loadFile(std::string);

	std::string leftTrim(std::string, std::string);
	std::string rightTrim(std::string, std::string);
	bool isValidVarName(std::string);
	bool isValidConstant(std::string);
	std::vector<std::string> tokeniseString(std::string, std::string);

public:
	//Takes in a string representing a procedure source statement. Validates and extracts relationships and information.
	//Returns 0 on successful validation and handling, -1 otherwise.
	int handleProcedure(std::string);
	//Takes in a string representing an assignment source statement. Validates and extracts relationships and information.
	//Returns 0 on successful validation and handling, -1 otherwise.
	int handleAssignment(std::string);
	//Takes in a string representing a read source statement. Validates and extracts relationships and information.
	//Returns 0 on successful validation and handling, -1 otherwise.
	int handleRead(std::string);
	//Takes in a string representing a print source statement. Validates and extracts relationships and information.
	//Returns 0 on successful validation and handling, -1 otherwise.
	int handlePrint(std::string);
	//Takes in a string representing a while source statement. Validates and extracts relationships and information.
	//Returns 0 on successful validation and handling, -1 otherwise.
	int handleWhile(std::string);
	//Takes in a string representing an if source statement. Validates and extracts relationships and information.
	//Returns 0 on successful validation and handling, -1 otherwise.
	int handleIf(std::string);
	//Takes in a string representing an else source statement. Validates and extracts relationships and information.
	//Returns 0 on successful validation and handling, -1 otherwise.
	int handleElse(std::string);
	//Takes in a string representing a call source statement. Validates and extracts relationships and information.
	//Returns 0 on successful validation and handling, -1 otherwise.
	int handleCall(std::string);
	//Takes in a string representing a switch source statement. Validates and extracts relationships and information.
	//Returns 0 on successful validation and handling, -1 otherwise.
	int handleSwitch(std::string);
	//Takes in a string representing a case in switch source statement. Validates and extracts relationships and information.
	//Returns 0 on successful validation and handling, -1 otherwise.
	int handleSwitchCase(std::string);
	//Takes in a string representing a closed curly bracket source statement. Validates and extracts relationships and information.
	//Returns 0 on successful validation and handling, -1 otherwise.
	int handleCloseBracket(std::string);

	//Takes in a string containing the filename of the source to parse, and a reference to PKB to populate
	//Returns 0 on successful parse, -1 otherwise.
	int parse(std::string, PKB&);

	//setter/getter functions for testing
	void setPKB(PKB*);
	void setStatementNumber(int);
	void setProcedure(std::string);
	void setCurrentFollowVector(std::vector<int>);
	void setParentVector(std::vector<int>);
	void setAllFollowStack(std::vector<std::vector<int>>);
	void setWithinProcedure(bool);

	int getStatementNumber();
	std::vector<int> getParentVector();
	std::vector<int> getCurrentFollowVector();
	std::vector<std::vector<int>> getAllFollowStack();
	std::string getCurrentProcedure();
	std::unordered_set<std::string> getProcNames();
	std::unordered_map<int, std::string> getProcCalledByTable();
	std::string getErrorMessage();
};