#pragma once

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <regex>
#include <algorithm>

using namespace std;

#include "Parser.h"

static int KEY_PROCEDURE = 1;
static int KEY_ASSIGN = 2;
static int KEY_IF = 3;
static int KEY_ELSE = 4;
static int KEY_WHILE = 5;
static int KEY_READ = 6;
static int KEY_PRINT = 7;
static int KEY_CLOSE_BRACKET = 8;
static int KEY_CALL = 9;

static string varNameRegex = "([[:alpha:]]([[:alnum:]])*)";
static string constantRegex = "[[:digit:]]+";
static string spaceRegex = "[[:s:]]*";
static string openCurlyRegex = "\\{";

bool withinProcedure;
bool emptyProcedure;
vector<string> sourceCode;
int statementNumber;

Parser::Parser() {
	withinProcedure = false;
	sourceCode.clear();
	statementNumber = 1;
}

int Parser::Parse(string fileName) {
	loadFile(fileName);
	for (unsigned int i = 0; i < sourceCode.size(); i++) {
		int intent = getStatementIntent(sourceCode[i]);
		int result = 0;
		if (intent == KEY_PROCEDURE) {
			result = handleProcedure(sourceCode[i]);
		}
		else {
			if (!withinProcedure) {
				cout << "All statements should be contained within procedures" << endl;
				return false;
			}
			else {
				if (intent == KEY_ASSIGN) {
					result = handleAssignment(sourceCode[i]);
				}
				else if (intent == KEY_CLOSE_BRACKET) {
					result = handleCloseBracket(sourceCode[i]);
				}
				else if (intent == KEY_IF) {
					result = handleIf(sourceCode[i]);
				}
				else if (intent == KEY_WHILE) {
					result = handleWhile(sourceCode[i]);
				}
				else if (intent == KEY_PRINT) {
					result = handlePrint(sourceCode[i]);
				}
				else if (intent == KEY_READ) {
				result = handleRead(sourceCode[i]);
				}
				else {
					cout << "Statement of unknown type encountered" << endl;
					cout << "Line is: " << sourceCode[i] << endl;
					result = -1;
				}
			}
		}
		//early termination if parsing fails at any point
		if (result != 0) {
			break;
		}
	}
	return 0;
}

int Parser::getStatementIntent(string line) {
	//check assignment first for potential variable names being keywords
	if (line.find("=", 0) != string::npos) {
		return KEY_ASSIGN;
	}
	//otherwise tokenise and find first token as keyword for statement.
	//tokenise to split on spaces and brackets that might appear
	vector<string> tokenLine = tokeniseString(line, " \t\n({");
	tokenLine[0] = leftTrim(tokenLine[0], " \t\n");
	tokenLine[1] = rightTrim(tokenLine[0], " \t\n");
	if (tokenLine[0] == "procedure") {
		return KEY_PROCEDURE;
	}
	if (tokenLine[0] == "while") {
		return KEY_WHILE;
	}
	if (tokenLine[0] == "if") {
		return KEY_IF;
	}
	if (tokenLine[0] == "read") {
		return KEY_READ;
	}
	if (tokenLine[0] == "print") {
		return KEY_PRINT;
	}
	if (tokenLine[0] == "else") {
		return KEY_ELSE;
	}
	if (tokenLine[0] == "while") {
		return KEY_WHILE;
	}
	if (tokenLine[0] == "if") {
		return KEY_IF;
	}
	if (tokenLine[0] == "else") {
		return KEY_ELSE;
	}
	if (tokenLine[0] == "}") {
		return KEY_CLOSE_BRACKET;
	}
	return -1;
}

bool Parser::checkProcedure(string procLine) {
	//parse the procedure and check it is valid.
	string procedureRegexString = spaceRegex + "procedure" + spaceRegex + varNameRegex + spaceRegex + openCurlyRegex + spaceRegex;
	regex procedureRegex(procedureRegexString);
	if (!regex_match(procLine, procedureRegex)) {
		cout << "Procedure statement is invalid" << endl;
		return false;
	}
	return true;
}

