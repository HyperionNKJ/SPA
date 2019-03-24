#include "With.h"

With::With(DesignEntity& refOne, DesignEntity& refTwo) : Clause(refOne, refTwo, ClauseType::WITH) {
	refOneIdentity = determineRefIdentity(refOne);
	refTwoIdentity = determineRefIdentity(refTwo);
}

RefIdentity With::determineRefIdentity(DesignEntity& ref) {
	Type type = ref.getType();
	AttrRef attrRef = ref.getAttrRef();
	RefIdentity refIdentity;

	try {
		refIdentity = refIdentityTable.at(make_pair(type, attrRef));
	}
	catch (out_of_range e) { // case where ref is not allowed in with clause, for example const.varName (type = CONSTANT, attrRef = VAR_NAME)
		refIdentity = INVALID;
	}
	return refIdentity;
}

Result With::evaluate(const PKB& pkb) {
	this->pkb = pkb;
	Result* result;

	if (refOneIdentity == INVALID || refTwoIdentity == INVALID) {
		result = new Result();
		result->setPassed(false);
	}

	string refOneValue = paraOne.getValue();
	Type refOneType = paraOne.getType();
	AttrRef refOneAttRef = paraOne.getAttrRef();
	string refTwoValue = paraTwo.getValue();
	Type refTwoType = paraTwo.getType();
	AttrRef refTwoAttRef = paraTwo.getAttrRef();

	if (refOneIdentity == INTEGER) {
		if (refTwoIdentity == ATTR_REF_INT) {
			result = evaulateIntAttrRefInt(refOneValue, refTwoValue, refTwoType);
		}
		else if (refTwoIdentity == INTEGER) {
			result = evaulateIntInt(refOneValue, refTwoValue);
		}
		else if (refTwoIdentity == CONSTANT_VALUE) {
			result = evaulateIntConstVal(refOneValue, refTwoValue);
		}
		else if (refTwoIdentity == SYNONYM) {
			result = evaulateIntSyn(refOneValue, refTwoValue);
		}
		else {
			result = new Result();
			result->setPassed(false);
		}
	} 
	else if (refOneIdentity == ATTR_REF_INT) {
		if (refTwoIdentity == INTEGER) {
			result = evaulateIntAttrRefInt(refTwoValue, refOneValue, refOneType);
		}
		else if (refTwoIdentity == ATTR_REF_INT) {
			result = evaulateAttrRefIntAttrRefInt(refOneValue, refOneType, refTwoValue, refTwoType);
		}
		else if (refTwoIdentity == CONSTANT_VALUE) {
			result = evaulateAttrRefIntConstVal(refOneValue, refOneType, refTwoValue);
		}
		else if (refTwoIdentity == SYNONYM) {
			result = evaulateAttrRefIntSyn(refOneValue, refOneType, refTwoValue);
		}
		else {
			result = new Result();
			result->setPassed(false);
		}
	}
	else if (refOneIdentity == STRING) {
		if (refTwoIdentity == STRING) {
			result = evaluateStrStr(refOneValue, refTwoValue);
		}
		else if (refTwoIdentity == ATTR_REF_STRING) {
			result = evaluateStrAttrRefStr(refOneValue, refTwoValue, refTwoType);
		}
		else {
			result = new Result();
			result->setPassed(false);
		}
	}
	else if (refOneIdentity == ATTR_REF_STRING) {
		if (refTwoIdentity == STRING) {
			result = evaluateStrAttrRefStr(refTwoValue, refOneValue, refOneType);
		}
		else if (refTwoIdentity == ATTR_REF_STRING) {
			result = evaulateAttrRefStrAttrRefStr(refOneValue, refOneType, refTwoValue, refTwoType);
		}
		else {
			result = new Result();
			result->setPassed(false);
		}
	}
	else if (refOneIdentity == CONSTANT_VALUE) {
		if (refTwoIdentity == INTEGER) {
			result = evaulateIntConstVal(refTwoValue, refOneValue);
		}
		else if (refTwoIdentity == ATTR_REF_INT) {
			result = evaulateAttrRefIntConstVal(refTwoValue, refTwoType, refOneValue);
		}
		else if (refTwoIdentity == CONSTANT_VALUE) {
			result = evaulateConstValConstVal(refOneValue, refOneValue);
		}
		else if (refTwoIdentity == SYNONYM) {
			result = evaulateConstValSyn(refOneValue, refTwoValue);
		}
		else {
			result = new Result();
			result->setPassed(false);
		}
	}
	else if (refOneIdentity == SYNONYM) {
		if (refTwoIdentity == INTEGER) {
			result = evaulateIntSyn(refTwoValue, refOneValue);
		}
		else if (refTwoIdentity == ATTR_REF_INT) {
			result = evaulateAttrRefIntSyn(refTwoValue, refTwoType, refOneValue);
		}
		else if (refTwoIdentity == CONSTANT_VALUE) {
			result = evaulateConstValSyn(refTwoValue, refOneValue);
		}
		else if (refTwoIdentity == SYNONYM) {
			result = evaulateSynSyn(refOneValue, refTwoValue);
		}
		else {
			result = new Result();
			result->setPassed(false);
		}
	}
	else {
		result = new Result();
		result->setPassed(false);
	}
	return *result;
}

