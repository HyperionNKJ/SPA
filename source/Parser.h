#pragma once

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <regex>

using namespace std;

bool checkProcedure(string);
int handleProcedure(string);
bool checkAssignment(string);
bool checkExpr(string);
bool checkTerm(string);
bool checkFactor(string);
int handleAssignment(string);

int getStatementIntent(string);
int Parse(string);
vector<string> loadFile(string);

string leftTrim(string, string);
string rightTrim(string, string);
bool isValidVarName(string);
bool isValidConstant(string);