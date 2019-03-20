#pragma once

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <regex>
#include <algorithm>

#include "Parser.h"
#include "pkb.h"
#include "Type.h"

static string varNameRegex = "([[:alpha:]]([[:alnum:]])*)";
static string constantRegex = "[[:digit:]]+";
static string spaceRegex = "[[:s:]]*";
static string openCurlyRegex = "\\{";

int Parser::parse(string fileName, PKB& p) {
	pkb = &p;
	try {
		loadFile(fileName);
	}
	catch (int err) {
		cout << "Failed to open file" << endl;
		return -1;
	}
	for (unsigned int i = 0; i < sourceCode.size(); i++) {
		Statement_Key intent = getStatementIntent(sourceCode[i]);
		int result = 0;
		if (intent == KEY_PROCEDURE) {
			result = handleProcedure(sourceCode[i]);
		}
		else {
			if (!withinProcedure) {
				cout << "All statements should be contained within procedures. Error at line " << statementNumber << endl;
				return false;
			}
			else {
				if (expectElse && intent != KEY_ELSE) {
					cout << "Expected an else statement at line " << statementNumber << endl;
					result = -1;
				}
				else {
					if (intent == KEY_ASSIGN) {
						result = handleAssignment(sourceCode[i]);
						emptyProcedure = false;
						statementNumber++;
					}
					else if (intent == KEY_CLOSE_BRACKET) {
						result = handleCloseBracket(sourceCode[i]);
					}
					else if (intent == KEY_IF) {
						result = handleIf(sourceCode[i]);
						emptyProcedure = false;
						statementNumber++;
					}
					else if (intent == KEY_WHILE) {
						result = handleWhile(sourceCode[i]);
						emptyProcedure = false;
						statementNumber++;
					}
					else if (intent == KEY_PRINT) {
						result = handlePrint(sourceCode[i]);
						emptyProcedure = false;
						statementNumber++;
					}
					else if (intent == KEY_READ) {
						result = handleRead(sourceCode[i]);
						emptyProcedure = false;
						statementNumber++;
					}
					else if (intent == KEY_ELSE && expectElse == true) {
						result = handleElse(sourceCode[i]);
					}
					else if (intent == KEY_ELSE && expectElse == false) {
						cout << "Else statement without accompanying if found just before line " << statementNumber << endl;
					}
					else if (intent == KEY_CALL) {
						result = handleCall(sourceCode[i]);
						statementNumber++;
						emptyProcedure = false;
					}
					else {
						cout << "Statement of unknown type encountered at line " << statementNumber << endl;
						result = -1;
					}
				}
			}
		}
		//early termination if parsing fails at any point
		if (result != 0) {
			return -1;
		}
	}
	de.processCalls();
	setCallsT();
	setCallUsesModifies();
	setProcIndirectUsesModifies();
	return 0;
}

Statement_Key Parser::getStatementIntent(string line) {
	//check assignment first for potential variable names being keywords
	if (line.find("=", 0) != string::npos && line.find("<=") == string::npos && line.find("==") == string::npos
		&& line.find(">=") == string::npos && line.find("!=") == string::npos) {
		return KEY_ASSIGN;
	}
	//otherwise tokenise and find first token as keyword for statement.
	//tokenise to split on spaces and brackets that might appear
	vector<string> tokenLine = tokeniseString(line, " \t\n({");
	tokenLine[0] = leftTrim(tokenLine[0], " \t\n");
	tokenLine[0] = rightTrim(tokenLine[0], " \t\n");
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
	if (tokenLine[0] == "call") {
		return KEY_CALL;
	}
	if (tokenLine[0] == "}") {
		return KEY_CLOSE_BRACKET;
	}
	return KEY_ERROR;
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

	bool result;
	result = pkb->insertProc(procedureName);
	de.insertProc(procedureName);
	
	currProcedure = procedureName;
	withinProcedure = true;
	emptyProcedure = true;
	firstInProc = true;
	return 0;
}