// e.g. 12 = 15
Result* With::evaulateIntInt(const string& numOne, const string& numTwo) {
	return evaluateStrStr(numOne, numTwo);
} 

// e.g. 12 = n
Result* With::evaulateIntSyn(const string& num, const string& progLineSyn) {
	Result* result = new Result();
	int progLineNum = stoi(num);
	if (pkb.getAllStmts().size() >= progLineNum) {
		result->setPassed(true);
		result->setAnswer(progLineSyn, progLineNum);
	}
	else {
		result->setPassed(false);
	}
	return result;
}

// e.g. 12 = c.value
Result* With::evaulateIntConstVal(const string& num, const string& constSyn) {
	Result* result = new Result();
	int constValue = stoi(num);
	unordered_set<int> constants = pkb.getAllConstant();
	if (constants.count(constValue)) {
		result->setPassed(true);
		result->setAnswer(constSyn, constValue);
	}
	else {
		result->setPassed(false);
	}
	return result;
}

// e.g. 12 = c.stmt#
Result* With::evaulateIntAttrRefInt(const string& num, const string& synonym, const Type& type) {
	Result* result = new Result();
	int stmtNum = stoi(num);
	unordered_set<int> validStatements = getValidStmts(type);
	if (validStatements.count(stmtNum)) {
		result->setPassed(true);
		result->setAnswer(synonym, stmtNum);
	}
	else {
		result->setPassed(false);
	}
	return result;
}

// e.g. "second" = "second"
Result* With::evaluateStrStr(const string& stringOne, const string& stringTwo) {
	Result* result = new Result();
	result->setPassed(stringOne == stringTwo);
	return result;
}

// e.g. "main" = p.procName
Result* With::evaluateStrAttrRefStr(const string& name, const string& synonym, const Type& type) {
	Result* result = new Result();
	unordered_set<string> validNames = getValidNames(type);
	if (validNames.count(name)) {
		result->setPassed(true);
		if (type == PROCEDURE || type == VARIABLE) {
			result->setAnswer(synonym, { name }, getIndexTable(type));
		}
		else { // type must be call or read or print
			result->setAnswer(synonym, getStmtNums(type, name));
		}
	}
	else {
		result->setPassed(false);
	}
	return result;
}

// e.g. c.value = n
Result* With::evaulateConstValSyn(const string& constSyn, const string& progLineSyn) {
	Result* result = new Result();
	unordered_set<int> allConstants = pkb.getAllConstant();
	unordered_set<int> allProgLines = pkb.getAllStmts();
	unordered_map<int, int> intersectionMap = getCommonValueMap(allConstants, allProgLines);
	if (!intersectionMap.empty()) {
		result->setPassed(true);
		result->setAnswer(constSyn, progLineSyn, intersectionMap);
	}
	else {
		result->setPassed(false);
	}
	return result;
}

// e.g. c.value = c1.value
Result* With::evaulateConstValConstVal(const string& constSynOne, const string& constSynTwo) {
	Result* result = new Result();
	unordered_set<int> allConstants = pkb.getAllConstant();
	if (!allConstants.empty()) {
		result->setPassed(true);
		result->setAnswer(constSynOne, constSynTwo, getCommonValueMap(allConstants, allConstants));
	}
	else {
		result->setPassed(false);
	}
	return result;
}

