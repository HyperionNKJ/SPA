#pragma once

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <regex>
#include <vector>
#include <string>

class Parser {
public:
	Parser();

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
	int handleCloseBracket(string);
	bool checkCondExpr(string);
	bool checkRelExpr(string);
	bool checkRelFactor(string);

	int getStatementIntent(string);
	int Parse(string);
	vector<string> loadFile(string);

	string leftTrim(string, string);
	string rightTrim(string, string);
	bool isValidVarName(string);
	bool isValidConstant(string);
	vector<string> tokeniseString(string, string);
};