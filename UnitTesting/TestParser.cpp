#include "stdafx.h"
#include "CppUnitTest.h"
#include "Parser.h"
#include "PKB.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TestParser
{
	TEST_CLASS(TestParser)
	{
		PKB *pkb = new PKB();
		Parser parser;

	public:

		TEST_METHOD(TestCheckProcedure)
		{
			parser.setPKB(pkb);
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

		TEST_METHOD(TestCheckFactor) {
			parser.setPKB(pkb);
			bool result;
			
			result = parser.checkFactor("Avar");
			Assert::AreEqual(result, true, L"incorrect", LINE_INFO());

			result = parser.checkFactor("1234");
			Assert::AreEqual(result, true, L"incorrect", LINE_INFO());

			result = parser.checkFactor("(a)");
			Assert::AreEqual(result, true, L"incorrect", LINE_INFO());

			result = parser.checkFactor("aab**");
			Assert::AreEqual(result, false, L"incorrect", LINE_INFO());
		}

		TEST_METHOD(TestCheckTerm) {
			parser.setPKB(pkb);
			bool result;

			result = parser.checkFactor("c*d");
			Assert::AreEqual(result, true, L"incorrect", LINE_INFO());

			result = parser.checkFactor("c*d*e");
			Assert::AreEqual(result, true, L"incorrect", LINE_INFO());

			result = parser.checkFactor("Avar");
			Assert::AreEqual(result, true, L"incorrect", LINE_INFO());

			result = parser.checkFactor("a+b");
			Assert::AreEqual(result, false, L"incorrect", LINE_INFO());
		}

		TEST_METHOD(TestCheckExpr) {
			parser.setPKB(pkb);
			bool result;

			result = parser.checkExpr("a+b*c*(d+e)");
			Assert::AreEqual(result, true, L"incorrect", LINE_INFO());

			result = parser.checkExpr("c*d*e");
			Assert::AreEqual(result, true, L"incorrect", LINE_INFO());

			result = parser.checkExpr("(a+b)*c%e*(10+15)");
			Assert::AreEqual(result, true, L"incorrect", LINE_INFO());

			result = parser.checkExpr("8*(100+201)%5");
			Assert::AreEqual(result, true, L"incorrect", LINE_INFO());
		}

		TEST_METHOD(TestCheckAssignment) {
			parser.setPKB(pkb);
			bool result;

			result = parser.checkAssignment("myvar= X + Y	-Z + 11;");
			Assert::AreEqual(result, true, L"incorrect", LINE_INFO());

			result = parser.checkAssignment("x = x + y + z * (1 + 2 + 3 + x + y + z);");
			Assert::AreEqual(result, true, L"incorrect", LINE_INFO());

			result = parser.checkAssignment("x=x*( y+1/( z+2%( a+3 ) ) );");
			Assert::AreEqual(result, true, L"incorrect", LINE_INFO());

			result = parser.checkAssignment("123 = a+b+c+d;");
			Assert::AreEqual(result, false, L"incorrect", LINE_INFO());

			result = parser.checkAssignment("= a+b+c+d;");
			Assert::AreEqual(result, false, L"incorrect", LINE_INFO());

			result = parser.checkAssignment("x = ;");
			Assert::AreEqual(result, false, L"incorrect", LINE_INFO());

			result = parser.checkAssignment("x= (a+b+c+d));");
			Assert::AreEqual(result, false, L"incorrect", LINE_INFO());

			result = parser.checkAssignment("x= )a+b+c+d;");
			Assert::AreEqual(result, false, L"incorrect", LINE_INFO());
		}

		TEST_METHOD(TestCheckRelFactor) {
			parser.setPKB(pkb);
			bool result;

			result = parser.checkRelFactor("somevarname");
			Assert::AreEqual(result, true, L"incorrect", LINE_INFO());

			result = parser.checkTerm("531");
			Assert::AreEqual(result, true, L"incorrect", LINE_INFO());

			result = parser.checkTerm("a+b+c*d");
			Assert::AreEqual(result, true, L"incorrect", LINE_INFO());

			result = parser.checkTerm("123notavar");
			Assert::AreEqual(result, false, L"incorrect", LINE_INFO());
		}
		TEST_METHOD(TestCheckCondExpr) {
			parser.setPKB(pkb);
			bool result;

			result = parser.checkCondExpr("first>second");
			Assert::AreEqual(result, true, L"incorrect", LINE_INFO());

			result = parser.checkCondExpr("first>=second");
			Assert::AreEqual(result, true, L"incorrect", LINE_INFO());

			result = parser.checkCondExpr("first<=second");
			Assert::AreEqual(result, true, L"incorrect", LINE_INFO());

			result = parser.checkCondExpr("first<second");
			Assert::AreEqual(result, true, L"incorrect", LINE_INFO());

			result = parser.checkCondExpr("first==second");
			Assert::AreEqual(result, true, L"incorrect", LINE_INFO());

			result = parser.checkCondExpr("first!=second");
			Assert::AreEqual(result, true, L"incorrect", LINE_INFO());

			result = parser.checkCondExpr("!(first>second)");
			Assert::AreEqual(result, true, L"incorrect", LINE_INFO());

			result = parser.checkCondExpr("(first>second) && (third!=fourth)");
			Assert::AreEqual(result, true, L"incorrect", LINE_INFO());

			result = parser.checkCondExpr("(first>second) || (!(third!=fourth))");
			Assert::AreEqual(result, true, L"incorrect", LINE_INFO());

			result = parser.checkCondExpr("123 < 456");
			Assert::AreEqual(result, true, L"incorrect", LINE_INFO());

			result = parser.checkCondExpr("(1+2==3) && (2=5)");
			Assert::AreEqual(result, true, L"incorrect", LINE_INFO());

			result = parser.checkCondExpr("first == second && first > 2");
			Assert::AreEqual(result, false, L"incorrect", LINE_INFO());

			result = parser.checkCondExpr("first == second || first > 2");
			Assert::AreEqual(result, false, L"incorrect", LINE_INFO());

			result = parser.checkCondExpr("(first == second) && first > 2");
			Assert::AreEqual(result, false, L"incorrect", LINE_INFO());
		}

		TEST_METHOD(TestCheckWhileProcedure) {
			parser.setPKB(pkb);
			bool result;
			result = parser.checkWhile("while () {");
			Assert::AreEqual(result, false, L"incorrect", LINE_INFO());
		}

		TEST_METHOD(TestCheckPrint) {
			parser.setPKB(pkb);
			bool result;

			result = parser.checkPrint("print myvar;");
			Assert::AreEqual(result, true, L"incorrect", LINE_INFO());

			result = parser.checkPrint("	print	 manyspaces;");
			Assert::AreEqual(result, true, L"incorrect", LINE_INFO());

			result = parser.checkPrint("print 123;");
			Assert::AreEqual(result, false, L"incorrect", LINE_INFO());

			result = parser.checkPrint("print avar anothervar;");
			Assert::AreEqual(result, false, L"incorrect", LINE_INFO());
		}

		TEST_METHOD(TestCheckRead) {
			parser.setPKB(pkb);
			bool result;

			result = parser.checkRead("read myvar;");
			Assert::AreEqual(result, true, L"incorrect", LINE_INFO());

			result = parser.checkRead("	read	 manyspaces;");
			Assert::AreEqual(result, true, L"incorrect", LINE_INFO());

			result = parser.checkRead("read 123;");
			Assert::AreEqual(result, false, L"incorrect", LINE_INFO());

			result = parser.checkRead("read avar anothervar;");
			Assert::AreEqual(result, false, L"incorrect", LINE_INFO());
		}

		TEST_METHOD(TestCheckRead) {
			parser.setPKB(pkb);
			bool result;

			result = parser.checkRead("read myvar;");
			Assert::AreEqual(result, true, L"incorrect", LINE_INFO());

			result = parser.checkRead("	read	 manyspaces;");
			Assert::AreEqual(result, true, L"incorrect", LINE_INFO());

			result = parser.checkRead("read 123;");
			Assert::AreEqual(result, false, L"incorrect", LINE_INFO());

			result = parser.checkRead("read avar anothervar;");
			Assert::AreEqual(result, false, L"incorrect", LINE_INFO());
		}

	};
}