#include "stdafx.h"
#include "CppUnitTest.h"
#include "PKB.h"
#include "Type.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
	PKB pkb, pkb_affects;
	bool result;
	TEST_CLASS(TestPKB) {
	public:
		TEST_CLASS_INITIALIZE(initialSetup) {
			pkb = PKB();

			// pkb_affects consists of proc Second of Code 5,
			// Code 7 and Code 9 in Full SPA Requirements.
			// Code 5 section belongs in one proc and the other
			// code belongs in a second proc.
			pkb_affects.setNext(1, 2);
			pkb_affects.setPrevious(1, 2);
			pkb_affects.setNext(2, 3);
			pkb_affects.setPrevious(2, 3);
			pkb_affects.setNext(3, 4);
			pkb_affects.setPrevious(3, 4);
			pkb_affects.setNext(3, 7);
			pkb_affects.setPrevious(3, 7);
			pkb_affects.setNext(4, 5);
			pkb_affects.setPrevious(4, 5);
			pkb_affects.setNext(5, 6);
			pkb_affects.setPrevious(5, 6);
			pkb_affects.setNext(6, 3);
			pkb_affects.setPrevious(6, 3);
			pkb_affects.setNext(7, 8);
			pkb_affects.setPrevious(7, 8);
			pkb_affects.setNext(7, 9);
			pkb_affects.setPrevious(7, 9);
			pkb_affects.setNext(8, 10);
			pkb_affects.setPrevious(8, 10);
			pkb_affects.setNext(9, 10);
			pkb_affects.setPrevious(9, 10);
			pkb_affects.setNext(10, 11);
			pkb_affects.setPrevious(10, 11);
			pkb_affects.setNext(11, 12);
			pkb_affects.setPrevious(11, 12);
			pkb_affects.setNext(13, 14);
			pkb_affects.setPrevious(13, 14);
			pkb_affects.setNext(14, 15);
			pkb_affects.setPrevious(14, 15);
			pkb_affects.setNext(15, 16);
			pkb_affects.setPrevious(15, 16);
			pkb_affects.setNext(16, 17);
			pkb_affects.setPrevious(16, 17);
			pkb_affects.setNext(17, 18);
			pkb_affects.setPrevious(17, 18);

			pkb_affects.insertStmtType(1, ASSIGN);
			pkb_affects.insertStmtType(2, ASSIGN);
			pkb_affects.insertStmtType(4, ASSIGN);
			pkb_affects.insertStmtType(6, ASSIGN);
			pkb_affects.insertStmtType(8, ASSIGN);
			pkb_affects.insertStmtType(9, ASSIGN);
			pkb_affects.insertStmtType(10, ASSIGN);
			pkb_affects.insertStmtType(11, ASSIGN);
			pkb_affects.insertStmtType(12, ASSIGN);
			pkb_affects.insertStmtType(13, ASSIGN);
			pkb_affects.insertStmtType(15, ASSIGN);
			pkb_affects.insertStmtType(16, ASSIGN);
			pkb_affects.insertStmtType(18, ASSIGN);

			pkb_affects.insertStmtType(3, WHILE);
			pkb_affects.insertCPRStmtType(5, PRINT, "test");
			pkb_affects.insertStmtType(7, IF);
			pkb_affects.insertCPRStmtType(14, CALL, "A");
			pkb_affects.insertCPRStmtType(17, READ, "x");

			pkb_affects.setModifies(1, "x");
			pkb_affects.setModifies(2, "i");
			pkb_affects.setModifies(4, "x");
			pkb_affects.setModifies(6, "i");
			pkb_affects.setModifies(8, "x");
			pkb_affects.setModifies(9, "z");
			pkb_affects.setModifies(10, "z");
			pkb_affects.setModifies(11, "y");
			pkb_affects.setModifies(12, "x");
			pkb_affects.setModifies(13, "x");
			pkb_affects.setModifies("A", "x");
			pkb_affects.setModifies(15, "v");
			pkb_affects.setModifies(16, "x");
			pkb_affects.setModifies(17, "x");
			pkb_affects.setModifies(18, "v");

			pkb_affects.setUses(4, "x");
			pkb_affects.setUses(4, "y");
			pkb_affects.setUses(6, "i");
			pkb_affects.setUses(8, "x");
			pkb_affects.setUses(10, "z");
			pkb_affects.setUses(10, "x");
			pkb_affects.setUses(10, "i");
			pkb_affects.setUses(11, "z");
			pkb_affects.setUses(12, "x");
			pkb_affects.setUses(12, "y");
			pkb_affects.setUses(12, "z");
			pkb_affects.setUses(13, "a");
			pkb_affects.setUses(15, "x");
			pkb_affects.setUses(16, "a");
			pkb_affects.setUses(18, "x");
		}
		
		TEST_METHOD(TestInsertVar) {
			result = pkb.insertVar("var");
			Assert::IsTrue(result);

			result = pkb.insertVar("var");
			Assert::IsFalse(result);
		}

		TEST_METHOD(TestInsertConstant) {
			result = pkb.insertConstant(6);
			Assert::IsTrue(result);

			result = pkb.insertConstant(6);
			Assert::IsFalse(result);
		}

		TEST_METHOD(TestInsertProc) {
			result = pkb.insertProc("main");
			Assert::IsTrue(result);

			result = pkb.insertProc("main");
			Assert::IsFalse(result);
		}

		TEST_METHOD(TestInsertStmtType) {
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
			result = pkb.insertStmtType(16, SWITCH);
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
			result = pkb.insertStmtType(16, SWITCH);
			Assert::IsFalse(result);

			result = pkb.insertStmtType(16, CALL);
			Assert::IsFalse(result);
		}

		TEST_METHOD(TestInsertCPRStmtType) {
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
			result = pkb.insertPattern("a b +", 23);
			Assert::IsTrue(result);
			result = pkb.insertPattern("a b +", 24);
			Assert::IsTrue(result);
			result = pkb.insertPattern("a b +", 23);
			Assert::IsFalse(result);
		}

		TEST_METHOD(TestInsertFullPattern) {
			result = pkb.insertPattern("a b c * +", 23);
			Assert::IsTrue(result);
			result = pkb.insertPattern("a b c * +", 24);
			Assert::IsTrue(result);
			result = pkb.insertPattern("a b c * +", 23);
			Assert::IsFalse(result);
		}

		TEST_METHOD(TestInsertIfControlVar) {
			result = pkb.insertIfControlVar(32, "x");
			Assert::IsTrue(result);
			result = pkb.insertIfControlVar(32, "y");
			Assert::IsTrue(result);
			result = pkb.insertIfControlVar(32, "x");
			Assert::IsFalse(result);
		}

		TEST_METHOD(TestInsertWhileControlVar) {
			result = pkb.insertWhileControlVar(32, "x");
			Assert::IsTrue(result);
			result = pkb.insertWhileControlVar(32, "y");
			Assert::IsTrue(result);
			result = pkb.insertWhileControlVar(32, "x");
			Assert::IsFalse(result);
		}

		TEST_METHOD(TestInsertSwitchControlVar) {
			result = pkb.insertSwitchControlVar(32, "x");
			Assert::IsTrue(result);
			result = pkb.insertSwitchControlVar(32, "y");
			Assert::IsTrue(result);
			result = pkb.insertSwitchControlVar(32, "x");
			Assert::IsFalse(result);
		}

		TEST_METHOD(TestSetFollows) {
			result = pkb.setFollows(4, 7);
			Assert::IsTrue(result);

			result = pkb.setFollows(4, 7);
			Assert::IsFalse(result);
		}
		
		TEST_METHOD(TestSetFollowsT) {
			result = pkb.setFollowsT(4, 7);
			Assert::IsTrue(result);

			result = pkb.setFollowsT(4, 7);
			Assert::IsFalse(result);
		}
		
		TEST_METHOD(TestSetFollowedBy) {
			result = pkb.setFollowedBy(4, 7);
			Assert::IsTrue(result);

			result = pkb.setFollowedBy(4, 7);
			Assert::IsFalse(result);
		}

		TEST_METHOD(TestSetFollowedByT) {
			result = pkb.setFollowedByT(4, 7);
			Assert::IsTrue(result);

			result = pkb.setFollowedByT(4, 7);
			Assert::IsFalse(result);
		}

		TEST_METHOD(TestSetParent) {
			result = pkb.setParent(4, 7);
			Assert::IsTrue(result);

			result = pkb.setParent(4, 7);
			Assert::IsFalse(result);
		}

		TEST_METHOD(TestSetParentT) {
			result = pkb.setParentT(4, 7);
			Assert::IsTrue(result);

			result = pkb.setParentT(4, 7);
			Assert::IsFalse(result);
		}

		TEST_METHOD(TestSetChild) {
			result = pkb.setChild(4, 7);
			Assert::IsTrue(result);

			result = pkb.setChild(4, 7);
			Assert::IsFalse(result);
		}

		TEST_METHOD(TestSetChildT) {
			result = pkb.setChildT(4, 7);
			Assert::IsTrue(result);

			result = pkb.setChildT(4, 7);
			Assert::IsFalse(result);
		}

		TEST_METHOD(TestSetModifiesByStmtNum) {
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
			result = pkb.setModifies("main", "var");
			Assert::IsTrue(result);

			result = pkb.setModifies("main", "var");
			Assert::IsFalse(result);
		}

		TEST_METHOD(TestSetUsesByStmtNum) {
			result = pkb.setUses(4, "var");
			Assert::IsTrue(result);

			result = pkb.setUses(4, "var");
			Assert::IsFalse(result);
		}

		TEST_METHOD(TestSetUsesByProcName) {
			result = pkb.setUses("main", "var");
			Assert::IsTrue(result);

			result = pkb.setUses("main", "var");
			Assert::IsFalse(result);
		}

		TEST_METHOD(TestSetCalls) {
			result = pkb.setCalls("caller", "callee");
			Assert::IsTrue(result);
			result = pkb.setCalls("caller", "callee2");
			Assert::IsTrue(result);
			result = pkb.setCalls("caller", "callee");
			Assert::IsFalse(result);
		}

		TEST_METHOD(TestSetCalledBy) {
			result = pkb.setCalledBy("callee", "caller");
			Assert::IsTrue(result);
			result = pkb.setCalledBy("callee", "caller2");
			Assert::IsTrue(result);
			result = pkb.setCalledBy("callee", "caller");
			Assert::IsFalse(result);
		}

		TEST_METHOD(TestSetCallsT) {
			result = pkb.setCallsT("caller", "callee");
			Assert::IsTrue(result);
			result = pkb.setCallsT("caller", "callee2");
			Assert::IsTrue(result);
			result = pkb.setCallsT("caller", "callee");
			Assert::IsFalse(result);
		}

		TEST_METHOD(TestSetCalledByT) {
			result = pkb.setCalledByT("callee", "caller");
			Assert::IsTrue(result);
			result = pkb.setCalledByT("callee", "caller2");
			Assert::IsTrue(result);
			result = pkb.setCalledByT("callee", "caller");
			Assert::IsFalse(result);
		}

		TEST_METHOD(TestSetNext) {
			result = pkb.setNext(1, 2);
			Assert::IsTrue(result);
			result = pkb.setNext(2, 3);
			Assert::IsTrue(result);
			result = pkb.setNext(1, 2);
			Assert::IsFalse(result);
		}

		TEST_METHOD(TestSetPrevious) {
			result = pkb.setPrevious(5, 4);
			Assert::IsTrue(result);
			result = pkb.setPrevious(4, 3);
			Assert::IsTrue(result);
			result = pkb.setPrevious(5, 4);
			Assert::IsFalse(result);
		}

		// PQL side
		TEST_METHOD(TestGetParentChildrenPairs) {
			unordered_map<int, unordered_set<int>>expectedMap, testMap;
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

		TEST_METHOD(TestGetModifierUserPairs) {
			unordered_map<int, unordered_set<int>> expectedMap, testMap;
			expectedMap = {
				{1, {4, 8, 10, 12}},
				{2, {6, 10}},
				{4, {4, 8, 10, 12}},
				{6, {6, 10}},
				{8, {10, 12}},
				{9, {10}},
				{10, {11, 12}},
				{11, {12}}
			};

			testMap = pkb_affects.getModifierUserPairs();
			result = expectedMap == testMap;
			Assert::IsTrue(result);
		}
	};
}