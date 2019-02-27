#include "stdafx.h"
#include "CppUnitTest.h"
#include "Parser.h"
#include "PKB.h"

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

			string wrongName = "All your base are belong to us.";
			for (string it : procs) {
				Assert::AreEqual(procName, it, L"incorrect", LINE_INFO());
				Assert::AreNotEqual(wrongName, it, L"incorrect", LINE_INFO());
			}
		}

		TEST_METHOD(TestInsertAssignment) {
			Parser parser;
			PKB pkb;
			parser.setPKB(&pkb);
			unordered_set<string> vars;
			unordered_set<int> consts;

			vars = pkb.getAllVariables();
			consts = pkb.getAllConstant();
			Assert::AreEqual((int) vars.size(), 0, L"incorrect", LINE_INFO());
			Assert::AreEqual((int) consts.size(), 0, L"incorrect", LINE_INFO());

			string correctVars[] = {"a", "b", "c", "d"};
			int correctConsts[] = {1, 10};
			parser.handleAssignment("a		=	b + c*d - 1 * 10");

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

		TEST_METHOD(TestNoNestSource) {
			Parser parser;
			PKB pkb;

			unordered_set<string> vars;
			unordered_set<int> consts;

			vars = pkb.getAllVariables();
			consts = pkb.getAllConstant();
			Assert::AreEqual(0, (int)vars.size(), L"incorrect", LINE_INFO());
			Assert::AreEqual(0, (int)consts.size(), L"incorrect", LINE_INFO());

			string testInput = "..\\Tests\\Iteration1Tests\\no_nest\\no_nest_source.txt";
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

			string testInput = "..\\Tests\\Iteration1Tests\\single_nest\\single_nest_source.txt";
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

			string testInput = "..\\Tests\\Iteration1Tests\\double_nest\\double_nest_source.txt";
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