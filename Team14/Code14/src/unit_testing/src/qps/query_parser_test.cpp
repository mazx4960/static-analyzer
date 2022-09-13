#include <gtest/gtest.h>

#include <algorithm>

#include "qps/pql/query_call.h"
#include "qps/query_parser.h"

TEST(QueryParserTest, ValidSingleParentClauseTest) {
  std::vector<Token *> single_clause_parent_query = {
      new KeywordToken("assign"), new SymbolToken("v1"),   new SemicolonToken(),       new KeywordToken("assign"),
      new SymbolToken("v2"),      new SemicolonToken(),    new KeywordToken("Select"), new SymbolToken("v1"),
      new SymbolToken("such"),    new SymbolToken("that"), new KeywordToken("Parent"), new RoundOpenBracketToken(),
      new SymbolToken("v1"),      new CommaToken(),        new SymbolToken("v2"),      new RoundCloseBracketToken(),
      new EndOfFileToken()};
  QueryParser parser = QueryParser(single_clause_parent_query);
  parser.parse();

  QueryDeclaration* v1 = new AssignDeclaration(QuerySynonym("v1"));
  QueryDeclaration* v2 = new AssignDeclaration(QuerySynonym("v2"));
  Query expected_query = Query(std::vector<QueryDeclaration*> {v1, v2},
                               SelectCall(v1, std::vector<QueryClause*> {new ParentClause(v1, v2)}));

  // check declarations
  ASSERT_EQ(parser.getDeclarations()[0]->getSynonym(), v1->getSynonym());
  ASSERT_EQ(parser.getDeclarations()[1]->getSynonym(), v2->getSynonym());

  // check call
  ASSERT_EQ(parser.getQueryCalls()[0]->getType(), expected_query.getQueryCall().getType());

  // check clause
  ASSERT_EQ(parser.getQueryCalls()[0]->getClauseVector()[0]->getClauseType(),
            expected_query.getQueryCall().getClauseVector()[0]->getClauseType());
  ASSERT_EQ(static_cast<ParentClause*>(parser.getQueryCalls()[0]->getClauseVector()[0])->getFirst()->getSynonym().toString(),
            static_cast<ParentClause*>(expected_query.getQueryCall().getClauseVector()[0])->getFirst()->getSynonym().toString());
  ASSERT_EQ(static_cast<ParentClause*>(parser.getQueryCalls()[0]->getClauseVector()[0])->getSecond()->getSynonym().toString(),
            static_cast<ParentClause*>(expected_query.getQueryCall().getClauseVector()[0])->getSecond()->getSynonym().toString());
}

TEST(QueryParserTest, ValidSingleFollowsClauseTest) {
  std::vector<Token *> single_clause_parent_query = {
      new KeywordToken("assign"), new SymbolToken("v1"),   new SemicolonToken(),       new KeywordToken("assign"),
      new SymbolToken("v2"),      new SemicolonToken(),    new KeywordToken("Select"), new SymbolToken("v1"),
      new SymbolToken("such"),    new SymbolToken("that"), new KeywordToken("Follows"), new RoundOpenBracketToken(),
      new SymbolToken("v1"),      new CommaToken(),        new SymbolToken("v2"),      new RoundCloseBracketToken(),
      new EndOfFileToken()};
  QueryParser parser = QueryParser(single_clause_parent_query);
  parser.parse();

  QueryDeclaration* v1 = new AssignDeclaration(QuerySynonym("v1"));
  QueryDeclaration* v2 = new AssignDeclaration(QuerySynonym("v2"));
  Query expected_query = Query(std::vector<QueryDeclaration*> {v1, v2},
                               SelectCall(v1, std::vector<QueryClause*> {new FollowsClause(v1, v2)}));

  // check declarations
  ASSERT_EQ(parser.getDeclarations()[0]->getSynonym(), v1->getSynonym());
  ASSERT_EQ(parser.getDeclarations()[1]->getSynonym(), v2->getSynonym());

  // check call
  ASSERT_EQ(parser.getQueryCalls()[0]->getType(), expected_query.getQueryCall().getType());

  // check clause
  ASSERT_EQ(parser.getQueryCalls()[0]->getClauseVector()[0]->getClauseType(),
            expected_query.getQueryCall().getClauseVector()[0]->getClauseType());
  ASSERT_EQ(static_cast<FollowsClause*>(parser.getQueryCalls()[0]->getClauseVector()[0])->getFirst()->getSynonym().toString(),
            static_cast<FollowsClause*>(expected_query.getQueryCall().getClauseVector()[0])->getFirst()->getSynonym().toString());
  ASSERT_EQ(static_cast<FollowsClause*>(parser.getQueryCalls()[0]->getClauseVector()[0])->getSecond()->getSynonym().toString(),
            static_cast<FollowsClause*>(expected_query.getQueryCall().getClauseVector()[0])->getSecond()->getSynonym().toString());
}