bool Parser::checkAssignment(string assignmentLine) {
	size_t equalPos = assignmentLine.find_first_of("=");
	string lhsLine = assignmentLine.substr(0, equalPos);
	string rhsLine = assignmentLine.substr(equalPos + 1, string::npos);
	if (!isValidVarName(lhsLine)) {
		cout << "Error in left hand side of assignment statement at line" << statementNumber << endl;
		return false;
	}
	if (!checkExpr(rhsLine)) {
		cout << "Error in right hand side of assignment statement at line " << statementNumber << endl;
		return false;
	}
	return true;
}

bool Parser::checkExpr(string expr) {
	//start from the back, start scanning for + or -. Track brackets and ignore + or - inside them.
	int bracketTracker = 0;
	for (int currPos = expr.length() - 1; currPos >= 0; currPos--) {
		if (expr[currPos] == '(') {
			bracketTracker--;
		}
		else if (expr[currPos] == ')') {
			bracketTracker++;
		}
		if (bracketTracker == 0) {
			if (expr[currPos] == '+' || expr[currPos] == '-') {
				string firstExpr = expr.substr(0, currPos);
				string secondTerm = expr.substr(currPos + 1, string::npos);
				return checkExpr(firstExpr) & checkTerm(secondTerm);
			}
		}
		else if (bracketTracker < 0) {
			cout << "Extra mismatched ( bracket encountered in assignment statement at line " << statementNumber << endl;
			return false;
		}
	}
	if (bracketTracker > 0) {
		cout << "Extra mismatched ) bracket encountered in assignment statement at line " << statementNumber << endl;
		return false;
	}
	//reach the end with no + or -, check for single term
	return checkTerm(expr);
}

bool Parser::checkTerm(string term) {
	//similar logic to checkExpr
	//start from the back, start scanning for *, /, %. Track brackets and ignore delimiters inside them.
	int bracketTracker = 0;
	for (int currPos = term.length() - 1; currPos >= 0; currPos--) {
		if (term[currPos] == '(') {
			bracketTracker--;
		}
		else if (term[currPos] == ')') {
			bracketTracker++;
		}
		if (bracketTracker == 0) {
			if (term[currPos] == '*' || term[currPos] == '/' || term[currPos] == '%') {
				string firstTerm = term.substr(0, currPos);
				string secondFactor = term.substr(currPos + 1, string::npos);
				return checkTerm(firstTerm) & checkFactor(secondFactor);
			}
		}
		else if (bracketTracker < 0) {
			cout << "Extra mismatched ( bracket encountered in assignment statement at line " << statementNumber << endl;
			return false;
		}
	}
	//reach the end with no * / %, check for single factor
	return checkFactor(term);
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
	size_t closeBracketPos = factor.find_last_of(")");
	if (openBracketPos == 0 && closeBracketPos == factor.length()-1) {
		return checkExpr(factor.substr(openBracketPos+1, closeBracketPos - openBracketPos - 1));
	}
	cout << "Failed in parsing a Factor in statement at line " << statementNumber << endl;
	return false;
}

