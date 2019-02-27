#include "stdafx.h"
#include "CppUnitTest.h"
#include "Parser.h"
#include "PKB.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace IntegrationTesting
{		
	TEST_CLASS(TestParserToPKB)
	{
	public:
		
		TEST_METHOD(TestInsertProcedure)
		{
			Parser parser;
			PKB pkb;
			parser.setPKB(&pkb);
			unordered_set<string> procs;

			procs = pkb.getAllProcedures();
			Assert::AreEqual((int)procs.size(), 0);

			string procName = "myProcOne";
			parser.handleProcedure("procedure myProcOne {");

			procs = pkb.getAllProcedures();
			Assert::AreEqual((int) procs.size(), 1);

			string wrongName = "hahahaha";
			for (string it : procs) {
				Assert::AreEqual(it, procName);
				Assert::AreNotEqual(it, wrongName);
			}
		}
	};
}