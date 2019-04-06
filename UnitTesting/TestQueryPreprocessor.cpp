#include <iostream>
#include "stdafx.h"
#include "CppUnitTest.h"
#include "QueryPreprocessorFormatter.h"
#include "QueryPreprocessorPatternParser.h"
#include "QueryPreprocessorTokenizer.h"
#include "QueryPreprocessorWithParser.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
	TEST_CLASS(TestQueryPreprocessor) {
public:
	TEST_METHOD(emptyQueryShouldNotCauseException) {
		std::string original = "";
		std::string expected = "";

		QueryPreprocessorFormatter formatter(original);
		std::string result = formatter.getFormattedQuery();

		Assert::IsTrue(result == expected);
	}
	TEST_METHOD(leadingWhitespaceRemoved) {
		std::string original = "\t\n\v\f\r          Select a;";
		std::string expected = "Select a;";

		QueryPreprocessorFormatter formatter(original);
		std::string result = formatter.getFormattedQuery();

		Assert::IsTrue(result == expected);
	}
	TEST_METHOD(trailingWhitespaceRemoved) {
		std::string original = "Select a;          \t\n\v\f\r";
		std::string expected = "Select a;";

		QueryPreprocessorFormatter formatter(original);
		std::string result = formatter.getFormattedQuery();

		Assert::IsTrue(result == expected);
	}
	TEST_METHOD(extraWhitespaceRemoved) {
		std::string original = "Select\t\n\v\f\r          a;";
		std::string expected = "Select a;";

		QueryPreprocessorFormatter formatter(original);
		std::string result = formatter.getFormattedQuery();

		Assert::IsTrue(result == expected);
	}
	TEST_METHOD(allExtraWhitespaceRemoved) {
		std::string original = "\t\n\v\f\r          Select\t\n\v\f\r          a;\t\n\v\f\r          ";
		std::string expected = "Select a;";

		QueryPreprocessorFormatter formatter(original);
		std::string result = formatter.getFormattedQuery();

		Assert::IsTrue(result == expected);
	}
	TEST_METHOD(spaceAfterConstantRHSWithClause) {
		std::string original = "stmt s1;\nassign a2;\nvariable v;\nSelect s1 such that Follows*(a2, s1) and Uses(a2, \"v\") with v = \"v\" pattern a2( _ , _ );";
		std::string expected = "stmt s1;assign a2;variable v;Select s1 such that Follows*(a2,s1) and Uses(a2,\"v\") with v=\"v\" pattern a2(_,_);";

		QueryPreprocessorFormatter formatter(original);
		std::string result = formatter.getFormattedQuery();

		Assert::IsTrue(result == expected);
	}
	TEST_METHOD(statementTokenizerShouldSplitSuccessfully) {
		std::string statementA = "stmt s1;";
		std::string statementB = "assign a2;";
		std::string statementC = "variable v;";
		std::string statementD = "Select s1 such that Follows*(a2,s1) and Uses(a2,\"v\") with v=\"v\" pattern a2(_,_);";
		std::string query = statementA + statementB + statementC + statementD;

		QueryPreprocessorTokenizer tokenizer(query);
		bool status = tokenizer.tokenize();
		std::vector<std::string> resultStatements = tokenizer.getStatements();

		Assert::IsTrue(status);
		Assert::IsTrue(resultStatements.size() == 4);
		Assert::IsTrue(resultStatements[0] + ";" == statementA);
		Assert::IsTrue(resultStatements[1] + ";" == statementB);
		Assert::IsTrue(resultStatements[2] + ";" == statementC);
		Assert::IsTrue(resultStatements[3] + ";" == statementD);
	}
	TEST_METHOD(selectStatementShouldBeAfterDeclareStatement) {
		std::string statementA = "stmt s1;";
		std::string statementB = "Select s1 such that Follows*(a2,s1) and Uses(a2,\"v\") with v=\"v\" pattern a2(_,_);";
		std::string query = statementB + statementA;

		QueryPreprocessorTokenizer tokenizer(query);
		bool status = tokenizer.tokenize();

		Assert::IsFalse(status);
	}
	TEST_METHOD(declareStatementMustExist) {
		std::string statementA = "Select s1 such that Follows*(a2,s1) and Uses(a2,\"v\") with v=\"v\" pattern a2(_,_);";
		std::string query = statementA;

		QueryPreprocessorTokenizer tokenizer(query);
		bool status = tokenizer.tokenize();

		Assert::IsFalse(status);
	}
	TEST_METHOD(withClauseWithSameRhsLhsValueNotAdded) {
		ProcessedQuery stubQuery;
		stubQuery.insertDeclaration("a", Type::ASSIGN);
		stubQuery.insertDeclaration("p", Type::PROCEDURE);
		stubQuery.insertDeclaration("pl", Type::PROGLINE);
		stubQuery.insertDeclaration("v", Type::VARIABLE);

		std::string withClauseA = "a.stmt#=a.stmt#";
		std::string withClauseB = "v.varName=v.varName";
		std::string withClauseC = "p.procName=p.procName";
		std::string withClauseD = "pl=pl";
		std::string withClauseE = "1=1";
		std::string withClauseF = "\"a\"=\"a\"";

		QueryPreprocessorWithParser withParserA(withClauseA, stubQuery);
		bool status = withParserA.parse();

		Assert::IsTrue(status);
		Assert::IsTrue(stubQuery.booleanClauses.size() == 0);
		Assert::IsTrue(stubQuery.withClauses.size() == 0);

		QueryPreprocessorWithParser withParserB(withClauseB, stubQuery);
		status = withParserB.parse();

		Assert::IsTrue(status);
		Assert::IsTrue(stubQuery.booleanClauses.size() == 0);
		Assert::IsTrue(stubQuery.withClauses.size() == 0);

		QueryPreprocessorWithParser withParserC(withClauseC, stubQuery);
		status = withParserC.parse();

		Assert::IsTrue(status);
		Assert::IsTrue(stubQuery.booleanClauses.size() == 0);
		Assert::IsTrue(stubQuery.withClauses.size() == 0);

		QueryPreprocessorWithParser withParserD(withClauseD, stubQuery);
		status = withParserD.parse();

		Assert::IsTrue(status);
		Assert::IsTrue(stubQuery.booleanClauses.size() == 0);
		Assert::IsTrue(stubQuery.withClauses.size() == 0);

		QueryPreprocessorWithParser withParserE(withClauseE, stubQuery);
		status = withParserE.parse();

		Assert::IsTrue(status);
		Assert::IsTrue(stubQuery.booleanClauses.size() == 0);
		Assert::IsTrue(stubQuery.withClauses.size() == 0);

		QueryPreprocessorWithParser withParserF(withClauseF, stubQuery);
		status = withParserF.parse();

		Assert::IsTrue(status);
		Assert::IsTrue(stubQuery.booleanClauses.size() == 0);
		Assert::IsTrue(stubQuery.withClauses.size() == 0);
	}
	TEST_METHOD(withClauseWithConstantRhsLhsValueReturnEmptyResult) {
		ProcessedQuery stubQuery;

		std::string withClauseA = "1=2";
		std::string withClauseB = "\"a\"=\"b\"";

		QueryPreprocessorWithParser withParserA(withClauseA, stubQuery);
		bool status = withParserA.parse();

		Assert::IsFalse(status);
		Assert::IsTrue(stubQuery.booleanClauses.size() == 0);
		Assert::IsTrue(stubQuery.withClauses.size() == 0);

		QueryPreprocessorWithParser withParserB(withClauseB, stubQuery);
		status = withParserB.parse();

		Assert::IsFalse(status);
		Assert::IsTrue(stubQuery.booleanClauses.size() == 0);
		Assert::IsTrue(stubQuery.withClauses.size() == 0);
	}
	TEST_METHOD(withClauseStmtNumStmtNumParsed) {
		ProcessedQuery stubQuery;
		stubQuery.insertDeclaration("a1", Type::ASSIGN);
		stubQuery.insertDeclaration("a2", Type::ASSIGN);

		std::string withClauseA = "a1.stmt#=a2.stmt#";

		QueryPreprocessorWithParser withParserA(withClauseA, stubQuery);
		bool status = withParserA.parse();

		Assert::IsTrue(status);
		Assert::IsTrue(stubQuery.booleanClauses.size() == 0);
		Assert::IsTrue(stubQuery.withClauses.size() == 1);

		DesignEntity paramOne = stubQuery.withClauses[0]->getParaOne();
		DesignEntity paramTwo = stubQuery.withClauses[0]->getParaTwo();

		Assert::IsTrue(paramOne.getValue() == "a1");
		Assert::IsTrue(paramOne.isType(Type::ASSIGN));
		Assert::IsTrue(paramOne.isAttrRef(AttrRef::STMT_NUM));
		Assert::IsTrue(paramTwo.getValue() == "a2");
		Assert::IsTrue(paramTwo.isType(Type::ASSIGN));
		Assert::IsTrue(paramTwo.isAttrRef(AttrRef::STMT_NUM));
	}
	TEST_METHOD(withClauseProcLineProcLineParsed) {
		ProcessedQuery stubQuery;
		stubQuery.insertDeclaration("p1", Type::PROGLINE);
		stubQuery.insertDeclaration("p2", Type::PROGLINE);

		std::string withClauseA = "p1=p2";

		QueryPreprocessorWithParser withParserA(withClauseA, stubQuery);
		bool status = withParserA.parse();

		Assert::IsTrue(status);
		Assert::IsTrue(stubQuery.booleanClauses.size() == 0);
		Assert::IsTrue(stubQuery.withClauses.size() == 1);

		DesignEntity paramOne = stubQuery.withClauses[0]->getParaOne();
		DesignEntity paramTwo = stubQuery.withClauses[0]->getParaTwo();

		Assert::IsTrue(paramOne.getValue() == "p1");
		Assert::IsTrue(paramOne.isType(Type::PROGLINE));
		Assert::IsTrue(paramOne.isAttrRef(AttrRef::UNASSIGNED));
		Assert::IsTrue(paramTwo.getValue() == "p2");
		Assert::IsTrue(paramOne.isType(Type::PROGLINE));
		Assert::IsTrue(paramOne.isAttrRef(AttrRef::UNASSIGNED));
	}
	TEST_METHOD(withClauseStmtNumProcLineParsed) {
		ProcessedQuery stubQuery;
		stubQuery.insertDeclaration("a", Type::ASSIGN);
		stubQuery.insertDeclaration("p", Type::PROGLINE);

		std::string withClauseA = "a.stmt#=p";

		QueryPreprocessorWithParser withParserA(withClauseA, stubQuery);
		bool status = withParserA.parse();

		Assert::IsTrue(status);
		Assert::IsTrue(stubQuery.booleanClauses.size() == 0);
		Assert::IsTrue(stubQuery.withClauses.size() == 1);

		DesignEntity paramOne = stubQuery.withClauses[0]->getParaOne();
		DesignEntity paramTwo = stubQuery.withClauses[0]->getParaTwo();

		Assert::IsTrue(paramOne.getValue() == "a");
		Assert::IsTrue(paramOne.isType(Type::ASSIGN));
		Assert::IsTrue(paramOne.isAttrRef(AttrRef::STMT_NUM));
		Assert::IsTrue(paramTwo.getValue() == "p");
		Assert::IsTrue(paramTwo.isType(Type::PROGLINE));
		Assert::IsTrue(paramTwo.isAttrRef(AttrRef::UNASSIGNED));
	}
	TEST_METHOD(withClauseStmtNumConstantParsed) {
		ProcessedQuery stubQuery;
		stubQuery.insertDeclaration("a", Type::ASSIGN);

		std::string withClauseA = "a.stmt#=1";

		QueryPreprocessorWithParser withParserA(withClauseA, stubQuery);
		bool status = withParserA.parse();

		Assert::IsTrue(status);
		Assert::IsTrue(stubQuery.booleanClauses.size() == 0);
		Assert::IsTrue(stubQuery.withClauses.size() == 1);

		DesignEntity paramOne = stubQuery.withClauses[0]->getParaOne();
		DesignEntity paramTwo = stubQuery.withClauses[0]->getParaTwo();

		Assert::IsTrue(paramOne.getValue() == "a");
		Assert::IsTrue(paramOne.isType(Type::ASSIGN));
		Assert::IsTrue(paramOne.isAttrRef(AttrRef::STMT_NUM));
		Assert::IsTrue(paramTwo.getValue() == "1");
		Assert::IsTrue(paramTwo.isType(Type::WITH_INTEGER));
		Assert::IsTrue(paramTwo.isAttrRef(AttrRef::UNASSIGNED));
	}
	TEST_METHOD(withClauseVarVarParsed) {
		ProcessedQuery stubQuery;
		stubQuery.insertDeclaration("v1", Type::VARIABLE);
		stubQuery.insertDeclaration("v2", Type::VARIABLE);

		std::string withClauseA = "v1.varName=v2.varName";

		QueryPreprocessorWithParser withParserA(withClauseA, stubQuery);
		bool status = withParserA.parse();

		Assert::IsTrue(status);
		Assert::IsTrue(stubQuery.booleanClauses.size() == 0);
		Assert::IsTrue(stubQuery.withClauses.size() == 1);

		DesignEntity paramOne = stubQuery.withClauses[0]->getParaOne();
		DesignEntity paramTwo = stubQuery.withClauses[0]->getParaTwo();

		Assert::IsTrue(paramOne.getValue() == "v1");
		Assert::IsTrue(paramOne.isType(Type::VARIABLE));
		Assert::IsTrue(paramOne.isAttrRef(AttrRef::VAR_NAME));
		Assert::IsTrue(paramTwo.getValue() == "v2");
		Assert::IsTrue(paramTwo.isType(Type::VARIABLE));
		Assert::IsTrue(paramTwo.isAttrRef(AttrRef::VAR_NAME));
	}
	TEST_METHOD(withClauseProcProcParsed) {
		ProcessedQuery stubQuery;
		stubQuery.insertDeclaration("p1", Type::PROCEDURE);
		stubQuery.insertDeclaration("p2", Type::PROCEDURE);

		std::string withClauseA = "p1.procName=p2.procName";

		QueryPreprocessorWithParser withParserA(withClauseA, stubQuery);
		bool status = withParserA.parse();

		Assert::IsTrue(status);
		Assert::IsTrue(stubQuery.booleanClauses.size() == 0);
		Assert::IsTrue(stubQuery.withClauses.size() == 1);

		DesignEntity paramOne = stubQuery.withClauses[0]->getParaOne();
		DesignEntity paramTwo = stubQuery.withClauses[0]->getParaTwo();

		Assert::IsTrue(paramOne.getValue() == "p1");
		Assert::IsTrue(paramOne.isType(Type::PROCEDURE));
		Assert::IsTrue(paramOne.isAttrRef(AttrRef::PROC_NAME));
		Assert::IsTrue(paramTwo.getValue() == "p2");
		Assert::IsTrue(paramTwo.isType(Type::PROCEDURE));
		Assert::IsTrue(paramTwo.isAttrRef(AttrRef::PROC_NAME));
	}
	TEST_METHOD(withClauseVarProcParsed) {
		ProcessedQuery stubQuery;
		stubQuery.insertDeclaration("v", Type::VARIABLE);
		stubQuery.insertDeclaration("p", Type::PROCEDURE);

		std::string withClauseA = "v.varName=p.procName";

		QueryPreprocessorWithParser withParserA(withClauseA, stubQuery);
		bool status = withParserA.parse();

		Assert::IsTrue(status);
		Assert::IsTrue(stubQuery.booleanClauses.size() == 0);
		Assert::IsTrue(stubQuery.withClauses.size() == 1);

		DesignEntity paramOne = stubQuery.withClauses[0]->getParaOne();
		DesignEntity paramTwo = stubQuery.withClauses[0]->getParaTwo();

		Assert::IsTrue(paramOne.getValue() == "v");
		Assert::IsTrue(paramOne.isType(Type::VARIABLE));
		Assert::IsTrue(paramOne.isAttrRef(AttrRef::VAR_NAME));
		Assert::IsTrue(paramTwo.getValue() == "p");
		Assert::IsTrue(paramTwo.isType(Type::PROCEDURE));
		Assert::IsTrue(paramTwo.isAttrRef(AttrRef::PROC_NAME));
	}
	TEST_METHOD(withClauseVarConstantParsed) {
		ProcessedQuery stubQuery;
		stubQuery.insertDeclaration("v", Type::VARIABLE);

		std::string withClauseA = "v.varName=\"var\"";

		QueryPreprocessorWithParser withParserA(withClauseA, stubQuery);
		bool status = withParserA.parse();

		Assert::IsTrue(status);
		Assert::IsTrue(stubQuery.booleanClauses.size() == 0);
		Assert::IsTrue(stubQuery.withClauses.size() == 1);

		DesignEntity paramOne = stubQuery.withClauses[0]->getParaOne();
		DesignEntity paramTwo = stubQuery.withClauses[0]->getParaTwo();

		Assert::IsTrue(paramOne.getValue() == "v");
		Assert::IsTrue(paramOne.isType(Type::VARIABLE));
		Assert::IsTrue(paramOne.isAttrRef(AttrRef::VAR_NAME));
		Assert::IsTrue(paramTwo.getValue() == "var");
		Assert::IsTrue(paramTwo.isType(Type::WITH_STRING));
		Assert::IsTrue(paramTwo.isAttrRef(AttrRef::UNASSIGNED));
	}
	TEST_METHOD(withClauseProcConstantParsed) {
		ProcessedQuery stubQuery;
		stubQuery.insertDeclaration("p", Type::PROCEDURE);

		std::string withClauseA = "p.procName=\"var\"";

		QueryPreprocessorWithParser withParserA(withClauseA, stubQuery);
		bool status = withParserA.parse();

		Assert::IsTrue(status);
		Assert::IsTrue(stubQuery.booleanClauses.size() == 0);
		Assert::IsTrue(stubQuery.withClauses.size() == 1);

		DesignEntity paramOne = stubQuery.withClauses[0]->getParaOne();
		DesignEntity paramTwo = stubQuery.withClauses[0]->getParaTwo();

		Assert::IsTrue(paramOne.getValue() == "p");
		Assert::IsTrue(paramOne.isType(Type::PROCEDURE));
		Assert::IsTrue(paramOne.isAttrRef(AttrRef::PROC_NAME));
		Assert::IsTrue(paramTwo.getValue() == "var");
		Assert::IsTrue(paramTwo.isType(Type::WITH_STRING));
		Assert::IsTrue(paramTwo.isAttrRef(AttrRef::UNASSIGNED));
	}
	TEST_METHOD(withClauseIntNameFail) {
		ProcessedQuery stubQuery;
		stubQuery.insertDeclaration("a", Type::ASSIGN);
		stubQuery.insertDeclaration("v", Type::VARIABLE);

		std::string withClauseA = "a.stmt#=v.varName";

		QueryPreprocessorWithParser withParserA(withClauseA, stubQuery);
		bool status = withParserA.parse();

		Assert::IsFalse(status);
	}
	TEST_METHOD(withClauseNameIntFail) {
		ProcessedQuery stubQuery;
		stubQuery.insertDeclaration("a", Type::ASSIGN);
		stubQuery.insertDeclaration("v", Type::VARIABLE);

		std::string withClauseA = "v.varName=a.stmt#";

		QueryPreprocessorWithParser withParserA(withClauseA, stubQuery);
		bool status = withParserA.parse();

		Assert::IsFalse(status);
	}
	TEST_METHOD(parseAssignPatternClauseUnderscoreUnderscorePass) {
		ProcessedQuery stubQuery;
		stubQuery.insertDeclaration("a", Type::ASSIGN);

		std::string patternCl = "a(_,_)";

		QueryPreprocessorPatternParser parser(patternCl, stubQuery);
		bool status = parser.parse();

		Assert::IsTrue(status);

		DesignEntity paramOne = stubQuery.otherClauses[0]->getParaOne();
		DesignEntity paramTwo = stubQuery.otherClauses[0]->getParaTwo();

		Assert::IsTrue(paramOne.getValue() == "");
		Assert::IsTrue(paramOne.isType(Type::UNDERSCORE));
		Assert::IsTrue(paramOne.isAttrRef(AttrRef::UNASSIGNED));
		Assert::IsTrue(paramTwo.getValue() == "");
		Assert::IsTrue(paramTwo.isType(Type::UNDERSCORE));
		Assert::IsTrue(paramTwo.isAttrRef(AttrRef::UNASSIGNED));
	}
	TEST_METHOD(parseAssignPatternClauseConstantUnderscorePass) {
		ProcessedQuery stubQuery;
		stubQuery.insertDeclaration("a", Type::ASSIGN);
		stubQuery.insertDeclaration("v", Type::VARIABLE);

		std::string patternCl = "a(v,_)";

		QueryPreprocessorPatternParser parser(patternCl, stubQuery);
		bool status = parser.parse();

		Assert::IsTrue(status);

		DesignEntity paramOne = stubQuery.otherClauses[0]->getParaOne();
		DesignEntity paramTwo = stubQuery.otherClauses[0]->getParaTwo();

		Assert::IsTrue(paramOne.getValue() == "v");
		Assert::IsTrue(paramOne.isType(Type::VARIABLE));
		Assert::IsTrue(paramOne.isAttrRef(AttrRef::UNASSIGNED));
		Assert::IsTrue(paramTwo.getValue() == "");
		Assert::IsTrue(paramTwo.isType(Type::UNDERSCORE));
		Assert::IsTrue(paramTwo.isAttrRef(AttrRef::UNASSIGNED));
	}
	TEST_METHOD(parseAssignPatternClauseVariableUnderscorePass) {
		ProcessedQuery stubQuery;
		stubQuery.insertDeclaration("a", Type::ASSIGN);

		std::string patternCl = "a(\"v\",_)";

		QueryPreprocessorPatternParser parser(patternCl, stubQuery);
		bool status = parser.parse();

		Assert::IsTrue(status);

		DesignEntity paramOne = stubQuery.otherClauses[0]->getParaOne();
		DesignEntity paramTwo = stubQuery.otherClauses[0]->getParaTwo();

		Assert::IsTrue(paramOne.getValue() == "v");
		Assert::IsTrue(paramOne.isType(Type::FIXED));
		Assert::IsTrue(paramOne.isAttrRef(AttrRef::UNASSIGNED));
		Assert::IsTrue(paramTwo.getValue() == "");
		Assert::IsTrue(paramTwo.isType(Type::UNDERSCORE));
		Assert::IsTrue(paramTwo.isAttrRef(AttrRef::UNASSIGNED));
	}
	TEST_METHOD(parseAssignPatternClauseUnderscoreConstantPass) {
		ProcessedQuery stubQuery;
		stubQuery.insertDeclaration("a", Type::ASSIGN);

		std::string patternCl = "a(_,\"v\")";

		QueryPreprocessorPatternParser parser(patternCl, stubQuery);
		bool status = parser.parse();

		Assert::IsTrue(status);

		DesignEntity paramOne = stubQuery.otherClauses[0]->getParaOne();
		DesignEntity paramTwo = stubQuery.otherClauses[0]->getParaTwo();

		Assert::IsTrue(paramOne.getValue() == "");
		Assert::IsTrue(paramOne.isType(Type::UNDERSCORE));
		Assert::IsTrue(paramOne.isAttrRef(AttrRef::UNASSIGNED));
		Assert::IsTrue(paramTwo.getValue() == "v");
		Assert::IsTrue(paramTwo.isType(Type::PATTERN_EXACT));
		Assert::IsTrue(paramTwo.isAttrRef(AttrRef::UNASSIGNED));
	}
	TEST_METHOD(parseAssignPatternClauseUnderscoreExactPass) {
		ProcessedQuery stubQuery;
		stubQuery.insertDeclaration("a", Type::ASSIGN);

		std::string patternCl = "a(_,\"((a+b)*(z+x))\")";

		QueryPreprocessorPatternParser parser(patternCl, stubQuery);
		bool status = parser.parse();

		Assert::IsTrue(status);

		DesignEntity paramOne = stubQuery.otherClauses[0]->getParaOne();
		DesignEntity paramTwo = stubQuery.otherClauses[0]->getParaTwo();

		Assert::IsTrue(paramOne.getValue() == "");
		Assert::IsTrue(paramOne.isType(Type::UNDERSCORE));
		Assert::IsTrue(paramOne.isAttrRef(AttrRef::UNASSIGNED));
		Assert::IsTrue(paramTwo.getValue() == "a b + z x + *");
		Assert::IsTrue(paramTwo.isType(Type::PATTERN_EXACT));
		Assert::IsTrue(paramTwo.isAttrRef(AttrRef::UNASSIGNED));
	}
	TEST_METHOD(parseAssignPatternClauseUnderscoreSubPass) {
		ProcessedQuery stubQuery;
		stubQuery.insertDeclaration("a", Type::ASSIGN);

		std::string patternCl = "a(_,_\"((a+b)*(z+x))\"_)";

		QueryPreprocessorPatternParser parser(patternCl, stubQuery);
		bool status = parser.parse();

		Assert::IsTrue(status);

		DesignEntity paramOne = stubQuery.otherClauses[0]->getParaOne();
		DesignEntity paramTwo = stubQuery.otherClauses[0]->getParaTwo();

		Assert::IsTrue(paramOne.getValue() == "");
		Assert::IsTrue(paramOne.isType(Type::UNDERSCORE));
		Assert::IsTrue(paramOne.isAttrRef(AttrRef::UNASSIGNED));
		Assert::IsTrue(paramTwo.getValue() == "a b + z x + *");
		Assert::IsTrue(paramTwo.isType(Type::PATTERN_SUB));
		Assert::IsTrue(paramTwo.isAttrRef(AttrRef::UNASSIGNED));
	}
	};
}
