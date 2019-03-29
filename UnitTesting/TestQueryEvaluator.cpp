#include "stdafx.h"
#include <string>
#include "CppUnitTest.h"
#include "QueryEvaluator.h"
#include "Calls.h"
#include "CallsT.h"
#include "Follows.h"
#include "FollowsT.h"
#include "ModifiesP.h"
#include "ModifiesS.h"
#include "Next.h"
#include "NextT.h"
#include "Parent.h"
#include "ParentT.h"
#include "PatternAssign.h"
#include "PatternIf.h"
#include "PatternWhile.h"
#include "UsesP.h"
#include "UsesS.h"
#include "With.h"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
	TEST_CLASS(TestQueryEvaluator) {
		public:
			TEST_METHOD(groupBasedOnConnectedSyn) {
				vector<Clause*> unconnectedClauses{
				new ModifiesS(DesignEntity("c", Type::CALL), DesignEntity("x", Type::VARIABLE)),
				new ModifiesP(DesignEntity("x", Type::PROCEDURE), DesignEntity("y", Type::VARIABLE)),
				new Parent(DesignEntity("w", Type::WHILE), DesignEntity("p", Type::CALL)),
				new ParentT(DesignEntity("p", Type::IF), DesignEntity("b", Type::PRINT)),
				new Calls(DesignEntity("p", Type::PROCEDURE), DesignEntity("a", Type::PROCEDURE)),
				new CallsT(DesignEntity("w", Type::PROCEDURE), DesignEntity("x", Type::PROCEDURE)),
				new Next(DesignEntity("s1", Type::PROGLINE), DesignEntity("s2", Type::READ)),
				new NextT(DesignEntity("d", Type::ASSIGN), DesignEntity("e", Type::WHILE)),
				new Follows(DesignEntity("f", Type::STATEMENT), DesignEntity("g", Type::READ)),
				new FollowsT(DesignEntity("s1", Type::WHILE), DesignEntity("q", Type::IF)),
				new UsesS(DesignEntity("s1", Type::PRINT), DesignEntity("a", Type::WHILE)) };
				
				vector<pair<unordered_set<std::string>, vector<Clause*>>> connectedClauses;
				QueryEvaluator evaluator;
				evaluator.groupBasedOnConnectedSyn(unconnectedClauses, connectedClauses);
				string s;
				for (auto& synClausePair : connectedClauses) {
					s.append("\n");
					s.append("Synonyms: ");
					for (auto& syn : synClausePair.first) {
						s.append(syn + " ");
					}
					s.append("\n");
					s.append("Clauses: ");
					for (auto& clause : synClausePair.second) {
						s.append(" ");
						for (auto& arg : clause->getSynonyms()) {
							s.append(arg);
						}
					}
				}
				const char *cstr = s.c_str();
				Logger::WriteMessage(cstr);
			}
	};
}