TEST(QueryParserTest, ValidSinglePatternClauseTest) {
  std::vector<Token *> single_clause_parent_query = {
      new KeywordToken("assign"), new SymbolToken("a"),         new SemicolonToken(),   new KeywordToken("Select"),
      new SymbolToken("a"),       new KeywordToken("pattern"),  new SymbolToken("a"),   new RoundOpenBracketToken(),
      new QuoteToken(),           new SymbolToken("x"),         new QuoteToken(),       new CommaToken(),
      new QuoteToken(),           new SymbolToken("x"),         new OperatorToken("+"), new SymbolToken("y"),
      new QuoteToken(),           new RoundCloseBracketToken(), new EndOfFileToken()};
  QueryParser parser = QueryParser(single_clause_parent_query);
  parser.parse();

  QueryDeclaration* a = new AssignDeclaration(QuerySynonym("a"));

  Query expected_query =
      Query(std::vector<QueryDeclaration*> {a},
            SelectCall(a, std::vector<QueryClause*> {new AssignPatternClause(a,
                                                                             new StringDeclaration("x"),
                                                                             new ExpressionDeclaration("x+y"))}));

  // check declarations
  ASSERT_EQ(parser.getDeclarations()[0]->getSynonym(), a->getSynonym());

  // check call
  ASSERT_EQ(parser.getQueryCalls()[0]->getType(), expected_query.getQueryCall().getType());

  // check clause
  ASSERT_EQ(parser.getQueryCalls()[0]->getClauseVector()[0]->getClauseType(),
            expected_query.getQueryCall().getClauseVector()[0]->getClauseType());
  ASSERT_EQ(static_cast<AssignPatternClause*>(parser.getQueryCalls()[0]->getClauseVector()[0])->getFirst()->getSynonym().toString(),
            static_cast<AssignPatternClause*>(expected_query.getQueryCall().getClauseVector()[0])->getFirst()->getSynonym().toString());
  ASSERT_EQ(static_cast<AssignPatternClause*>(parser.getQueryCalls()[0]->getClauseVector()[0])->getSecond()->getSynonym().toString(),
            static_cast<AssignPatternClause*>(expected_query.getQueryCall().getClauseVector()[0])->getSecond()->getSynonym().toString());
  ASSERT_EQ(static_cast<AssignPatternClause*>(parser.getQueryCalls()[0]->getClauseVector()[0])->getThird()->getSynonym().toString(),
            static_cast<AssignPatternClause*>(expected_query.getQueryCall().getClauseVector()[0])->getThird()->getSynonym().toString());
}


TEST(QueryParserTest, InvalidTokenTest1) {
  std::vector<Token *> invalid_tokens = {new KeywordToken("var"),    new SymbolToken("v1"), new SemicolonToken(),
                                         new KeywordToken("assign"), new SymbolToken("v2"), new SemicolonToken(),
                                         new KeywordToken("Select"), new SymbolToken("v1"), new EndOfFileToken()};
  QueryParser validator = QueryParser(invalid_tokens);
  try {
    validator.parse();
    FAIL();
  } catch (const ParseSyntaxError &expected) {}
}

TEST(QueryParserTest, InvalidTokenTest2) {
  std::vector<Token *> invalid_tokens = {new KeywordToken("assign"), new SymbolToken("v1"), new SemicolonToken(),
                                         new KeywordToken("assign"), new SymbolToken("v1"), new SemicolonToken(),
                                         new KeywordToken("Select"), new SymbolToken("v1"), new EndOfFileToken()};
  QueryParser validator = QueryParser(invalid_tokens);
  try {
    validator.parse();
    FAIL();
  } catch (const ParseSemanticError &expected) {}
}