int Parser::handleAssignment(string assignmentLine) {
	//clear spaces within the line to make parsing easier
	//also remove the semicolon since not needed to check grammar of the assign statement
	string cleanedAssignment = "";
	for (unsigned int i = 0; i < assignmentLine.length(); i++) {
		if (assignmentLine[i] != ' ' && assignmentLine[i] != '\t' && assignmentLine[i] != '\n' && assignmentLine[i] != ';') {
			cleanedAssignment += assignmentLine[i];
		}
	}
	if (!checkAssignment(cleanedAssignment)) {
		return -1;
	}
	
	setParent(statementNumber);
	setFollow(statementNumber);
	setNext(statementNumber, NONEC);
	//Separate variable names, constants and operation/brackets from each other
	vector<string> assignTokens = vector<string>();
	string lhsVar = cleanedAssignment.substr(0, cleanedAssignment.find_first_of("="));
	string rhs = cleanedAssignment.substr(cleanedAssignment.find_first_of("=") + 1, string::npos);
	string currToken = "";
	for (unsigned int i = 0; i < rhs.length(); i++) {
		//assignment should have only alphanum and bracket/op. Less than 48 in ascii must be a bracket/op
		if (rhs[i] < 48) {
			if (!currToken.empty()) {
				assignTokens.push_back(currToken);
				currToken = "";
			}
			assignTokens.push_back(rhs.substr(i, 1));
		}
		else {
			currToken += rhs[i];
		}
	}
	if (!currToken.empty()) {
		assignTokens.push_back(currToken);
	}
	//form postfix expression using Dijkstra Shunting Yard
	vector<string> postfixRHS = vector<string>();
	vector<string> opStack = vector<string>();
	for (unsigned int i = 0; i < assignTokens.size(); i++) {
		if (isValidConstant(assignTokens[i]) || isValidVarName(assignTokens[i])) {
			postfixRHS.push_back(assignTokens[i]);
		}
		else if (assignTokens[i] == "(") {
			opStack.push_back(assignTokens[i]);
		}
		else if (assignTokens[i] == ")") {
			while (opStack.back() != "(") {
				postfixRHS.push_back(opStack.back());
				opStack.pop_back();
			}
			opStack.pop_back();
		}
		else if (assignTokens[i] == "+" || assignTokens[i] == "-") {
			while (opStack.size() > 0 && (opStack.back() == "+" || opStack.back() == "-")) {
				postfixRHS.push_back(opStack.back());
				opStack.pop_back();
			}
			opStack.push_back(assignTokens[i]);
		}
		else if (assignTokens[i] == "*" || assignTokens[i] == "/" || assignTokens[i] == "%") {
			while (opStack.size() > 0 && (opStack.back() == "*" || opStack.back() == "/" || opStack.back() == "%")) {
				postfixRHS.push_back(opStack.back());
				opStack.pop_back();
			}
			opStack.push_back(assignTokens[i]);
		}
	}
	while (opStack.size() > 0) {
		postfixRHS.push_back(opStack.back());
		opStack.pop_back();
	}
	//extract all possible substrings from the postfix notation
	//start at each possible location and attempt to build a string, terminating if a string cannot be a valid pattern
	vector<string> rhsSubstring = vector<string>();
	string currentSubstr;
	int tokenCount, opCount;
	for (unsigned int i = 0; i < postfixRHS.size() - 1; i++) {
		currentSubstr = "";
		tokenCount = opCount = 0;
		for (unsigned int j = i; j < postfixRHS.size(); j++) {
			currentSubstr += postfixRHS[j] + " ";
			if (isValidVarName(postfixRHS[j]) || isValidConstant(postfixRHS[j])) {
				tokenCount++;
			}
			else {
				opCount++;
			}
			if ((currentSubstr.length() > 2 && tokenCount - 1 == opCount) || (currentSubstr.length() == 1 && tokenCount == 1)) {
				rhsSubstring.push_back(rightTrim(currentSubstr, " "));
			}
			else if (opCount >= tokenCount) {
				break;
			}
		}
	}
	// DEBUG
	for (unsigned int i = 0; i < rhsSubstring.size(); i++) {
		cout << rhsSubstring[i] << endl;
	}
	//set lhs var
	pkb->insertVar(lhsVar);
	setModifies(statementNumber, currProcedure, lhsVar);
	//set rhs var, constants
	for (unsigned int i = 0; i < assignTokens.size(); i++) {
		if (isValidVarName(assignTokens[i])) {
			pkb->insertVar(assignTokens[i]);
			setUses(statementNumber, currProcedure, assignTokens[i]);
		}
		else if (isValidConstant(assignTokens[i])) {
			pkb->insertConstant(stoi(assignTokens[i]));
		}
	}
	pkb->insertAssignStmt(statementNumber, lhsVar, assignTokens);
	pkb->insertStmtType(statementNumber, ASSIGN);
	currentFollowVector.push_back(statementNumber);
	return 0;
}