// e.g. n = n1
Result* With::evaulateSynSyn(const string& progLineSynOne, const string& progLineSynTwo) {
	Result* result = new Result();
	unordered_set<int> allProgLines = pkb.getAllStmts();
	result->setPassed(true); // program must contain at least one statement / program line. Hence n = n1 is always true!
	result->setAnswer(progLineSynOne, progLineSynTwo, getCommonValueMap(allProgLines, allProgLines));
	return result;
}

// e.g. w.stmt# = n
Result* With::evaulateAttrRefIntSyn(const string& synonym, const Type& type, const string& progLineSyn) {
	Result* result = new Result();
	unordered_set<int> validStatements = getValidStmts(type);
	if (!validStatements.empty()) {
		result->setPassed(true); // validStatements is subset of n. Hence, intersection is simply validStatements.
		result->setAnswer(synonym, progLineSyn, getCommonValueMap(validStatements, validStatements)); 
	}
	else {
		result->setPassed(false);
	}
	return result;
}

// e.g. a.stmt# = c.value
Result* With::evaulateAttrRefIntConstVal(const string& synonym, const Type& type, const string& constSyn) {
	Result* result = new Result();
	unordered_set<int> validStatements = getValidStmts(type);
	unordered_set<int> allConstants = pkb.getAllConstant();
	unordered_map<int, int> intersectionMap = getCommonValueMap(allConstants, validStatements);
	if (!intersectionMap.empty()) {
		result->setPassed(true); 
		result->setAnswer(synonym, constSyn, intersectionMap);
	}
	else {
		result->setPassed(false);
	}
	return result;
}

// e.g. s.stmt# = w.stmt#
Result* With::evaulateAttrRefIntAttrRefInt(const string& synOne, const Type& typeOne, const string& synTwo, const Type& typeTwo) {
	Result* result = new Result();

	if (typeOne == STATEMENT || typeTwo == STATEMENT || typeOne == typeTwo) { 
		unordered_set<int> validStmtsOne = getValidStmts(typeOne);
		unordered_set<int> validStmtsTwo = getValidStmts(typeTwo);
		unordered_map<int, int> intersectionMap = getCommonValueMap(validStmtsOne, validStmtsTwo);
		if (!intersectionMap.empty()) {
			result->setPassed(true);
			result->setAnswer(synOne, synTwo, intersectionMap);
		}
		else {
			result->setPassed(false);
		}
	}
	else { // impossible for a statement to have 2 different types
		result->setPassed(false);
	}
	return result;
}

// e.g. v.varName = p.procName
Result* With::evaulateAttrRefStrAttrRefStr(const string& synOne, const Type& typeOne, const string& synTwo, const Type& typeTwo) {
	Result* result = new Result();
	unordered_set<string> validNamesOne = getValidNames(typeOne);
	unordered_set<string> validNamesTwo = getValidNames(typeTwo);
	unordered_set<string> commonString = getCommonString(validNamesOne, validNamesTwo);

	if (!commonString.empty()) {
		result->setPassed(true);
		bool oneIsStringTyped = (typeOne == PROCEDURE || typeOne == VARIABLE);
		bool twoIsStringTyped = (typeTwo == PROCEDURE || typeTwo == VARIABLE);

		if (oneIsStringTyped && twoIsStringTyped) { // e.g. proc.procName = var.varName, proc is string typed and var is string typed
			result->setAnswer(synOne, synTwo, getStrStrPairWithCommonStr(commonString), getIndexTable(typeOne), getIndexTable(typeTwo));
		}
		else if (!oneIsStringTyped && !twoIsStringTyped) { // e.g. call.procName = read.varName, call is int typed and read is int typed
			result->setAnswer(synOne, synTwo, getIntIntPairWithCommonStr(commonString, typeOne, typeTwo));
		}
		else if (oneIsStringTyped && !twoIsStringTyped) { // // e.g. var.varName = print.varName, var is string typed and print is int typed
			result->setAnswer(synOne, synTwo, getStrIntPairWithCommonStr(commonString, typeTwo), getIndexTable(typeOne));
		}
		else { // // e.g. read.varName = proc.procName, read is int typed and proc is string typed
			result->setAnswer(synTwo, synOne, getStrIntPairWithCommonStr(commonString, typeOne), getIndexTable(typeTwo));
		}
	}
	else {
		result->setPassed(false);
	}
	return result;
}