int Parser::handleProcedure(string procLine) {
	if (!checkProcedure(procLine)) {
		return -1;
	}
	size_t startPos = procLine.find("procedure");
	size_t endPos = procLine.find_first_of("{");
	string procedureName = procLine.substr(startPos + 9, endPos - startPos - 9);
	procedureName = leftTrim(procedureName, " \t");
	procedureName = rightTrim(procedureName, " \t");
	
	withinProcedure = true;
	emptyProcedure = true;
	return 0;
}

bool Parser::checkAssignment(string assignmentLine) {
	//clear spaces within the line to make parsing easier
	//also remove the semicolon since not needed to check grammar of the assign statement
	string cleanedAssignment = "";
	for (unsigned int i = 0; i < assignmentLine.length(); i++) {
		if (assignmentLine[i] != ' ' && assignmentLine[i] != '\t' && assignmentLine[i] != '\n' && assignmentLine[i] != ';') {
			cleanedAssignment += assignmentLine[i];
		}
	}
	size_t equalPos = cleanedAssignment.find_first_of("=");
	string lhsLine = cleanedAssignment.substr(0, equalPos);
	string rhsLine = cleanedAssignment.substr(equalPos + 1, string::npos);
	if (!isValidVarName(lhsLine)) {
		cout << "Error in left hand side of assignment statement at line" << statementNumber << endl;
		return false;
	}
	if (!checkExpr(rhsLine)) {
		cout << "Error in right hand side of assignment statement at line " << statementNumber << endl;
		return false;
	}
	//extract info


	statementNumber++;
	return true;
}

bool Parser::checkExpr(string expr) {
	//check for + or - operators to split into grammar parts
	size_t firstDelimiterPos = expr.find_first_of("+-");
	//neither + nor -, try to check for a single term
	if (firstDelimiterPos == string::npos) {
		return checkTerm(expr);
	}
	//find first minus or plus and truncate, then try to accept both parts of the statement
	string firstLinePart = expr.substr(0, firstDelimiterPos);
	string secondLinePart = expr.substr(firstDelimiterPos+1, string::npos);
	if (checkExpr(firstLinePart)) {
		return checkTerm(secondLinePart);
	}
	return false;
}

bool Parser::checkTerm(string term) {
	//similar logic to checkExpr
	size_t firstDelimiterPos = term.find_first_of("*/%");
	if (firstDelimiterPos == string::npos) {
		return checkFactor(term);
	}
	string firstLinePart = term.substr(0, firstDelimiterPos);
	string secondLinePart = term.substr(firstDelimiterPos+1, string::npos);
	if (checkTerm(firstLinePart)) {
		return checkFactor(secondLinePart);
	}
	return false;
}

bool Parser::checkFactor(string factor) {
	if (isValidVarName(factor)) {
		return true;
	}
	if (isValidConstant(factor)) {
		return true;
	}
	//else try to search for matching ( ) bracket without anything else, and containing an expr
	size_t openBracketPos = factor.find_first_of("(");
	size_t closeBracketPos = factor.find_first_of(")");
	if (openBracketPos != string::npos && closeBracketPos != string::npos) {
		string lineFirstPart = factor.substr(0, openBracketPos);
		string lineLastPart = factor.substr(closeBracketPos + 1, string::npos);
		return checkExpr(factor.substr(openBracketPos, closeBracketPos - openBracketPos + 1));
	}
	return false;
}

int Parser::handleAssignment(string assignmentLine) {
	if (!checkAssignment(assignmentLine)) {
		return -1;
	}
	//do stuff to extract data from assignment statement here
	
	emptyProcedure = false;
	statementNumber++;
	return 0;
}

