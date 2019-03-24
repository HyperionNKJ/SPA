#include "stdafx.h"
#include "CppUnitTest.h"
#include "PKB.h"
#include "Type.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
	TEST_CLASS(TestPKB) {
	public:

		TEST_METHOD(TestInsertVar) {
			PKB pkb;
			bool result;

			result = pkb.insertVar("var");
			Assert::IsTrue(result);

			result = pkb.insertVar("var");
			Assert::IsFalse(result);
		}

		TEST_METHOD(TestInsertConstant) {
			PKB pkb;
			bool result;

			result = pkb.insertConstant(6);
			Assert::IsTrue(result);

			result = pkb.insertConstant(6);
			Assert::IsFalse(result);
		}

		TEST_METHOD(TestInsertProc) {
			PKB pkb;
			bool result;

			result = pkb.insertProc("main");
			Assert::IsTrue(result);

			result = pkb.insertProc("main");
			Assert::IsFalse(result);
		}

		TEST_METHOD(TestInsertStmtType) {
			PKB pkb;
			bool result;

			result = pkb.insertStmtType(10, STATEMENT);
			Assert::IsTrue(result);
			result = pkb.insertStmtType(11, READ);
			Assert::IsTrue(result);
			result = pkb.insertStmtType(12, WHILE);
			Assert::IsTrue(result);
			result = pkb.insertStmtType(13, IF);
			Assert::IsTrue(result);
			result = pkb.insertStmtType(14, ASSIGN);
			Assert::IsTrue(result);
			result = pkb.insertStmtType(15, PRINT);
			Assert::IsTrue(result);

			result = pkb.insertStmtType(10, STATEMENT);
			Assert::IsFalse(result);
			result = pkb.insertStmtType(11, READ);
			Assert::IsFalse(result);
			result = pkb.insertStmtType(12, WHILE);
			Assert::IsFalse(result);
			result = pkb.insertStmtType(13, IF);
			Assert::IsFalse(result);
			result = pkb.insertStmtType(14, ASSIGN);
			Assert::IsFalse(result);
			result = pkb.insertStmtType(15, PRINT);
			Assert::IsFalse(result);

			result = pkb.insertStmtType(16, CALL);
			Assert::IsFalse(result);
		}

		TEST_METHOD(TestInsertCPRStmtType) {
			PKB pkb;
			bool result;

			result = pkb.insertCPRStmtType(1, CALL, "proc");
			Assert::IsTrue(result);
			result = pkb.insertCPRStmtType(2, READ, "var1");
			Assert::IsTrue(result);
			result = pkb.insertCPRStmtType(3, PRINT, "var2");
			Assert::IsTrue(result);

			result = pkb.insertCPRStmtType(1, CALL, "proc");
			Assert::IsFalse(result);
			result = pkb.insertCPRStmtType(2, READ, "var1");
			Assert::IsFalse(result);
			result = pkb.insertCPRStmtType(3, PRINT, "var2");
			Assert::IsFalse(result);

			result = pkb.insertCPRStmtType(4, STATEMENT, "test");
			Assert::IsFalse(result);
		}

		TEST_METHOD(TestInsertPattern) {
			PKB pkb;
			bool result;

			result = pkb.insertPattern("a b +", 23);
			Assert::IsTrue(result);
			result = pkb.insertPattern("a b +", 24);
			Assert::IsTrue(result);
			result = pkb.insertPattern("a b +", 23);
			Assert::IsFalse(result);
		}

		TEST_METHOD(TestInsertFullPattern) {
			PKB pkb;
			bool result;

			result = pkb.insertPattern("a b c * +", 23);
			Assert::IsTrue(result);
			result = pkb.insertPattern("a b c * +", 24);
			Assert::IsTrue(result);
			result = pkb.insertPattern("a b c * +", 23);
			Assert::IsFalse(result);
		}

		TEST_METHOD(TestInsertIfControlVar) {
			PKB pkb;
			bool result;

			result = pkb.insertIfControlVar(32, "x");
			Assert::IsTrue(result);
			result = pkb.insertIfControlVar(32, "y");
			Assert::IsTrue(result);
			result = pkb.insertIfControlVar(32, "x");
			Assert::IsFalse(result);
		}

		TEST_METHOD(TestInsertWhileControlVar) {
			PKB pkb;
			bool result;

			result = pkb.insertWhileControlVar(32, "x");
			Assert::IsTrue(result);
			result = pkb.insertWhileControlVar(32, "y");
			Assert::IsTrue(result);
			result = pkb.insertWhileControlVar(32, "x");
			Assert::IsFalse(result);
		}

		TEST_METHOD(TestSetFollows) {
			PKB pkb;
			bool result;

			result = pkb.setFollows(4, 7);
			Assert::IsTrue(result);

			result = pkb.setFollows(4, 7);
			Assert::IsFalse(result);
		}
		
		TEST_METHOD(TestSetFollowsT) {
			PKB pkb;
			bool result;

			result = pkb.setFollowsT(4, 7);
			Assert::IsTrue(result);

			result = pkb.setFollowsT(4, 7);
			Assert::IsFalse(result);
		}
		
		TEST_METHOD(TestSetFollowedBy) {
			PKB pkb;
			bool result;

			result = pkb.setFollowedBy(4, 7);
			Assert::IsTrue(result);

			result = pkb.setFollowedBy(4, 7);
			Assert::IsFalse(result);
		}

		TEST_METHOD(TestSetFollowedByT) {
			PKB pkb;
			bool result;

			result = pkb.setFollowedByT(4, 7);
			Assert::IsTrue(result);

			result = pkb.setFollowedByT(4, 7);
			Assert::IsFalse(result);
		}

		TEST_METHOD(TestSetParent) {
			PKB pkb;
			bool result;

			result = pkb.setParent(4, 7);
			Assert::IsTrue(result);

			result = pkb.setParent(4, 7);
			Assert::IsFalse(result);
		}

		TEST_METHOD(TestSetParentT) {
			PKB pkb;
			bool result;

			result = pkb.setParentT(4, 7);
			Assert::IsTrue(result);

			result = pkb.setParentT(4, 7);
			Assert::IsFalse(result);
		}

		TEST_METHOD(TestSetChild) {
			PKB pkb;
			bool result;

			result = pkb.setChild(4, 7);
			Assert::IsTrue(result);

			result = pkb.setChild(4, 7);
			Assert::IsFalse(result);
		}

		TEST_METHOD(TestSetChildT) {
			PKB pkb;
			bool result;

			result = pkb.setChildT(4, 7);
			Assert::IsTrue(result);

			result = pkb.setChildT(4, 7);
			Assert::IsFalse(result);
		}

		TEST_METHOD(TestSetModifiesByStmtNum) {
			PKB pkb;
			bool result;

			result = pkb.setModifies(4, "var");
			Assert::IsTrue(result);

			result = pkb.setModifies(4, "var2");
			Assert::IsTrue(result);

			result = pkb.setModifies(5, "var");
			Assert::IsTrue(result);

			result = pkb.setModifies(4, "var");
			Assert::IsFalse(result);
		}

		TEST_METHOD(TestSetModifiesByProcName) {
			PKB pkb;
			bool result;

			result = pkb.setModifies("main", "var");
			Assert::IsTrue(result);

			result = pkb.setModifies("main", "var");
			Assert::IsFalse(result);
		}

		TEST_METHOD(TestSetUsesByStmtNum) {
			PKB pkb;
			bool result;

			result = pkb.setUses(4, "var");
			Assert::IsTrue(result);

			result = pkb.setUses(4, "var");
			Assert::IsFalse(result);
		}

		TEST_METHOD(TestSetUsesByProcName) {
			PKB pkb;
			bool result;

			result = pkb.setUses("main", "var");
			Assert::IsTrue(result);

			result = pkb.setUses("main", "var");
			Assert::IsFalse(result);
		}

		TEST_METHOD(TestSetCalls) {
			PKB pkb;
			bool result;

			result = pkb.setCalls("caller", "callee");
			Assert::IsTrue(result);
			result = pkb.setCalls("caller", "callee2");
			Assert::IsTrue(result);
			result = pkb.setCalls("caller", "callee");
			Assert::IsFalse(result);
		}

		TEST_METHOD(TestSetCalledBy) {
			PKB pkb;
			bool result;

			result = pkb.setCalledBy("callee", "caller");
			Assert::IsTrue(result);
			result = pkb.setCalledBy("callee", "caller2");
			Assert::IsTrue(result);
			result = pkb.setCalledBy("callee", "caller");
			Assert::IsFalse(result);
		}

		TEST_METHOD(TestSetCallsT) {
			PKB pkb;
			bool result;

			result = pkb.setCallsT("caller", "callee");
			Assert::IsTrue(result);
			result = pkb.setCallsT("caller", "callee2");
			Assert::IsTrue(result);
			result = pkb.setCallsT("caller", "callee");
			Assert::IsFalse(result);
		}

		TEST_METHOD(TestSetCalledByT) {
			PKB pkb;
			bool result;

			result = pkb.setCalledByT("callee", "caller");
			Assert::IsTrue(result);
			result = pkb.setCalledByT("callee", "caller2");
			Assert::IsTrue(result);
			result = pkb.setCalledByT("callee", "caller");
			Assert::IsFalse(result);
		}

		TEST_METHOD(TestSetNext) {
			PKB pkb;
			bool result;

			result = pkb.setNext(1, 2);
			Assert::IsTrue(result);
			result = pkb.setNext(2, 3);
			Assert::IsTrue(result);
			result = pkb.setNext(1, 2);
			Assert::IsFalse(result);
		}

		TEST_METHOD(TestSetPrevious) {
			PKB pkb;
			bool result;

			result = pkb.setPrevious(5, 4);
			Assert::IsTrue(result);
			result = pkb.setPrevious(4, 3);
			Assert::IsTrue(result);
			result = pkb.setPrevious(5, 4);
			Assert::IsFalse(result);
		}

		// PQL side
		TEST_METHOD(TestGetParentChildrenPairs) {
			PKB pkb;
			bool result;
			unordered_map<int, unordered_set<int>> expectedMap, testMap;
			expectedMap = {{1, {2, 3}}};

			pkb.insertStmtType(1, WHILE);
			pkb.insertStmtType(2, READ);
			pkb.insertStmtType(3, READ);
			pkb.insertStmtType(4, ASSIGN);

			pkb.setParent(1, 2);
			pkb.setParent(1, 3);
			pkb.setParent(1, 4);

			testMap = pkb.getParentChildrenPairs(WHILE, READ);
			result = expectedMap == testMap;
			Assert::IsTrue(result);
		}

	};
}