bool Parser::checkRead(string readLine) {
	string readRegexString = spaceRegex + "read" + spaceRegex + varNameRegex + spaceRegex + ";" + spaceRegex;
	regex readRegex(readRegexString);
	if (!regex_match(readLine, readRegex)) {
		cout << "Read statement is invalid at line " << statementNumber << endl;
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

	setParent(statementNumber);
	setFollow(statementNumber);
	setNext(statementNumber, NONEC);
	setModifies(statementNumber, currProcedure, varName);
	pkb->insertVar(varName);
	pkb->insertCPRStmtType(statementNumber, READ, varName);

	currentFollowVector.push_back(statementNumber);
	return 0;
}

bool Parser::checkPrint(string printLine) {
	string printRegexString = spaceRegex + "print" + spaceRegex + varNameRegex + spaceRegex + ";" + spaceRegex;
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

	setParent(statementNumber);
	setFollow(statementNumber);
	setNext(statementNumber, NONEC);
	setUses(statementNumber, currProcedure, varName);
	pkb->insertVar(varName);
	pkb->insertCPRStmtType(statementNumber, PRINT, varName);

	currentFollowVector.push_back(statementNumber);
	return 0;
}

bool Parser::checkWhile(string whileLine) {
	//find the brackets on the cond expr, check that no unexpected tokens are in the while line
	//then call functions to check validity of the cond expr
	size_t firstOpenBracket = whileLine.find_first_of("(");
	size_t lastCloseBracket = whileLine.find_last_of(")");
	if (firstOpenBracket == string::npos || lastCloseBracket == string::npos) {
		cout << "Missing ( and ) brackets around the conditional expession at line " << statementNumber << endl;
		return false;
	}
	string truncWhileLine = whileLine.substr(0, firstOpenBracket) + whileLine.substr(lastCloseBracket+1, string::npos);
	string condExprLine = whileLine.substr(firstOpenBracket, lastCloseBracket - firstOpenBracket + 1);
	string whileRegexString = spaceRegex + "while" + spaceRegex + openCurlyRegex + spaceRegex;
	regex whileRegex(whileRegexString);
	if (!regex_match(truncWhileLine, whileRegex)) {
		cout << "Unexpected tokens in the while statement at line " << statementNumber << endl;
		return false;
	}
	return checkCondExpr(condExprLine);
}

int Parser::handleWhile(string whileLine) {
	if (!checkWhile(whileLine)) {
		return -1;
	}

	//set relationships
	setParent(statementNumber);
	setFollow(statementNumber);
	setNext(statementNumber, NONEC);

	//update parent, container, follows trackers
	currentFollowVector.push_back(statementNumber);
	parentVector.push_back(statementNumber);
	containerTracker.push_back(WHILEC);
	allFollowStack.push_back(currentFollowVector);
	currentFollowVector.clear();
	pkb->insertStmtType(statementNumber, WHILE);

	//set uses relationships
	size_t openBracketPos = whileLine.find_first_of("(");
	size_t closeBracketPos = whileLine.find_last_of(")");
	string condExpr = whileLine.substr(openBracketPos + 1, closeBracketPos - openBracketPos - 1);
	vector<string> tokens = tokeniseString(condExpr, " \t&|()!");
	for (unsigned int i = 0; i < tokens.size(); i++) {
		if (isValidVarName(tokens[i])) {
			pkb->insertVar(tokens[i]);
			setUses(statementNumber, currProcedure, tokens[i]);
		}
		else if (isValidConstant(tokens[i])) {
			pkb->insertConstant(stoi(tokens[i]));
		}
	}
	return 0;
}

bool Parser::checkIf(string ifLine) {
	//check for brackets, then check the cond_expr
	//practically the same as while
	size_t firstOpenBracket = ifLine.find_first_of("(");
	size_t lastCloseBracket = ifLine.find_last_of(")");
	if (firstOpenBracket == string::npos || lastCloseBracket == string::npos) {
		cout << "Missing ( and ) brackets around the conditional expression at line " << statementNumber << endl;
		return false;
	}
	string truncIfLine = ifLine.substr(0, firstOpenBracket) + ifLine.substr(lastCloseBracket+1, string::npos);
	string condExprLine = ifLine.substr(firstOpenBracket, lastCloseBracket - firstOpenBracket + 1);
	string ifRegexString = spaceRegex + "if" + spaceRegex + "then" + spaceRegex + openCurlyRegex + spaceRegex;
	regex ifRegex(ifRegexString);
	if (!regex_match(truncIfLine, ifRegex)) {
		cout << "Unexpected tokens in the if statement at line " << statementNumber << endl;
		return false;
	}
	return checkCondExpr(condExprLine);
	return false;
}

int Parser::handleIf(string ifLine) {
	if (!checkIf(ifLine)) {
		return -1;
	}
	//set follow, parent relationships
	setParent(statementNumber);
	setFollow(statementNumber);
	setNext(statementNumber, NONEC);

	//update parent, follow, container trackers
	currentFollowVector.push_back(statementNumber);
	parentVector.push_back(statementNumber);
	containerTracker.push_back(IFC);
	allFollowStack.push_back(currentFollowVector);
	currentFollowVector.clear();
	pkb->insertStmtType(statementNumber, IF);

	//set uses relationships
	size_t openBracketPos = ifLine.find_first_of("(");
	size_t closeBracketPos = ifLine.find_last_of(")");
	string condExpr = ifLine.substr(openBracketPos + 1, closeBracketPos - openBracketPos - 1);
	vector<string> tokens = tokeniseString(condExpr, " \t&|()!");
	for (unsigned int i = 0; i < tokens.size(); i++) {
		if (isValidVarName(tokens[i])) {
			pkb->insertVar(tokens[i]);
			setUses(statementNumber, currProcedure, tokens[i]);
		}
		else if (isValidConstant(tokens[i])) {
			pkb->insertConstant(stoi(tokens[i]));
		}
	}
	return 0;
}

bool Parser::checkElse(string elseLine) {
	string elseRegexString = spaceRegex + "else" + spaceRegex + openCurlyRegex;
	regex elseRegex(elseRegexString);
	if (!regex_match(elseLine, elseRegex)) {
		cout << "Else statement has unexpected tokens just before line " << statementNumber << endl;
		return false;
	}
	return true;
}

int Parser::handleElse(string elseLine) {
	if (!checkElse(elseLine)) {
		return -1;
	}
	//reset booleans
	//reset follow tracker
	//set container tracker
	expectElse = false;
	firstInElse = true;
	containerTracker.push_back(ELSEC);
	currentFollowVector.clear();
	return 0;
}

bool Parser::checkCondExpr(string condExpr) {
	//clean spaces or tabs to make parsing easier
	string cleanedCondExpr = "";
	for (unsigned int i = 0; i < condExpr.length(); i++) {
		if (condExpr[i] != ' ' && condExpr[i] != '\t') {
			cleanedCondExpr += condExpr[i];
		}
	}
	condExpr = cleanedCondExpr;
	//check that brackets wrap the expression, without unexpected tokens
	if (condExpr.find_first_of("(") != 0 || condExpr.find_last_of(")") != (condExpr.length() - 1)) {
		cout << "Found unexpected token when expecting ( and ) around conditional expression at line " << statementNumber << endl;
		return false;
	}
	//remove external brackets
	condExpr = condExpr.substr(1, condExpr.length() - 2);
	/*look for an expected ! operator
	or if finding brackets, track in stack until we exit the brackets, and look for && or ||
	or if something else found, it should imply a rel expr */
	int bracketCount = 0;
	bool startOfExpr = true;
	bool checkForAndOr = false;
	for (unsigned int pos = 0; pos < condExpr.length(); pos++) {
		if (startOfExpr) {
			if (condExpr[pos] == '!') {
				//extract out the internal expr. Exclude ending brackets
				string nextCondExpr = condExpr.substr(pos + 1, condExpr.length()-pos);
				return checkCondExpr(nextCondExpr);
			}
			else if (condExpr[pos] == '(') {
				startOfExpr = false;
				bracketCount++;
			}
			else {
				//else no delimiter expected, assume it is a rel expr and strip brackets
				return checkRelExpr(condExpr);
			}
		}
		else if (checkForAndOr) {
			if (condExpr.substr(pos, 2) == "&&" || condExpr.substr(pos, 2) == "||") {
				//extract out the 2 expressions
				string firstCondExpr = condExpr.substr(0, pos);
				string secondCondExpr = condExpr.substr(pos + 2, string::npos);
				return checkCondExpr(firstCondExpr) & checkCondExpr(secondCondExpr);
			}
			else {
				cout << "Could not successfully parse conditional expression, missing and/or operator at line " << statementNumber << endl;
				return false;
			}
		}
		else {
			if (condExpr[pos] == '(') {
				bracketCount++;
				checkForAndOr = false;
			}
			else if (condExpr[pos] == ')') {
				bracketCount--;
				if (bracketCount == 0) {
					checkForAndOr = true;
				}
			}
		}
	}
	if (bracketCount > 0) {
		cout << "Mismatch in number of ( and ) brackets in conditional expression at line " << statementNumber << endl;
		return false;
	}
	cout << "Could not successfully parse the conditional expression at line " << statementNumber << endl;
	return false;
}																

bool Parser::checkRelExpr(string relExpr) {
	//should have only 1 of the relational operators
	//look for 2 character ones, then 1 character due to overlap in < and >
	size_t relOpPos = relExpr.find("==");
	relOpPos = min(relOpPos, relExpr.find("<="));
	relOpPos = min(relOpPos, relExpr.find(">="));
	relOpPos = min(relOpPos, relExpr.find("!="));
	int offset = 2;
	if (relOpPos == string::npos) {
		offset = 1;
		relOpPos = min(relOpPos, relExpr.find("<"));
		relOpPos = min(relOpPos, relExpr.find(">"));
	}
	if (relOpPos != string::npos) {
		string firstRelFactor = relExpr.substr(0, relOpPos);
		string secondRelFactor = relExpr.substr(relOpPos + offset, string::npos);
		return checkRelFactor(firstRelFactor) & checkRelFactor(secondRelFactor);
	}
	cout << "Could not find a relational operator at line " << statementNumber << endl;
	return false;
}

bool Parser::checkRelFactor(string relFactor) {
	relFactor = leftTrim(rightTrim(relFactor, " \t\n"), " \t");
	if (isValidVarName(relFactor)) {
		return true;
	}
	if (isValidConstant(relFactor)) {
		return true;
	}
	return checkExpr(relFactor);
}

int Parser::handleCloseBracket(string closeBracket) {
	//if no container statements tracked, assume close bracket is for procedure
	if (containerTracker.size() < 1) {
		if (emptyProcedure) {
			cout << "A procedure cannot be empty" << endl;
			return -1;
		}
		//clean up all relationship trackers
		containerTracker.clear();
		allFollowStack.clear();
		currentFollowVector.clear();
		parentVector.clear();
		withinProcedure = false;
		emptyProcedure = true;
		return 0;
	}
	else {
		//pop from parent stack, pop from stack of follow vectors for while, else
		//clear follow vector
		//set else checker and container for if
		currentFollowVector.clear();
		if (containerTracker.back() == WHILEC || containerTracker.back() == ELSEC) {
			if (parentVector.size() == 0) {
				cout << "Unexpected error when parsing end of container statement. Parent vector is empty. At line " << statementNumber << endl;
				return -1;
			}
			if (allFollowStack.size() == 0) {
				cout << "Unexpected error when parsing end of container statement. Follow stack is empty. At line " << statementNumber << endl;
				return -1;
			}
			parentVector.pop_back();
			currentFollowVector = allFollowStack.back();
			allFollowStack.pop_back();
			//track consecutive closed if/else statements
			setNext(statementNumber, containerTracker.back());
		}
		else if (containerTracker.back() == IFC) {
			expectElse = true;
			lastInIfTracker.push_back(statementNumber - 1);
		}
		containerTracker.pop_back();
	}
	return 0;
}

bool Parser::checkCall(string callLine) {
	string callRegexString = spaceRegex + "call" + spaceRegex + varNameRegex + spaceRegex + ";" + spaceRegex;
	regex callRegex(callRegexString);
	if (!regex_match(callLine, callRegex)) {
		cout << "Call statement is invalid at line " << statementNumber << endl;
		return false;
	}
	return true;
}

int Parser::handleCall(string callLine) {
	if (!checkCall(callLine)) {
		return -1;
	}
	//extract proc name
	size_t startPos = callLine.find_first_of("call");
	size_t endPos = callLine.find_first_of(";");
	string calledProcName = callLine.substr(startPos + 4, endPos - startPos - 4);
	calledProcName = leftTrim(calledProcName, " \t");
	calledProcName = rightTrim(calledProcName, " \t");

	setParent(statementNumber);
	setFollow(statementNumber);
	setNext(statementNumber, NONEC);
	pkb->insertCPRStmtType(statementNumber, CALL, calledProcName);
	currentFollowVector.push_back(statementNumber);
	
	procCalledByTable.insert({ currProcedure, statementNumber });
	setCalls(currProcedure, calledProcName);
	return 0;
}

bool Parser::setNext(int stmtNum, Container closingType) {
	//first statement in a procedure cannot possibly be 2nd argument in next
	//set boolean and return
	if (firstInProc) {
		firstInProc = false;
		return true;
	}
	//for case of first line in else
	if (firstInElse) {
		firstInElse = false;
		cout << parentVector.back() << " " << statementNumber << "first line in else " << endl;
		pkb->setNext(parentVector.back(), stmtNum);
		pkb->setPrevious(parentVector.back(), stmtNum);
		return true;
	}
	//for case of close bracket involving while
	if (closingType == WHILEC) {
		int lastWhile = parentVector.back();
		if (closedIfCount > 0) {
			while (closedIfCount > 0) {
				cout << lastInIfTracker.back() << " " << lastWhile << "closed while with if involved" << endl;
				cout << lastInElseTracker.back() << " " << lastWhile << endl;
				cout << lastWhile << " " << statementNumber << endl;
				
				pkb->setNext(lastInIfTracker.back(), lastWhile);
				pkb->setPrevious(lastInIfTracker.back(), lastWhile);
				pkb->setNext(lastInElseTracker.back(), lastWhile);
				pkb->setPrevious(lastInElseTracker.back(), lastWhile);
				pkb->setNext(lastWhile, statementNumber);
				pkb->setPrevious(lastWhile, statementNumber);
				
				lastInIfTracker.pop_back();
				lastInElseTracker.pop_back();
				closedIfCount--;
			}
		}
		else {
			cout << lastWhile << " " << statementNumber << " end of while with no if involved" << endl;
			cout << statementNumber - 1 << " " << lastWhile << endl;
			pkb->setNext(lastWhile, stmtNum);
			pkb->setPrevious(lastWhile, stmtNum);
			pkb->setNext(stmtNum - 1, lastWhile);
			pkb->setPrevious(stmtNum - 1, lastWhile);
		}
		return true;
	}
	//for case of close bracket involving else
	if (closingType == ELSEC) {
		lastInElseTracker.push_back(statementNumber - 1);
		closedIfCount++;
		return true;
	}
	//for case of not first line in else, neither is it closing bracket.
	if (closedIfCount > 0) {
		while (closedIfCount > 0) {
			cout << lastInIfTracker.back() << " " << statementNumber << "closed if no container " << endl;
			cout << lastInElseTracker.back() << " " << statementNumber << endl;
			
			pkb->setNext(lastInIfTracker.back(), stmtNum);
			pkb->setPrevious(lastInIfTracker.back(), stmtNum);
			pkb->setNext(lastInElseTracker.back(), stmtNum);
			pkb->setPrevious(lastInElseTracker.back(), stmtNum);
			
			lastInIfTracker.pop_back();
			lastInElseTracker.pop_back();
			closedIfCount--;
		}
	}
	else {
		cout << statementNumber - 1 << " " << statementNumber << "first nothing involved" << endl;
		pkb->setNext(stmtNum - 1, stmtNum);
		pkb->setPrevious(stmtNum - 1, stmtNum);
	}
	return true;
}

vector<string> Parser::loadFile(string fileName) {
	ifstream sourceFile;
	sourceFile.open(fileName);
	if (sourceFile.fail()) {
		throw;
	}
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
		string token = toTokenise.substr(0, delimiterPos);
		if (delimiterPos == string::npos) {
			toTokenise = "";
		}
		else {
			toTokenise = toTokenise.substr(delimiterPos + 1, string::npos);
		}
		if (!token.empty()) {
			tokenList.push_back(token);
		}
	}
	return tokenList;
}

