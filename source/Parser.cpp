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
bool emptyProcedure;
vector<string> sourceCode;
int statementNumber = 1;

static string varNameRegex = "([[:alpha:]]([[:alnum:]])*)";
static string constantRegex = "[[:digit:]]+";
static string spaceRegex = "[[:s:]]*";
static string openCurlyRegex = "\\{";

bool checkProcedure(string line) {
	//parse the procedure and check it is valid.
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
	emptyProcedure = true;
	return 0;
}

int handleCloseBracket(string line) {
	//currently only procedure close bracket without if or while
	//so handle as that case
	if (emptyProcedure) {
		cout << "A procedure cannot be empty" << endl;
		return -1;
	}
	withinProcedure = false;
	emptyProcedure = true;
	return 0;
}

bool checkAssignment(string line) {
	//clear spaces within the line to make parsing easier
	string cleanedLine = "";
	for (unsigned int i = 0; i < line.length(); i++) {
		if (line[i] != ' ' && line[i] != '\t' && line[i] != '\n') {
			cleanedLine += line[i];
		}
	}
	size_t equalPos = cleanedLine.find_first_of("=");
	string lhsLine = cleanedLine.substr(0, equalPos);
	string rhsLine = cleanedLine.substr(equalPos + 1, string::npos);
	if (!isValidVarName(lhsLine)) {
		cout << "Error in left hand side of assignment statement" << endl;
		return false;
	}
	if (!checkExpr(rhsLine)) {
		cout << "Error in right hand side of assignment statement" << endl;
		return false;
	}
	return true;
}

bool checkExpr(string line) {
	//check for + or - operators to split into grammar parts
	size_t firstPlusPos = line.find_first_of("+");
	size_t firstMinusPos = line.find_first_of("-");
	//neither + nor -, try to check for a single term
	if (firstPlusPos == string::npos && firstMinusPos == string::npos) {
		return checkTerm(line);
	}
	size_t firstCutoff = firstPlusPos;
	if (firstMinusPos < firstCutoff) {
		firstCutoff = firstMinusPos;
	}
	//find first minus or plus and truncate, then try to accept both parts of the statement
	string firstLinePart = line.substr(0, firstCutoff);
	string secondLinePart = line.substr(firstCutoff+1, string::npos);
	if (checkExpr(firstLinePart)) {
		return checkTerm(secondLinePart);
	}
	else {
		return false;
	}
	cout << "Unknown error encountered in parsing Expr" << endl;
	return false;
}

bool checkTerm(string line) {
	//similar logic to checkExpr
	size_t firstTimesPos = line.find_first_of("*");
	size_t firstDividePos = line.find_first_of("/");
	size_t firstModulusPos = line.find_first_of("%");
	if (firstTimesPos == string::npos && firstDividePos == string::npos && firstModulusPos == string::npos) {
		return checkFactor(line);
	}
	size_t firstCutoff = firstTimesPos;
	if (firstDividePos < firstCutoff) {
		firstCutoff = firstDividePos;
	}
	if (firstModulusPos < firstCutoff) {
		firstCutoff = firstModulusPos;
	}
	string firstLinePart = line.substr(0, firstCutoff);
	string secondLinePart = line.substr(firstCutoff+1, string::npos);
	if (checkTerm(firstLinePart)) {
		return checkFactor(secondLinePart);
	}
	else {
		return false;
	}
	cout << "Unknown error encountered in parsing Term" << endl;
	return false;
}

bool checkFactor(string line) {
	if (isValidVarName(line)) {
		return true;
	}
	if (isValidConstant(line)) {
		return true;
	}
	//else try to search for matching ( ) bracket without anything else, and containing an expr
	size_t openBracketPos = line.find_first_of("(");
	size_t closeBracketPos = line.find_first_of(")");
	string lineFirstPart = line.substr(0, openBracketPos);
	string lineLastPart = line.substr(closeBracketPos+1, string::npos);
	return checkExpr(line.substr(openBracketPos, closeBracketPos-openBracketPos+1));
}

int handleAssignment(string line) {
	if (!checkAssignment(line)) {
		return -1;
	}
	//do stuff to extract data from assignment statement here
	emptyProcedure = false;
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
	if (line.find("}") != string::npos) {
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
		else if (intent == KEY_CLOSE_BRACKET) {
			result = handleCloseBracket(sourceCode[i]);
		}
		else {
			cout << "Statement of unknown type encountered" << endl;
			cout << "Line is: " << sourceCode[i] << endl;
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
		//this loop is to break up multiple statements with semicolons in a single line
		size_t semicolonPos = currLine.find_first_of(";");
		while (semicolonPos != string::npos) {
			string currStatement = currLine.substr(0, semicolonPos);
			sourceCode.push_back(currStatement);
			currLine = currLine.substr(semicolonPos + 1, string::npos);
			semicolonPos = currLine.find_first_of(";");
		}
		if (!currLine.empty()) {
			sourceCode.push_back(currLine);
		}
	}
	return sourceCode;
}

bool isValidVarName(string line) {
	string varNameRegexString = spaceRegex + varNameRegex + spaceRegex;
	regex varRegex(varNameRegexString);
	if (!regex_match(line, varRegex)) {
		return false;
	}
	return true;
}

bool isValidConstant(string line) {
	string constantRegexString = spaceRegex + constantRegex + spaceRegex;
	regex constRegex(constantRegexString);
	if (!regex_match(line, constRegex)) {
		return false;
	}
	return true;
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