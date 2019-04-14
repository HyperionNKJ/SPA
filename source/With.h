#pragma once
#include "Clause.h"
#include <map>

enum RefIdentity {
	STRING, INTEGER, ATTR_REF_STRING, ATTR_REF_INT, CONSTANT_VALUE, SYNONYM, INVALID_REF
};

// A class that represents With clause and its evaluation logic
class With : public Clause {
private:
	RefIdentity refOneIdentity;
	RefIdentity refTwoIdentity;
	std::map<std::pair<Type, AttrRef>, RefIdentity> refIdentityTable =
	{
		{std::make_pair(WITH_STRING, UNASSIGNED), STRING},
		{std::make_pair(WITH_INTEGER, UNASSIGNED), INTEGER},
		{std::make_pair(CONSTANT, VALUE), CONSTANT_VALUE},
		{std::make_pair(PROGLINE, UNASSIGNED), SYNONYM},
		{std::make_pair(PROCEDURE, PROC_NAME), ATTR_REF_STRING},
		{std::make_pair(CALL, PROC_NAME), ATTR_REF_STRING},
		{std::make_pair(VARIABLE, VAR_NAME), ATTR_REF_STRING},
		{std::make_pair(READ, VAR_NAME), ATTR_REF_STRING},
		{std::make_pair(PRINT, VAR_NAME), ATTR_REF_STRING},
		{std::make_pair(STATEMENT, STMT_NUM), ATTR_REF_INT},
		{std::make_pair(READ, STMT_NUM), ATTR_REF_INT},
		{std::make_pair(PRINT, STMT_NUM), ATTR_REF_INT},
		{std::make_pair(CALL, STMT_NUM), ATTR_REF_INT},
		{std::make_pair(WHILE, STMT_NUM), ATTR_REF_INT},
		{std::make_pair(IF, STMT_NUM), ATTR_REF_INT},
		{std::make_pair(ASSIGN, STMT_NUM), ATTR_REF_INT},
		{std::make_pair(SWITCH, STMT_NUM), ATTR_REF_INT} // extension
	}; 

	RefIdentity determineRefIdentity(DesignEntity&);
	std::unordered_set<int> getValidStmts(const Type&);
	std::unordered_set<std::string> getValidNames(const Type&);
	std::unordered_map<std::string, int> getIndexTable(const Type&);
	std::unordered_set<int> getStmtNums(const Type&, const std::string&);
	std::unordered_set<std::string> getCommonString(const std::unordered_set<std::string>&, const std::unordered_set<std::string>&);
	std::unordered_map<int, int> getCommonValueMap(const std::unordered_set<int>&, const std::unordered_set<int>&);
	std::unordered_map<std::string, std::unordered_set<std::string>> getStrStrPairWithCommonStr(const std::unordered_set<std::string>&);
	std::unordered_map<int, std::unordered_set<int>> getIntIntPairWithCommonStr(const std::unordered_set<std::string>&, const Type&, const Type&);
	std::unordered_map<std::string, std::unordered_set<int>> getStrIntPairWithCommonStr(const std::unordered_set<std::string>&, const Type&);


	Result evaulateIntInt(const std::string&, const std::string&); // e.g. 12 = 15
	Result evaulateIntSyn(const std::string&, const std::string&); // e.g. 12 = n
	Result evaulateIntConstVal(const std::string&, const std::string&); // e.g. 12 = c.value
	Result evaulateIntAttrRefInt(const std::string&, const std::string&, const Type&); // e.g. 12 = c.stmt#
	Result evaluateStrStr(const std::string&, const std::string&); // e.g. "second" = "second"
	Result evaluateStrAttrRefStr(const std::string&, const std::string&, const Type&); // e.g. "main" = p.procName
	Result evaulateConstValSyn(const std::string&, const std::string&); // e.g. c.value = n
	Result evaulateConstValConstVal(const std::string&, const std::string&); // e.g. c.value = c1.value
	Result evaulateSynSyn(const std::string&, const std::string&); // e.g. n = n1
	Result evaulateAttrRefIntSyn(const std::string&, const Type&, const std::string&);  // e.g. w.stmt# = n
	Result evaulateAttrRefIntConstVal(const std::string&, const Type&, const std::string&); // e.g. a.stmt# = c.value
	Result evaulateAttrRefIntAttrRefInt(const std::string&, const Type&, const std::string&, const Type&); // e.g. s.stmt# = w.stmt#
	Result evaulateAttrRefStrAttrRefStr(const std::string&, const Type&, const std::string&, const Type&); // e.g. v.varName = p.procName
	
public:
	With(DesignEntity&, DesignEntity&);
	Result evaluate(const PKB&) override;
};