bool Parser::setParent(int currStatementNum) {
	for (unsigned int i = 0; i < parentVector.size(); i++) {
		pkb->setParentT(parentVector[i], currStatementNum);
		pkb->setChildT(parentVector[i], currStatementNum);
	}
	if (parentVector.size() > 0) {
		pkb->setParent(parentVector.back(), currStatementNum);
		pkb->setChild(parentVector.back(), currStatementNum);
	}
	return true;
}

bool Parser::setFollow(int currStatementNum) {
	for (unsigned int i = 0; i < currentFollowVector.size(); i++) {
		pkb->setFollowsT(currentFollowVector[i], currStatementNum);
		pkb->setFollowedByT(currentFollowVector[i], currStatementNum);
	}
	if (currentFollowVector.size() > 0) {
		pkb->setFollows(currentFollowVector.back(), currStatementNum);
		pkb->setFollowedBy(currentFollowVector.back(), currStatementNum);
	}
	return true;
}

bool Parser::setModifies(int currStatementNum, string currProc, string varName) {
	for (unsigned int i = 0; i < parentVector.size(); i++) {
		pkb->setModifies(parentVector[i], varName);
	}
	pkb->setModifies(currStatementNum, varName);
	de.insertProcModifies(currProc, varName);
	return true;
}

