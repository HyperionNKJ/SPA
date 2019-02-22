#include "stdafx.h"
#include "CppUnitTest.h"
#include "Parser.h"
#include "PKB.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TestParser
{
	TEST_CLASS(TestParser)
	{
		static PKB pkb;
		static Parser parser;

	public:

		TEST_METHOD(TestCheckProcedure)
		{
			parser.pkb = pkb;
			bool result;

			result = parser.checkProcedure("procedure abcd {");
			Assert::AreEqual(result, true, L"incorrect", LINE_INFO());

			result = parser.checkProcedure("procedure abcd{");
			Assert::AreEqual(result, true, L"incorrect", LINE_INFO());

			result = parser.checkProcedure("procedure a {");
			Assert::AreEqual(result, true, L"incorrect", LINE_INFO());

			//test spaces
			result = parser.checkProcedure("	procedure	abcd		 {");
			Assert::AreEqual(result, true, L"incorrect", LINE_INFO());

			//unaccepted proc name
			result = parser.checkProcedure("procedure 123 {");
			Assert::AreEqual(result, false, L"incorrect", LINE_INFO());

			//no proc name
			result = parser.checkProcedure("procedure {");
			Assert::AreEqual(result, true, L"incorrect", LINE_INFO());
		}

	};
}