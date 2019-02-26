#include "stdafx.h"
#include "CppUnitTest.h"
#include "PKB.h"
#include "../SPA/Type.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
	TEST_CLASS(TestPKB)
	{
	public:

		TEST_METHOD(TestInsertVar)
		{
			PKB pkb;
			bool result;

			result = pkb.insertVar("var");
			Assert::IsTrue(result);

			result = pkb.insertVar("var");
			Assert::IsFalse(result);
		}

		TEST_METHOD(TestInsertConstant)
		{
			PKB pkb;
			bool result;

			result = pkb.insertConstant(6);
			Assert::IsTrue(result);

			result = pkb.insertConstant(6);
			Assert::IsFalse(result);
		}

		TEST_METHOD(TestInsertProc)
		{
			PKB pkb;
			bool result;

			result = pkb.insertProc("main");
			Assert::IsTrue(result);

			result = pkb.insertProc("main");
			Assert::IsFalse(result);
		}

		TEST_METHOD(TestInsertStmtType)
		{
			PKB pkb;
			bool result;

			result = pkb.insertStmtType(10, STATEMENT);
			Assert::IsTrue(result);

			result = pkb.insertStmtType(10, STATEMENT);
			Assert::IsFalse(result);
		}

		TEST_METHOD(TestSetFollows)
		{
			PKB pkb;
			bool result;

			result = pkb.setFollows(4, 7);
			Assert::IsTrue(result);

			result = pkb.setFollows(4, 7);
			Assert::IsFalse(result);
		}
		
		TEST_METHOD(TestSetFollowsT)
		{
			PKB pkb;
			bool result;

			result = pkb.setFollowsT(4, 7);
			Assert::IsTrue(result);

			result = pkb.setFollowsT(4, 7);
			Assert::IsFalse(result);
		}
		
		TEST_METHOD(TestSetFollowedBy)
		{
			PKB pkb;
			bool result;

			result = pkb.setFollowedBy(4, 7);
			Assert::IsTrue(result);

			result = pkb.setFollowedBy(4, 7);
			Assert::IsFalse(result);
		}

		TEST_METHOD(TestSetFollowedByT)
		{
			PKB pkb;
			bool result;

			result = pkb.setFollowedByT(4, 7);
			Assert::IsTrue(result);

			result = pkb.setFollowedByT(4, 7);
			Assert::IsFalse(result);
		}

		TEST_METHOD(TestSetParent)
		{
			PKB pkb;
			bool result;

			result = pkb.setParent(4, 7);
			Assert::IsTrue(result);

			result = pkb.setParent(4, 7);
			Assert::IsFalse(result);
		}

		TEST_METHOD(TestSetParentT)
		{
			PKB pkb;
			bool result;

			result = pkb.setParentT(4, 7);
			Assert::IsTrue(result);

			result = pkb.setParentT(4, 7);
			Assert::IsFalse(result);
		}

		TEST_METHOD(TestSetChild)
		{
			PKB pkb;
			bool result;

			result = pkb.setChild(4, 7);
			Assert::IsTrue(result);

			result = pkb.setChild(4, 7);
			Assert::IsFalse(result);
		}

		TEST_METHOD(TestSetChildT)
		{
			PKB pkb;
			bool result;

			result = pkb.setChildT(4, 7);
			Assert::IsTrue(result);

			result = pkb.setChildT(4, 7);
			Assert::IsFalse(result);
		}

		TEST_METHOD(TestSetModifiesByStmtNum)
		{
			PKB pkb;
			bool result;

			result = pkb.setModifies(4, "var");
			Assert::IsTrue(result);

			result = pkb.setModifies(4, "var");
			Assert::IsFalse(result);
		}

		TEST_METHOD(TestSetModifiesByProcName)
		{
			PKB pkb;
			bool result;

			result = pkb.setModifies("main", "var");
			Assert::IsTrue(result);

			result = pkb.setModifies("main", "var");
			Assert::IsFalse(result);
		}

		TEST_METHOD(TestSetUsesByStmtNum)
		{
			PKB pkb;
			bool result;

			result = pkb.setUses(4, "var");
			Assert::IsTrue(result);

			result = pkb.setUses(4, "var");
			Assert::IsFalse(result);
		}

		TEST_METHOD(TestSetUsesByProcName)
		{
			PKB pkb;
			bool result;

			result = pkb.setUses("main", "var");
			Assert::IsTrue(result);

			result = pkb.setUses("main", "var");
			Assert::IsFalse(result);
		}

		TEST_METHOD(TestSetAssignStmt)
		{
			PKB pkb;
			bool result;
			vector<string> assignmentStmt = { "y", "+", "z" };

			result = pkb.insertAssignStmt(4, "x", assignmentStmt);
			Assert::IsTrue(result);

			result = pkb.insertAssignStmt(4, "x", assignmentStmt);
			Assert::IsFalse(result);
		}

	};
}