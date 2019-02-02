#pragma once

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <regex>

using namespace std;

#include "Parser.h"

static int KEY_PROCEDURE = 1;
static int KEY_ASSIGN = 2;
static int KEY_IF = 3;
static int KEY_ELSE = 4;
static int KEY_WHILE = 5;
static int KEY_CLOSE_BRACKET = 6;
static int KEY_CALL = 7;

bool withinProcedure;
vector<string> sourceCode;
int statementNumber = 1;

static string varNameRegex = "([[:alpha:]]([[:alnum:]])*)";
static string spaceRegex = "[[:s:]]*";
static string openCurlyRegex = "\\{";

/*Parser::Parser() {

}

Parser::Parser(PKB* p) {
	pkb = p;
	statementNumber = 1;
	withinProcedure = withinWhile = false;
	sourceCode = vector<string>();
}*/

bool checkProcedure(string line) {
	//parse the procedure and check it is valid.
	cout << "in check procedure function for line " << endl << line << endl;
	if (withinProcedure) {
		cout << "A procedure cannot be nested inside another procedure" << endl;
		return false;
	}
	string procedureRegexString = spaceRegex + "procedure" + spaceRegex + varNameRegex + spaceRegex + openCurlyRegex + spaceRegex;
	regex procedureRegex(procedureRegexString);
	if (!regex_match(line, procedureRegex)) {
		cout << "Procedure statement is invalid" << endl;
		return false;
	}
	return true;
}

int handleProcedure(string line) {
	if (!checkProcedure(line)) {
		return -1;
	}
	size_t endPos = line.find_first_of("{");
	size_t startPos = line.find("procedure");
	string procedureName = line.substr(startPos + 9, endPos - startPos - 9);
	procedureName = leftTrim(procedureName, " \t");
	procedureName = rightTrim(procedureName, " \t");
	withinProcedure = true;
	cout << procedureName << endl;
	return 0;
}

int handleIf(string line) {
	return 0;
}

int handleWhile(string line) {
	return 0;
}

bool checkAssignment(string line) {
	return true;
}

int handleAssignment(string line) {
	if (!checkAssignment(line)) {
		return -1;
	}
	return 0;
}

int getStatementIntent(string line) {
	//check for types in order of precedence to target edge cases
	//check assignment first for potential variable names being keywords
	if (line.find("=", 0) != string::npos) {
		return KEY_ASSIGN;
	}
	if (line.find("procedure") != string::npos) {
		return KEY_PROCEDURE;
	}
	if (line.find("|") != string::npos) {
		return KEY_CLOSE_BRACKET;
	}
	return -1;
}

int Parse(string fileName) {
	loadFile(fileName);
	for (unsigned int i = 0; i < sourceCode.size(); i++) {
		//cout << sourceCode[i] << endl;
		int intent = getStatementIntent(sourceCode[i]);
		int result = 0;
		if (intent == KEY_PROCEDURE) {
			result = handleProcedure(sourceCode[i]);
		}
		else if (intent == KEY_ASSIGN) {
			result = handleAssignment(sourceCode[i]);
		}
		else {
			cout << "Statement of unknown type encountered after line " << statementNumber << endl;
			break;
		}

		//early termination if parsing fails at any point
		if (result != 0) {
			break;
		}
	}
	return 0;
}

vector<string> loadFile(string fileName) {
	ifstream sourceFile;
	sourceFile.open(fileName);
	string currLine;
	while (getline(sourceFile, currLine)) {
		currLine = leftTrim(currLine, " \t");
		currLine = rightTrim(currLine, " \t\n");
		sourceCode.push_back(currLine);
	}
	return sourceCode;
}

/*
Params: String, String
Returns: String
This function erases leading characters in targetChar from the string line, and then returns line.
*/
string leftTrim(string line, string targetChar) {
	size_t startpos = line.find_first_not_of(targetChar);
	if (string::npos != startpos) {
		line = line.substr(startpos);
	}
	return line;
}

/*
Params: String, String
Returns: String
This function erases trailing characters in targetChar from the string line, and then returns line.
*/
string rightTrim(string line, string targetChar) {
	size_t endpos = line.find_last_not_of(targetChar);
	if (string::npos != endpos) {
		line = line.substr(0, endpos + 1);
	}
	return line;
}