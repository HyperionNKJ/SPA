#include "stdafx.h"
#include "CppUnitTest.h"
#include <vector>
#include <iostream>
#include "../SPA/QueryTokenizer.h"
#include "../SPA/Statement.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
	TEST_CLASS(TestQueryTokenizer)
	{
	public:

		TEST_METHOD(TestMethod1)
		{
			QueryTokenizer qt("assign a;");
			bool status = qt.tokenize();

			std::string a = "assign a";
			std::string z = "assign a";
			Assert::AreEqual(status,true);
		}

	};
}