bool Parser::setUses(int currStatementNum, string currProc, string varName) {
	for (unsigned int i = 0; i < parentVector.size(); i++) {
		pkb->setUses(parentVector[i], varName);
	}
	pkb->setUses(currStatementNum, varName);
	de.insertProcUses(currProc, varName);
	return true;
}

bool Parser::setCallUsesModifies() {
	unordered_set<string> procList = de.getProcList();
	unordered_map<string, unordered_set<string>> procModifiesTable = de.getProcModifiesTable();
	unordered_map<string, unordered_set<string>> procUsesTable = de.getProcUsesTable();
	for (const auto &elem : procCalledByTable) {
		string procName = elem.first;
		int stmtNum = elem.second;
		for (const auto &elem : procModifiesTable[procName]) {
			pkb->setModifies(stmtNum, elem);
		}
		for (const auto &elem : procUsesTable[procName]) {
			pkb->setUses(stmtNum, elem);
		}
	}
	return true;
}

bool Parser::setProcIndirectUsesModifies() {
	unordered_set<string> procList = de.getProcList();
	unordered_map<string, unordered_set<string>> procModifiesTable = de.getProcModifiesTable();
	unordered_map<string, unordered_set<string>> procUsesTable = de.getProcUsesTable();
	for (const auto &elem : procList) {
		string procName = elem;
		for (const auto &elem : procModifiesTable[procName]) {
			pkb->setModifies(procName, elem);
		}
		for (const auto &elem : procUsesTable[procName]) {
			pkb->setUses(procName, elem);
		}
	}
}

