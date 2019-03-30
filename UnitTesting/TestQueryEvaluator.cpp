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
			Clause* getOneSynClause(string syn1, string syn2) {
				return new ModifiesS(DesignEntity(syn1, Type::ASSIGN), DesignEntity(syn2, Type::FIXED));
			}
			Clause* getTwoSynClause(string syn1, string syn2) {
				return new ModifiesS(DesignEntity(syn1, Type::ASSIGN), DesignEntity(syn2, Type::STATEMENT));
			}
			string getStringRepresentation(vector<pair<unordered_set<std::string>, vector<Clause*>>>& connectedClauses) {
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
						for (auto& arg : clause->getSynonyms()) {
							s.append(arg);
						}
						s.append(" ");
					}
				}
				// uncomment below to print directly to console
				/*const char *cstr = s.c_str();
				Logger::WriteMessage(cstr);*/
				return s;
			}
			TEST_METHOD(groupBasedOnConnectedSyn) {
				QueryEvaluator evaluator;
				vector<pair<unordered_set<std::string>, vector<Clause*>>> connectedClauses; // initially empty

				vector<Clause*> unconnectedClauses{
					getOneSynClause("a","2"),
					getTwoSynClause("d","b"),
					getTwoSynClause("p","q"),
					getOneSynClause("c","3"),
					getTwoSynClause("c","e"),
					getTwoSynClause("e","a"),
					getOneSynClause("b","3"),
					getTwoSynClause("f","d"),
					getTwoSynClause("r","t"),
					getTwoSynClause("r","q"),
				}; 
				
				evaluator.groupBasedOnConnectedSyn(unconnectedClauses, connectedClauses); // connectedClause modified by reference
				string actualGrouping = getStringRepresentation(connectedClauses);
				string expectedGrouping = "\nSynonyms: a e c \nClauses: a ea c ce \nSynonyms: d b f \nClauses: db b fd \nSynonyms: p q r t \nClauses: pq rq rt ";
				Assert::AreEqual(expectedGrouping, actualGrouping);

				connectedClauses.clear();
				unconnectedClauses.clear();

				unconnectedClauses = {
					getTwoSynClause("a","b"),
					getTwoSynClause("c","d"),
					getTwoSynClause("e","f"),
					getTwoSynClause("g","h"),
					getTwoSynClause("e","g"),
					getTwoSynClause("t","w"),
					getTwoSynClause("x","c"),
					getTwoSynClause("p","q"),
					getTwoSynClause("w","b"),
					getTwoSynClause("fall","out"),
					getTwoSynClause("out","age"),
					getTwoSynClause("the","one"),
					getTwoSynClause("the","fall"),
					getTwoSynClause("get","f"),
					getTwoSynClause("h","i"),
					getTwoSynClause("i","w")
				};

				evaluator.groupBasedOnConnectedSyn(unconnectedClauses, connectedClauses); // connectedClause modified by reference
				actualGrouping = getStringRepresentation(connectedClauses);
				expectedGrouping = "\nSynonyms: i a b g w t e f h get \nClauses: ab wb tw iw ef eg gh getf hi \nSynonyms: c d x \nClauses: cd xc \nSynonyms: p q \nClauses: pq \nSynonyms: fall one out the age \nClauses: fallout outage thefall theone ";
				Assert::AreEqual(expectedGrouping, actualGrouping);

				connectedClauses.clear();
				unconnectedClauses.clear();

				unconnectedClauses = {
					getTwoSynClause("a","b"),
					getTwoSynClause("c","d"),
					getTwoSynClause("e","f"),
					getTwoSynClause("g","h"),
					getTwoSynClause("i","j"),
				};

				evaluator.groupBasedOnConnectedSyn(unconnectedClauses, connectedClauses); // connectedClause modified by reference
				actualGrouping = getStringRepresentation(connectedClauses);
				expectedGrouping = "\nSynonyms: a b \nClauses: ab \nSynonyms: c d \nClauses: cd \nSynonyms: e f \nClauses: ef \nSynonyms: g h \nClauses: gh \nSynonyms: i j \nClauses: ij ";
				Assert::AreEqual(expectedGrouping, actualGrouping);
			}
	};
}