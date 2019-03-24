#pragma once
#include "Clause.h"
#include <map>

enum RefIdentity {
	STRING, INTEGER, ATTR_REF_STRING, ATTR_REF_INT, CONSTANT_VALUE, SYNONYM, INVALID_REF
};

class With : public Clause {
private:
	RefIdentity refOneIdentity;
	RefIdentity refTwoIdentity;
	map<pair<Type, AttrRef>, RefIdentity> refIdentityTable =
	{
		{make_pair(WITH_STRING, UNASSIGNED), STRING},
		{make_pair(WITH_INTEGER, UNASSIGNED), INTEGER},
		{make_pair(CONSTANT, VALUE), CONSTANT_VALUE},
		{make_pair(PROGLINE, UNASSIGNED), SYNONYM},
		{make_pair(PROCEDURE, PROC_NAME), ATTR_REF_STRING},
		{make_pair(CALL, PROC_NAME), ATTR_REF_STRING},
		{make_pair(VARIABLE, VAR_NAME), ATTR_REF_STRING},
		{make_pair(READ, VAR_NAME), ATTR_REF_STRING},
		{make_pair(PRINT, VAR_NAME), ATTR_REF_STRING},
		{make_pair(STATEMENT, STMT_NUM), ATTR_REF_INT},
		{make_pair(READ, STMT_NUM), ATTR_REF_INT},
		{make_pair(PRINT, STMT_NUM), ATTR_REF_INT},
		{make_pair(CALL, STMT_NUM), ATTR_REF_INT},
		{make_pair(WHILE, STMT_NUM), ATTR_REF_INT},
		{make_pair(IF, STMT_NUM), ATTR_REF_INT},
		{make_pair(ASSIGN, STMT_NUM), ATTR_REF_INT}
	}; 

	RefIdentity determineRefIdentity(DesignEntity&);
	unordered_set<int> getValidStmts(const Type&);
	unordered_set<string> getValidNames(const Type&);
	unordered_map<string, int> getIndexTable(const Type&);
	unordered_set<int> getStmtNums(const Type&, const string&);
	unordered_set<string> getCommonString(const unordered_set<string>&, const unordered_set<string>&);
	unordered_map<int, int> getCommonValueMap(const unordered_set<int>&, const unordered_set<int>&);
	unordered_map<string, unordered_set<string>> getStrStrPairWithCommonStr(const unordered_set<string>&);
	unordered_map<int, unordered_set<int>> getIntIntPairWithCommonStr(const unordered_set<string>&, const Type&, const Type&);
	unordered_map<string, unordered_set<int>> getStrIntPairWithCommonStr(const unordered_set<string>&, const Type&);


	Result* evaulateIntInt(const string&, const string&); // e.g. 12 = 15
	Result* evaulateIntSyn(const string&, const string&); // e.g. 12 = n
	Result* evaulateIntConstVal(const string&, const string&); // e.g. 12 = c.value
	Result* evaulateIntAttrRefInt(const string&, const string&, const Type&); // e.g. 12 = c.stmt#
	Result* evaluateStrStr(const string&, const string&); // e.g. "second" = "second"
	Result* evaluateStrAttrRefStr(const string&, const string&, const Type&); // e.g. "main" = p.procName
	Result* evaulateConstValSyn(const string&, const string&); // e.g. c.value = n
	Result* evaulateConstValConstVal(const string&, const string&); // e.g. c.value = c1.value
	Result* evaulateSynSyn(const string&, const string&); // e.g. n = n1
	Result* evaulateAttrRefIntSyn(const string&, const Type&, const string&);  // e.g. w.stmt# = n
	Result* evaulateAttrRefIntConstVal(const string&, const Type&, const string&); // e.g. a.stmt# = c.value
	Result* evaulateAttrRefIntAttrRefInt(const string&, const Type&, const string&, const Type&); // e.g. s.stmt# = w.stmt#
	Result* evaulateAttrRefStrAttrRefStr(const string&, const Type&, const string&, const Type&); // e.g. v.varName = p.procName
	
public:
	With(DesignEntity&, DesignEntity&);
	Result evaluate(const PKB&) override;
};