unordered_map<string, unordered_set<int>> With::getStrIntPairWithCommonStr(const unordered_set<string>& commonStr, const Type& type) {
	unordered_map<string, unordered_set<int>> answer;
	for (auto str : commonStr) {
		unordered_set<int> validStmts = getStmtNums(type, str);
		answer.insert({ str, validStmts });
	}
	return answer;
}

unordered_map<int, unordered_set<int>> With::getIntIntPairWithCommonStr(const unordered_set<string>& commonStr, const Type& typeOne, const Type& typeTwo) {
	unordered_map<int, unordered_set<int>> stmtPair;
	for (auto str : commonStr) {
		unordered_set<int> validStmtsOne = getStmtNums(typeOne, str);
		unordered_set<int> validStmtsTwo = getStmtNums(typeTwo, str);
		for (auto stmtNum : validStmtsOne) {
			stmtPair.insert({ stmtNum, validStmtsTwo });
		}
	}
	return stmtPair;
}

// Function to duplicate a string set to map = {x, {x}}. This format is required by Result's setAnswer function.
unordered_map<string, unordered_set<string>> With::getStrStrPairWithCommonStr(const unordered_set<string>& set) {
	unordered_map<string, unordered_set<string>> duplicatedMap;
	for (auto str : set) {
		duplicatedMap.insert({ str, {str} });
	}
	return duplicatedMap;
}

unordered_set<int> With::getValidStmts(const Type& type) {
	switch (type) {
	case STATEMENT:
		return pkb.getAllStmts();
	case READ:
		return pkb.getReadStmts();
	case PRINT:
		return pkb.getPrintStmts();
	case CALL:
		return pkb.getCallStmts();
	case WHILE:
		return pkb.getWhileStmts();
	case IF:
		return pkb.getIfStmts();
	case ASSIGN:
		return pkb.getAssignStmts();
	default:
		return {};
	}
}

unordered_set<string> With::getValidNames(const Type& type) {
	switch (type) {
	case PROCEDURE:
		return pkb.getAllProcedures();
	case CALL:
		return pkb.getCallProcNames();
	case VARIABLE:
		return pkb.getAllVariables();
	case READ:
		return pkb.getReadVarNames();
	case PRINT:
		return pkb.getPrintVarNames();
	default:
		return {};
	}
}

unordered_map<string, int> With::getIndexTable(const Type& type) {
	switch (type) {
	case PROCEDURE:
		return pkb.getProcTable();
	case VARIABLE:
		return pkb.getVarTable();
	default:
		return {};
	}
}

unordered_set<int> With::getStmtNums(const Type& type, const string& name) {
	switch (type) {
	case CALL:
		return pkb.getCallStmtsWithProc(name);
	case READ:
		return pkb.getReadStmtsWithVar(name);
	case PRINT:
		return pkb.getPrintStmtsWithVar(name);
	default:
		return {};
	}
}

// Utility function to find intersection of 2 int sets. Can also be used to duplicate a set to map = {x, x} e.g. Set = {1, 2, 3} Map = { {1, 1}, {2, 2}, {3, 3} }
unordered_map<int, int> With::getCommonValueMap(const unordered_set<int>& setOne, const unordered_set<int>& setTwo) {
	unordered_map<int, int> intersectionMap;
	for (auto elem : setOne) {
		if (setTwo.count(elem)) {
			intersectionMap.insert({ elem, elem });
		}
	}
	return intersectionMap;
}

// Utility function to find intersection of 2 string sets.
unordered_set<string> With::getCommonString(const unordered_set<string>& setOne, const unordered_set<string>& setTwo) {
	unordered_set<string> intersectionMap;
	for (auto elem : setOne) {
		if (setTwo.count(elem)) {
			intersectionMap.insert(elem);
		}
	}
	return intersectionMap;
}