bool Parser::setCalls(string currProcedure, string calledProcName) {
	pkb->setCalls(currProcedure, calledProcName);
	pkb->setCalledBy(calledProcName, currProcedure);
	de.insertCall(currProcedure, calledProcName);
	return true;
}

bool Parser::setCallsT() {
	unordered_map<string, unordered_set<string>> callGraph = de.getCallGraph();
	unordered_set<string> procList = de.getProcList();
	//for each procedure, go through the calls graph with BFS and generate calls*.
	//then insert into PKB
	for (const auto &proc : procList) {
		queue<string> bfsQueue;
		for (const auto &calledProc : callGraph[proc]) {
			bfsQueue.push(calledProc);
		}
		while (bfsQueue.size() > 0) {
			string currCalledTProc = bfsQueue.front();
			bfsQueue.pop();
			pkb->setCallsT(proc, currCalledTProc);
			pkb->setCalledByT(currCalledTProc, proc);
			for (const auto &calledProc : callGraph[currCalledTProc]) {
				bfsQueue.push(calledProc);
			}
		}
	}
	return true;
}

void Parser::setPKB(PKB * p) {
	pkb = p;
}

void Parser::setStatementNumber(int sn) {
	statementNumber = sn;
}

int Parser::getStatementNumber() {
	return statementNumber;
}

void Parser::setParentVector(vector<int> v) {
	parentVector = v;
}

vector<int> Parser::getParentVector() {
	return parentVector;
}

void Parser::setCurrentFollowVector(vector<int> v) {
	currentFollowVector = v;
}

vector<int> Parser::getCurrentFollowVector() {
	return currentFollowVector;
}

void Parser::setAllFollowStack(vector<vector<int>> v) {
	allFollowStack = v;
}

vector<vector<int>> Parser::getAllFollowStack() {
	return allFollowStack;
}

string Parser::getCurrentProcedure() {
	return currProcedure;
}

unordered_map<string, int> Parser::getProcCalledByTable() {
	return procCalledByTable;
}