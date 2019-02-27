#include <regex>
#include "stdafx.h"
#include "CppUnitTest.h"
#include "QueryTokenizer.h"
#include "QueryParser.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
	TEST_CLASS(TestQueryProcessor) {
public:

	TEST_METHOD(parseDeclarativeStatementPass) {
		QueryTokenizer qt("assign a; stmt b,c,d;");
		qt.tokenize();
		std::vector<Statement> statementsResult = qt.getStatements();

		bool isTrue = (statementsResult[0].VALUE == "assign a"
			&& statementsResult[0].type == StatementType::DECLARATION
			&& statementsResult[1].VALUE == "stmt b,c,d"
			&& statementsResult[1].type == StatementType::DECLARATION);

		Assert::IsTrue(isTrue);
	}

	TEST_METHOD(parseSelectStatementPass) {
		QueryTokenizer qt("Select a1 such that Follows(a1,a2) pattern a2(_,_\"16\"_)");
		qt.tokenize();
		std::vector<Statement> statementsResult = qt.getStatements();

		bool isTrue = (statementsResult[0].VALUE == "Select a1 such that Follows(a1,a2) pattern a2(_,_\"16\"_)"
			&& statementsResult[0].type == StatementType::SELECT);

		Assert::IsTrue(isTrue);
	}

	TEST_METHOD(parseDeclarationAndSelectStatementPass) {
		QueryTokenizer qt("assign a1; assign a2;Select a1 such that Follows(a1,a2) pattern a2(_,_\"16\"_)");
		qt.tokenize();
		std::vector<Statement> statementsResult = qt.getStatements();

		bool isTrue = (statementsResult[0].VALUE == "assign a1"
			&& statementsResult[0].type == StatementType::DECLARATION
			&& statementsResult[1].VALUE == "assign a2"
			&& statementsResult[1].type == StatementType::DECLARATION
			&& statementsResult[2].VALUE == "Select a1 such that Follows(a1,a2) pattern a2(_,_\"16\"_)"
			&& statementsResult[2].type == StatementType::SELECT);

			Assert::IsTrue(isTrue);
	}

	TEST_METHOD(parseDeclarationStatementLeadingSpacePass) {
		QueryTokenizer qt("    \n    assign a1;     assign a2;     Select a1 such that Follows(a1,a2) pattern a2(_,_\"16\"_)");
		qt.tokenize();
		std::vector<Statement> statementsResult = qt.getStatements();

		bool isTrue = (statementsResult[0].VALUE == "assign a1"
			&& statementsResult[0].type == StatementType::DECLARATION
			&& statementsResult[1].VALUE == "assign a2"
			&& statementsResult[1].type == StatementType::DECLARATION
			&& statementsResult[2].VALUE == "Select a1 such that Follows(a1,a2) pattern a2(_,_\"16\"_)"
			&& statementsResult[2].type == StatementType::SELECT);

		Assert::IsTrue(isTrue);
	}

	TEST_METHOD(parseDeclarationStatementTrailingSpacePass) {
		QueryTokenizer qt("assign a1            \n;assign a2         ;Select a1 such that Follows(a1,a2) pattern a2(_,_\"16\"_);    \n ");
		qt.tokenize();
		std::vector<Statement> statementsResult = qt.getStatements();

		bool isTrue = (statementsResult[0].VALUE == "assign a1"
			&& statementsResult[0].type == StatementType::DECLARATION
			&& statementsResult[1].VALUE == "assign a2"
			&& statementsResult[1].type == StatementType::DECLARATION
			&& statementsResult[2].VALUE == "Select a1 such that Follows(a1,a2) pattern a2(_,_\"16\"_)"
			&& statementsResult[2].type == StatementType::SELECT);

		Assert::IsTrue(isTrue);
	}

	TEST_METHOD(parseDeclarationStatementLeadingAndTrailingSpacePass) {
		QueryTokenizer qt("          \n assign a1 ;assign a2 ;Select a1 such that Follows(a1,a2) pattern a2(_,_\"16\"_);    \n ");
		qt.tokenize();
		std::vector<Statement> statementsResult = qt.getStatements();

		bool isTrue = (statementsResult[0].VALUE == "assign a1"
			&& statementsResult[0].type == StatementType::DECLARATION
			&& statementsResult[1].VALUE == "assign a2"
			&& statementsResult[1].type == StatementType::DECLARATION
			&& statementsResult[2].VALUE == "Select a1 such that Follows(a1,a2) pattern a2(_,_\"16\"_)"
			&& statementsResult[2].type == StatementType::SELECT);

		Assert::IsTrue(isTrue);
	}

	TEST_METHOD(parseInvalidStatementFail) {
		QueryTokenizer qt("Follows(a1,a2) pattern a2(_,_\"16\"_);    \n ");
		bool isFalse = qt.tokenize();
		
		Assert::IsFalse(isFalse);
	}

	TEST_METHOD(parseDuplicateSynonymFail) {
		QueryTokenizer qt("assign a1,a1; assign a2;Select a1 such that Follows(a1,a2) pattern a2(_,_\"16\"_)");
		qt.tokenize();
		std::vector<Statement> statementsResult = qt.getStatements();
		QueryParser qp(statementsResult);
		bool isFalse = qp.parse();

		Assert::IsFalse(isFalse);
	}
	TEST_METHOD(parseKeywordSynonymPass) {
		QueryTokenizer qt("assign assign,a1; assign a2;Select a1 such that Follows(a1,a2) pattern a2(_,_\"16\"_)");
		qt.tokenize();
		std::vector<Statement> statementsResult = qt.getStatements();
		QueryParser qp(statementsResult);
		bool isFalse = qp.parse();

		Assert::IsFalse(isFalse);
	}

	TEST_METHOD(parseQueryPass) {
		QueryTokenizer qt("assign a1; assign a2;Select a1 such that Follows(a1,a2) pattern a2(_,_\"16\"_)");
		qt.tokenize();
		std::vector<Statement> statementsResult = qt.getStatements();
		QueryParser qp(statementsResult);
		bool isTrue = qp.parse();

		Assert::IsTrue(isTrue);
	}
	};
}