bool Parser::checkRead(string readLine) {
	string readRegexString = spaceRegex + "read" + spaceRegex + varNameRegex + spaceRegex;
	regex readRegex(readRegexString);
	if (!regex_match(readLine, readRegex)) {
		cout << "Read statement at is invalid at line " << statementNumber << endl;
		return false;
	}
	return true;
}

int Parser::handleRead(string readLine) {
	if (!checkRead(readLine)) {
		return -1;
	}
	//extract variable name
	size_t startPos = readLine.find_first_of("read");
	size_t endPos = readLine.find_first_of(";");
	string varName = readLine.substr(startPos + 4, endPos - startPos - 4);
	varName = leftTrim(varName, " \t");
	varName = rightTrim(varName, " \t");

	emptyProcedure = false;
	statementNumber++;
	return 0;
}

bool Parser::checkPrint(string printLine) {
	string printRegexString = spaceRegex + "print" + spaceRegex + varNameRegex + spaceRegex + spaceRegex;
	regex printRegex(printRegexString);
	if (!regex_match(printLine, printRegex)) {
		cout << "Print statement is invalid at line " << statementNumber << endl;
		return false;
	}
	return true;
}

int Parser::handlePrint(string printLine) {
	if (!checkPrint(printLine)) {
		return -1;
	}
	//extract variable name
	size_t startPos = printLine.find_first_of("read");
	size_t endPos = printLine.find_first_of(";");
	string varName = printLine.substr(startPos + 5, endPos - startPos - 5);
	varName = leftTrim(varName, " \t");
	varName = rightTrim(varName, " \t");

	emptyProcedure = false;
	statementNumber++;
}

bool Parser::checkWhile(string whileLine) {
	//find the brackets on the cond expr, check that no unexpected tokens are in the while line
	//then call functions to check validity of the cond expr
	size_t firstOpenBracket = whileLine.find_first_of("(");
	size_t lastCloseBracket = whileLine.find_last_of(")");
	string truncWhileLine = whileLine.substr(0, firstOpenBracket + 1) + whileLine.substr(lastCloseBracket, string::npos);
	string condExprLine = whileLine.substr(firstOpenBracket, lastCloseBracket - firstOpenBracket + 1);
	string whileRegexString = spaceRegex + "while" + spaceRegex + "(" + spaceRegex + ")" + spaceRegex + openCurlyRegex + spaceRegex;
	regex whileRegex(whileRegexString);
	if (!regex_match(whileLine, whileRegex)) {
		cout << "Unexpected tokens in the white statement at line " << statementNumber << endl;
		return false;
	}
	return checkCondExpr(condExprLine);
}

int Parser::handleWhile(string whileLine) {
	if (!checkWhile(whileLine)) {
		return -1;
	}
	//need to do things here
	//update parent vector.
	//place current follows vector into stack.
	//reset the follows vector
	//update bracket stack tracker
	//extract variables, constants in the cond_expr
}

bool Parser::checkIf(string ifLine) {
	//check for brackets, then do descent on the cond_expr
	//practically the same as while
}

int Parser::handleIf(string ifLine) {
	if (!checkIf(ifLine)) {
		return -1;
	}
	//about same as while. just keep track that there will be an else
	//update parent vector.
	//place current follows vector into stack.
	//reset follows vector
	//update bracket stack tracker
	//extract variables, constants in the cond_expr
}

