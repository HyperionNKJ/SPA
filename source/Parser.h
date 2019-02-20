#pragma once

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <regex>
#include <vector>
#include <string>
#include <stack>

using namespace std;

//Static integers to pass information on keyword in the statement
static int KEY_PROCEDURE = 1;
static int KEY_ASSIGN = 2;
static int KEY_IF = 3;
static int KEY_ELSE = 4;
static int KEY_WHILE = 5;
static int KEY_READ = 6;
static int KEY_PRINT = 7;
static int KEY_CLOSE_BRACKET = 8;
static int KEY_CALL = 9;
//Static integers to keep track of the container statements in the stack
static int WHILECONTAINER = 100;
static int IFCONTAINER = 101;
static int ELSECONTAINER = 102;

class Parser {
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
	bool checkCondExpr(string);
	bool checkRelExpr(string);
	bool checkRelFactor(string);

	int handleCloseBracket(string);

	int getStatementIntent(string);
	int parse(string);
	vector<string> loadFile(string);

	string leftTrim(string, string);
	string rightTrim(string, string);
	bool isValidVarName(string);
	bool isValidConstant(string);
	vector<string> tokeniseString(string, string);
};