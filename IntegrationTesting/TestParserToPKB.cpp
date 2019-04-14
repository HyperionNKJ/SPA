#include "stdafx.h"
#include "CppUnitTest.h"
#include "Parser.h"
#include "PKB.h"
#include "Type.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace IntegrationTesting {		
	TEST_CLASS(TestParserToPKB) {
	public:

		TEST_METHOD(TestInsertProcedure) {
			Parser parser;
			PKB pkb;
			parser.setPKB(&pkb);
			unordered_set<string> procs;

			procs = pkb.getAllProcedures();
			Assert::AreEqual(0, (int) procs.size(), L"incorrect", LINE_INFO());

			string procName = "myProcOne";
			parser.handleProcedure("procedure myProcOne {");

			procs = pkb.getAllProcedures();
			Assert::AreEqual(1, (int) procs.size(), L"incorrect", LINE_INFO());

			unordered_set<string> expectedProcs;
			expectedProcs.insert(procName);
			bool equalProcs = procs == expectedProcs;
			Assert::IsTrue(equalProcs, L"incorrect", LINE_INFO());
		}

		TEST_METHOD(TestInsertRead) {
			Parser parser;
			PKB pkb;
			parser.setPKB(&pkb);
			int stmtNumber = 5;
			parser.setStatementNumber(stmtNumber);
			parser.setCurrentFollowVector({ 3, 4 });
			parser.setParentVector({ 1 , 2 });
			pkb.insertStmtType(1, WHILE);
			pkb.insertStmtType(2, WHILE);
			pkb.insertStmtType(3, READ);
			pkb.insertStmtType(4, PRINT);
			parser.handleRead(" read ABC ;");

			Assert::IsTrue(pkb.isFollows(4, 5), L"Incorrect", LINE_INFO());
			Assert::IsTrue(pkb.isFollowsT(4, 5), L"Incorrect", LINE_INFO());
			Assert::IsTrue(pkb.isFollowsT(3, 5), L"Incorrect", LINE_INFO());
			Assert::IsTrue(pkb.isParent(2, 5), L"Incorrect", LINE_INFO());
			Assert::IsTrue(pkb.isParentT(2, 5), L"Incorrect", LINE_INFO());
			Assert::IsTrue(pkb.isParentT(1, 5), L"Incorrect", LINE_INFO());
			Assert::IsTrue(pkb.isModifies(5, "ABC"), L"Incorrect", LINE_INFO());
			Assert::IsTrue(pkb.isModifies(2, "ABC"), L"Incorrect", LINE_INFO());
			Assert::IsTrue(pkb.isModifies(1, "ABC"), L"Incorrect", LINE_INFO());
		}

		TEST_METHOD(TestInsertPrint) {
			Parser parser;
			PKB pkb;
			parser.setPKB(&pkb);
			int stmtNumber = 5;
			parser.setStatementNumber(stmtNumber);
			parser.setCurrentFollowVector({ 3, 4 });
			parser.setParentVector({ 1 , 2 });
			pkb.insertStmtType(1, WHILE);
			pkb.insertStmtType(2, WHILE);
			pkb.insertStmtType(3, READ);
			pkb.insertStmtType(4, PRINT);

			parser.handlePrint("		print ABC ;");
			Assert::IsTrue(pkb.isFollows(4, 5), L"Incorrect", LINE_INFO());
			Assert::IsTrue(pkb.isFollowsT(4, 5), L"Incorrect", LINE_INFO());
			Assert::IsTrue(pkb.isFollowsT(3, 5), L"Incorrect", LINE_INFO());
			Assert::IsTrue(pkb.isParent(2, 5), L"Incorrect", LINE_INFO());
			Assert::IsTrue(pkb.isParentT(2, 5), L"Incorrect", LINE_INFO());
			Assert::IsTrue(pkb.isParentT(1, 5), L"Incorrect", LINE_INFO());
			Assert::IsTrue(pkb.isUses(5, "ABC"), L"Incorrect", LINE_INFO());
			Assert::IsTrue(pkb.isUses(2, "ABC"), L"Incorrect", LINE_INFO());
			Assert::IsTrue(pkb.isUses(1, "ABC"), L"Incorrect", LINE_INFO());
		}

		TEST_METHOD(TestInsertWhile) {
			Parser parser;
			PKB pkb;
			parser.setPKB(&pkb);
			int stmtNumber = 5;
			parser.setStatementNumber(stmtNumber);
			parser.setCurrentFollowVector({ 3, 4 });
			parser.setParentVector({ 1 , 2 });
			pkb.insertStmtType(1, WHILE);
			pkb.insertStmtType(2, WHILE);
			pkb.insertStmtType(3, READ);
			pkb.insertStmtType(4, PRINT);

			parser.handleProcedure("procedure test{");
			parser.handleWhile("   while  ( a <= ((123*b)+12))  {");
			Assert::IsTrue(pkb.isFollows(4, 5), L"Incorrect", LINE_INFO());
			Assert::IsTrue(pkb.isFollowsT(4, 5), L"Incorrect", LINE_INFO());
			Assert::IsTrue(pkb.isFollowsT(3, 5), L"Incorrect", LINE_INFO());
			Assert::IsTrue(pkb.isParent(2, 5), L"Incorrect", LINE_INFO());
			Assert::IsTrue(pkb.isParentT(2, 5), L"Incorrect", LINE_INFO());
			Assert::IsTrue(pkb.isParentT(1, 5), L"Incorrect", LINE_INFO());
			Assert::IsTrue(pkb.isUses(5, "a"), L"Incorrect", LINE_INFO());
			Assert::IsTrue(pkb.isUses(2, "a"), L"Incorrect", LINE_INFO());
			Assert::IsTrue(pkb.isUses(1, "a"), L"Incorrect", LINE_INFO());
			Assert::IsTrue(pkb.isUses(5, "b"), L"Incorrect", LINE_INFO());
			Assert::IsTrue(pkb.isUses(2, "b"), L"Incorrect", LINE_INFO());
			Assert::IsTrue(pkb.isUses(1, "b"), L"Incorrect", LINE_INFO());
		}

		TEST_METHOD(TestInsertIf) {
			Parser parser;
			PKB pkb;
			parser.setPKB(&pkb);
			int stmtNumber = 5;
			parser.setStatementNumber(stmtNumber);
			parser.setCurrentFollowVector({ 3, 4 });
			parser.setParentVector({ 1 , 2 });
			pkb.insertStmtType(1, WHILE);
			pkb.insertStmtType(2, WHILE);
			pkb.insertStmtType(3, READ);
			pkb.insertStmtType(4, PRINT);

			parser.handleIf("   if  ( a <= ((123*b)+12)) then  {");
			Assert::IsTrue(pkb.isFollows(4, 5), L"Incorrect", LINE_INFO());
			Assert::IsTrue(pkb.isFollowsT(4, 5), L"Incorrect", LINE_INFO());
			Assert::IsTrue(pkb.isFollowsT(3, 5), L"Incorrect", LINE_INFO());
			Assert::IsTrue(pkb.isParent(2, 5), L"Incorrect", LINE_INFO());
			Assert::IsTrue(pkb.isParentT(2, 5), L"Incorrect", LINE_INFO());
			Assert::IsTrue(pkb.isParentT(1, 5), L"Incorrect", LINE_INFO());
			Assert::IsTrue(pkb.isUses(5, "a"), L"Incorrect", LINE_INFO());
			Assert::IsTrue(pkb.isUses(2, "a"), L"Incorrect", LINE_INFO());
			Assert::IsTrue(pkb.isUses(1, "a"), L"Incorrect", LINE_INFO());
			Assert::IsTrue(pkb.isUses(5, "b"), L"Incorrect", LINE_INFO());
			Assert::IsTrue(pkb.isUses(2, "b"), L"Incorrect", LINE_INFO());
			Assert::IsTrue(pkb.isUses(1, "b"), L"Incorrect", LINE_INFO());
		}

		TEST_METHOD(TestInsertCall) {
			Parser parser;
			PKB pkb;
			parser.setPKB(&pkb);
			int stmtNumber = 5;
			parser.setStatementNumber(stmtNumber);
			parser.setCurrentFollowVector({ 3, 4 });
			parser.setParentVector({ 1 , 2 });
			parser.setProcedure("me");
			pkb.insertStmtType(1, WHILE);
			pkb.insertStmtType(2, WHILE);
			pkb.insertStmtType(3, READ);
			pkb.insertStmtType(4, PRINT);

			parser.handleCall("   call  other  ;");
			Assert::IsTrue(pkb.isFollows(4, 5), L"Incorrect", LINE_INFO());
			Assert::IsTrue(pkb.isFollowsT(4, 5), L"Incorrect", LINE_INFO());
			Assert::IsTrue(pkb.isFollowsT(3, 5), L"Incorrect", LINE_INFO());
			Assert::IsTrue(pkb.isParent(2, 5), L"Incorrect", LINE_INFO());
			Assert::IsTrue(pkb.isParentT(2, 5), L"Incorrect", LINE_INFO());
			Assert::IsTrue(pkb.isParentT(1, 5), L"Incorrect", LINE_INFO());
			Assert::IsTrue(pkb.isCalls("me", "other"), L"Incorrect", LINE_INFO());
		}

		TEST_METHOD(TestNoNestSource) {
			Parser parser;
			PKB pkb;

			unordered_set<string> vars;
			unordered_set<int> consts;

			vars = pkb.getAllVariables();
			consts = pkb.getAllConstant();
			Assert::AreEqual(0, (int)vars.size(), L"incorrect", LINE_INFO());
			Assert::AreEqual(0, (int)consts.size(), L"incorrect", LINE_INFO());

			string testInput = "..\\Tests\\Iteration1\\no_nest\\no_nest_source.txt";
			parser.parse(testInput, pkb);
			string correctVars[] = { "x", "xx", "x1", "xx1", "y", "z", "www" };
			int correctConsts[] = { 0, 1, 10, 100 };

			vars = pkb.getAllVariables();
			consts = pkb.getAllConstant();
			Assert::AreEqual((int) sizeof(correctVars) / sizeof(correctVars[0]), (unsigned int) vars.size(), L"incorrect", LINE_INFO());
			Assert::AreEqual((int) sizeof(correctConsts) / sizeof(correctConsts[0]), (unsigned int) consts.size(), L"incorrect", LINE_INFO());

			for (string it : correctVars) {
				Assert::IsTrue(vars.find(it) != vars.end(), L"incorrect", LINE_INFO());
			}
			for (int it : correctConsts) {
				Assert::IsTrue(consts.find(it) != consts.end(), L"incorrect", LINE_INFO());
			}
		}

		TEST_METHOD(TestSingleNestSource) {
			Parser parser;
			PKB pkb;

			unordered_set<string> vars;
			unordered_set<int> consts;
			unordered_set<int> ifs;
			unordered_set<int> whiles;

			vars = pkb.getAllVariables();
			consts = pkb.getAllConstant();
			ifs = pkb.getIfStmts();
			whiles = pkb.getWhileStmts();
			Assert::AreEqual(0, (int)vars.size(), L"incorrect", LINE_INFO());
			Assert::AreEqual(0, (int)consts.size(), L"incorrect", LINE_INFO());
			Assert::AreEqual(0, (int)ifs.size(), L"incorrect", LINE_INFO());
			Assert::AreEqual(0, (int)whiles.size(), L"incorrect", LINE_INFO());

			string testInput = "..\\Tests\\Iteration1\\single_nest\\single_nest_source.txt";
			parser.parse(testInput, pkb);

			string correctVars[] = { "x", "z", "i", "w", "p", "y" };
			int correctConsts[] = { 2, 3, 5, 1, 0 };
			int correctIfStmts[] = { 4 };
			int correctWhileStmts[] = { 8 };

			vars = pkb.getAllVariables();
			consts = pkb.getAllConstant();
			ifs = pkb.getIfStmts();
			whiles = pkb.getWhileStmts();
			Assert::AreEqual((int) sizeof(correctVars) / sizeof(correctVars[0]), (unsigned int)vars.size(), L"incorrect", LINE_INFO());
			Assert::AreEqual((int) sizeof(correctConsts) / sizeof(correctConsts[0]), (unsigned int)consts.size(), L"incorrect", LINE_INFO());
			Assert::AreEqual((int) sizeof(correctIfStmts) / sizeof(correctIfStmts[0]), (unsigned int)ifs.size(), L"incorrect", LINE_INFO());
			Assert::AreEqual((int) sizeof(correctWhileStmts) / sizeof(correctWhileStmts[0]), (unsigned int)whiles.size(), L"incorrect", LINE_INFO());

			for (string it : correctVars) {
				Assert::IsTrue(vars.find(it) != vars.end(), L"incorrect", LINE_INFO());
			}
			for (int it : correctConsts) {
				Assert::IsTrue(consts.find(it) != consts.end(), L"incorrect", LINE_INFO());
			}
			for (int it : correctIfStmts) {
				Assert::IsTrue(ifs.find(it) != ifs.end(), L"incorrect", LINE_INFO());
			}
			for (int it : correctWhileStmts) {
				Assert::IsTrue(whiles.find(it) != whiles.end(), L"incorrect", LINE_INFO());
			}
		}

		TEST_METHOD(TestDoubleNestSource) {
			Parser parser;
			PKB pkb;

			unordered_set<string> vars;
			unordered_set<int> consts;
			unordered_set<int> ifs;
			unordered_set<int> whiles;

			vars = pkb.getAllVariables();
			consts = pkb.getAllConstant();
			ifs = pkb.getIfStmts();
			whiles = pkb.getWhileStmts();
			Assert::AreEqual(0, (int)vars.size(), L"incorrect", LINE_INFO());
			Assert::AreEqual(0, (int)consts.size(), L"incorrect", LINE_INFO());
			Assert::AreEqual(0, (int)ifs.size(), L"incorrect", LINE_INFO());
			Assert::AreEqual(0, (int)whiles.size(), L"incorrect", LINE_INFO());

			string testInput = "..\\Tests\\Iteration1\\double_nest\\double_nest_source.txt";
			parser.parse(testInput, pkb);

			string correctVars[] = { "a", "b", "c", "x", "k" };
			int correctConsts[] = { 1, 7, 15, 16 };
			int correctIfStmts[] = { 2, 10 };
			int correctWhileStmts[] = { 3, 6, 9 };

			vars = pkb.getAllVariables();
			consts = pkb.getAllConstant();
			ifs = pkb.getIfStmts();
			whiles = pkb.getWhileStmts();
			Assert::AreEqual((int) sizeof(correctVars) / sizeof(correctVars[0]), (unsigned int)vars.size(), L"incorrect", LINE_INFO());
			Assert::AreEqual((int) sizeof(correctConsts) / sizeof(correctConsts[0]), (unsigned int)consts.size(), L"incorrect", LINE_INFO());
			Assert::AreEqual((int) sizeof(correctIfStmts) / sizeof(correctIfStmts[0]), (unsigned int)ifs.size(), L"incorrect", LINE_INFO());
			Assert::AreEqual((int) sizeof(correctWhileStmts) / sizeof(correctWhileStmts[0]), (unsigned int)whiles.size(), L"incorrect", LINE_INFO());

			for (string it : correctVars) {
				Assert::IsTrue(vars.find(it) != vars.end(), L"incorrect", LINE_INFO());
			}
			for (int it : correctConsts) {
				Assert::IsTrue(consts.find(it) != consts.end(), L"incorrect", LINE_INFO());
			}
			for (int it : correctIfStmts) {
				Assert::IsTrue(ifs.find(it) != ifs.end(), L"incorrect", LINE_INFO());
			}
			for (int it : correctWhileStmts) {
				Assert::IsTrue(whiles.find(it) != whiles.end(), L"incorrect", LINE_INFO());
			}
		}
	};
}