bool Parser::checkCondExpr(string condExpr) {
	//check that brackets wrap the expression, without unexpected tokens
	if (condExpr.find_first_of("(") != 0 || condExpr.find_last_of(")") != condExpr.length - 1) {
		cout << "Found unexpected token when expecting ( and ) around conditional expression at line " << statementNumber << endl;
		return false;
	}
	//search for first !, && or || operator
	size_t andOperatorPos = condExpr.find("&&");
	size_t orOperatorPos = condExpr.find("||");
	size_t notOperatorPos = condExpr.find("!");
	if (notOperatorPos < andOperatorPos && notOperatorPos < orOperatorPos) {
		size_t openBracketPos = condExpr.find_first_of("(");
		size_t closeBracketPos = condExpr.find_last_of(")");
		string internalCondExpr = condExpr.substr(openBracketPos, closeBracketPos - openBracketPos - 1);
		return checkCondExpr(internalCondExpr);
	}
	size_t firstDelimiterPos = std::min(andOperatorPos, orOperatorPos);
	if (firstDelimiterPos != string::npos) {
		string firstCondExpr = condExpr.substr(0, firstDelimiterPos);
		string secondCondExpr = condExpr.substr(firstDelimiterPos + 2, string::npos);
		return checkCondExpr(firstCondExpr) & checkCondExpr(secondCondExpr);
	}
	//else no delimiter, assume it is a rel expr and strip brackets
	string relExpr = condExpr.substr(1, condExpr.length - 2);
	relExpr = leftTrim(relExpr, " \r");
	relExpr = rightTrim(relExpr, " \r");
	return checkRelExpr(condExpr);

}

bool Parser::checkRelExpr(string relExpr) {

}

bool Parser::checkRelFactor(string relFactor) {
	relFactor = leftTrim(relFactor, " \t");
	relFactor = rightTrim(relFactor, " \t\n");
	if (isValidVarName(relFactor)) {
		return true;
	}
	if (isValidConstant(relFactor)) {
		return true;
	}
	return checkExpr(relFactor);
}

int Parser::handleCloseBracket(string closeBracket) {
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

vector<string> Parser::loadFile(string fileName) {
	ifstream sourceFile;
	sourceFile.open(fileName);
	string currLine;
	string allSourceCode = "";
	while (getline(sourceFile, currLine)) {
		allSourceCode += currLine;
	}
	allSourceCode = rightTrim(allSourceCode, " \t\n");
	while (!allSourceCode.empty()) {
		allSourceCode = leftTrim(allSourceCode, " \t\n");
		/*Break up into lines in 3 cases
		First is semicolon, second is open brackets, third is close brackets */
		size_t delimitPos = allSourceCode.find_first_of(";{}");
		while (delimitPos != string::npos) {
			string currStatement = allSourceCode.substr(0, delimitPos+1);
			sourceCode.push_back(currStatement);
			allSourceCode = allSourceCode.substr(delimitPos + 1, string::npos);
			delimitPos = allSourceCode.find_first_of(";{}");
		}
		if (!allSourceCode.empty()) {
			sourceCode.push_back(allSourceCode);
		}
	}
	return sourceCode;
}

bool Parser::isValidVarName(string line) {
	string varNameRegexString = spaceRegex + varNameRegex + spaceRegex;
	regex varRegex(varNameRegexString);
	if (!regex_match(line, varRegex)) {
		return false;
	}
	return true;
}

bool Parser::isValidConstant(string line) {
	string constantRegexString = spaceRegex + constantRegex + spaceRegex;
	regex constRegex(constantRegexString);
	if (!regex_match(line, constRegex)) {
		return false;
	}
	return true;
}

string Parser::leftTrim(string line, string targetChar) {
	size_t startpos = line.find_first_not_of(targetChar);
	if (string::npos != startpos) {
		line = line.substr(startpos);
	}
	return line;
}

string Parser::rightTrim(string line, string targetChar) {
	size_t endpos = line.find_last_not_of(targetChar);
	if (string::npos != endpos) {
		line = line.substr(0, endpos + 1);
	}
	return line;
}

vector<string> Parser::tokeniseString(string toTokenise, string delimiters) {
	vector<string> tokenList = vector<string>();
	string currToken;
	while (!toTokenise.empty()) {
		size_t delimiterPos = toTokenise.find_first_of(delimiters);
		tokenList.push_back(toTokenise.substr(0, delimiterPos));
	}
	return tokenList;
}