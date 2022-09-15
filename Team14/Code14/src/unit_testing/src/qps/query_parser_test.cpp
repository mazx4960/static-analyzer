#include <gtest/gtest.h>

#include <algorithm>

#include "qps/pql/query_call.h"
#include "qps/query_parser.h"


TEST(QueryParserTest, AssignDeclarationParseTest) {
  std::vector<Token *> assign_tokens = {
      new KeywordToken("assign"), new SymbolToken("a"), new SemicolonToken()
  };
  QueryParser parser = QueryParser(assign_tokens);
  QueryDeclaration *assign_declaration = parser.parseDeclaration();
  ASSERT_EQ(*assign_declaration, AssignDeclaration(new QuerySynonym("a")));
}

TEST(QueryParserTest, StmtDeclarationParseTest) {
  std::vector<Token *> stmt_tokens = {
      new KeywordToken("stmt"), new SymbolToken("s"), new SemicolonToken()
  };
  QueryParser parser = QueryParser(stmt_tokens);
  QueryDeclaration *stmt_declaration = parser.parseDeclaration();
  ASSERT_EQ(*stmt_declaration, StatementDeclaration(new QuerySynonym("s")));
}

TEST(QueryParserTest, VariableDeclarationParseTest) {
  std::vector<Token *> var_tokens = {
      new KeywordToken("variable"), new SymbolToken("v"), new SemicolonToken()
  };
  QueryParser parser = QueryParser(var_tokens);
  QueryDeclaration *var_declaration = parser.parseDeclaration();
  ASSERT_EQ(*var_declaration, VariableDeclaration(new QuerySynonym("v")));
}

TEST(QueryParserTest, ConstantDeclarationParseTest) {
  std::vector<Token *> const_tokens = {
      new KeywordToken("constant"), new SymbolToken("c"), new SemicolonToken()
  };
  QueryParser parser = QueryParser(const_tokens);
  QueryDeclaration *const_declaration = parser.parseDeclaration();
  ASSERT_EQ(*const_declaration, ConstantDeclaration(new QuerySynonym("c")));
}

TEST(QueryParserTest, ProcedureDeclarationParseTest) {
  std::vector<Token *> proc_tokens = {
      new KeywordToken("procedure"), new SymbolToken("p"), new SemicolonToken()
  };
  QueryParser parser = QueryParser(proc_tokens);
  QueryDeclaration *proc_declaration = parser.parseDeclaration();
  ASSERT_EQ(*proc_declaration, ProcedureDeclaration(new QuerySynonym("p")));
}

TEST(QueryParserTest, ReadDeclarationParseTest) {
  std::vector<Token *> read_tokens = {
      new KeywordToken("read"), new SymbolToken("r"), new SemicolonToken()
  };
  QueryParser parser = QueryParser(read_tokens);
  QueryDeclaration *read_declaration = parser.parseDeclaration();
  ASSERT_EQ(*read_declaration, ReadDeclaration(new QuerySynonym("r")));
}

TEST(QueryParserTest, PrintDeclarationParseTest) {
  std::vector<Token *> print_tokens = {
      new KeywordToken("print"), new SymbolToken("pr"), new SemicolonToken()
  };
  QueryParser parser = QueryParser(print_tokens);
  QueryDeclaration *print_declaration = parser.parseDeclaration();
  ASSERT_EQ(*print_declaration, PrintDeclaration(new QuerySynonym("pr")));
}

TEST(QueryParserTest, CallDeclarationParseTest) {
  std::vector<Token *> call_tokens = {
      new KeywordToken("call"), new SymbolToken("cl"), new SemicolonToken()
  };
  QueryParser parser = QueryParser(call_tokens);
  QueryDeclaration *call_declaration = parser.parseDeclaration();
  ASSERT_EQ(*call_declaration, CallDeclaration(new QuerySynonym("cl")));
}

TEST(QueryParserTest, WhileDeclarationParseTest) {
  std::vector<Token *> while_tokens = {
      new KeywordToken("while"), new SymbolToken("wh"), new SemicolonToken()
  };
  QueryParser parser = QueryParser(while_tokens);
  QueryDeclaration *while_declaration = parser.parseDeclaration();
  ASSERT_EQ(*while_declaration, WhileDeclaration(new QuerySynonym("wh")));
}

TEST(QueryParserTest, IfDeclarationParseTest) {
  std::vector<Token *> if_tokens = {
      new KeywordToken("if"), new SymbolToken("wh"), new SemicolonToken()
  };
  QueryParser parser = QueryParser(if_tokens);
  QueryDeclaration *if_declaration = parser.parseDeclaration();
  ASSERT_EQ(*if_declaration, IfDeclaration(new QuerySynonym("wh")));
}

TEST(QueryParserTest, IntegerStmtRefDeclarationParseTest) {
  std::vector<Token *> stmt_ref_tokens = {
      new LiteralToken("1")
  };
  QueryParser parser = QueryParser(stmt_ref_tokens);
  QueryDeclaration *stmt_ref_declaration = parser.parseStmtRefDeclaration(false);
  ASSERT_EQ(*stmt_ref_declaration, IntegerDeclaration("1"));
}

TEST(QueryParserTest, SymbolStmtRefDeclarationParseTest) {
  std::vector<Token *> stmt_ref_tokens = {
      new KeywordToken("variable"), new SymbolToken("v"), new SemicolonToken(), new SymbolToken("v")
  };
  QueryParser parser = QueryParser(stmt_ref_tokens);
  parser.parseDeclarations();
  QueryDeclaration *stmt_ref_declaration = parser.parseStmtRefDeclaration(false);
  ASSERT_EQ(*stmt_ref_declaration, VariableDeclaration(new QuerySynonym("v")));
}

TEST(QueryParserTest, StringEntRefDeclarationParseTest) {
  std::vector<Token *> ent_ref_tokens = {
      new QuoteToken(), new SymbolToken("abc"), new QuoteToken()
  };
  QueryParser parser = QueryParser(ent_ref_tokens);
  QueryDeclaration *ent_ref_declaration = parser.parseEntRefDeclaration(false);
  ASSERT_EQ(*ent_ref_declaration, StringDeclaration("abc"));
}

TEST(QueryParserTest, SymbolEntRefDeclarationParseTest) {
  std::vector<Token *> ent_ref_tokens = {
      new KeywordToken("variable"), new SymbolToken("v"), new SemicolonToken(), new SymbolToken("v")
  };
  QueryParser parser = QueryParser(ent_ref_tokens);
  parser.parseDeclarations();
  QueryDeclaration *ent_ref_declaration = parser.parseEntRefDeclaration(false);
  ASSERT_EQ(*ent_ref_declaration, VariableDeclaration(new QuerySynonym("v")));
}

TEST(QueryParserTest, ExpressionParseTest) {
  std::vector<Token *> expression_tokens = {
      new QuoteToken(), new SymbolToken("a"), new OperatorToken("+"), new SymbolToken("b"), new OperatorToken("+"), new SymbolToken("c"), new QuoteToken()
  };
  QueryParser parser = QueryParser(expression_tokens);
  QueryDeclaration *expression_declaration = parser.parseExpression();
  ASSERT_EQ(*expression_declaration, ExpressionDeclaration("(((a)+(b))+(c))"));
}

TEST(QueryParserTest, WildExpressionParseTest) {
  std::vector<Token *> expression_tokens = {
      new WildCardToken(), new QuoteToken(), new SymbolToken("a"), new OperatorToken("+"), new SymbolToken("b"), new OperatorToken("+"), new SymbolToken("c"), new QuoteToken(), new WildCardToken()
  };
  QueryParser parser = QueryParser(expression_tokens);
  QueryDeclaration *expression_declaration = parser.parseExpression();
  ASSERT_EQ(*expression_declaration, WildCardExpressionDeclaration("(((a)+(b))+(c))"));
}


TEST(QueryParserTest, ValidSingleParentClauseTest) {
  std::vector<Token *> single_clause_parent_query = {
      new KeywordToken("assign"), new SymbolToken("v1"), new SemicolonToken(), new KeywordToken("assign"),
      new SymbolToken("v2"), new SemicolonToken(), new KeywordToken("Select"), new SymbolToken("v1"),
      new SymbolToken("such"), new SymbolToken("that"), new KeywordToken("Parent"), new RoundOpenBracketToken(),
      new SymbolToken("v1"), new CommaToken(), new SymbolToken("v2"), new RoundCloseBracketToken(),
      new EndOfFileToken()};
  QueryParser parser = QueryParser(single_clause_parent_query);
  parser.parse();

  QueryDeclaration *v1 = new AssignDeclaration(new QuerySynonym("v1"));
  QueryDeclaration *v2 = new AssignDeclaration(new QuerySynonym("v2"));
  Query expected_query = Query(std::vector<QueryDeclaration *>{v1, v2},
                               SelectCall(v1, std::vector<QueryClause *>{new ParentClause(v1, v2)}));

  // check declarations
  ASSERT_EQ(parser.getDeclarations()[0]->getSynonym()->toString(), v1->getSynonym()->toString());
  ASSERT_EQ(parser.getDeclarations()[1]->getSynonym()->toString(), v2->getSynonym()->toString());

  // check call
  ASSERT_EQ(parser.getQueryCalls()[0]->getType(), expected_query.getQueryCall().getType());

  // check clause
  ASSERT_EQ(parser.getQueryCalls()[0]->getClauseVector()[0]->getClauseType(),
            expected_query.getQueryCall().getClauseVector()[0]->getClauseType());

  ASSERT_EQ(static_cast<ParentClause *>(parser.getQueryCalls()[0]->getClauseVector()[0])
                ->getFirst()->getSynonym()->toString(),
            static_cast<ParentClause *>(expected_query.getQueryCall().getClauseVector()[0])
                ->getFirst()->getSynonym()->toString());

  ASSERT_EQ(static_cast<ParentClause *>(parser.getQueryCalls()[0]->getClauseVector()[0])
                ->getSecond()->getSynonym()->toString(),
            static_cast<ParentClause *>(expected_query.getQueryCall().getClauseVector()[0])
                ->getSecond()->getSynonym()->toString());
}

TEST(QueryParserTest, ValidSingleFollowsClauseTest) {
  std::vector<Token *> single_clause_parent_query = {
      new KeywordToken("assign"), new SymbolToken("v1"), new SemicolonToken(), new KeywordToken("assign"),
      new SymbolToken("v2"), new SemicolonToken(), new KeywordToken("Select"), new SymbolToken("v1"),
      new SymbolToken("such"), new SymbolToken("that"), new KeywordToken("Follows"), new RoundOpenBracketToken(),
      new SymbolToken("v1"), new CommaToken(), new SymbolToken("v2"), new RoundCloseBracketToken(),
      new EndOfFileToken()};
  QueryParser parser = QueryParser(single_clause_parent_query);
  parser.parse();

  QueryDeclaration *v1 = new AssignDeclaration(new QuerySynonym("v1"));
  QueryDeclaration *v2 = new AssignDeclaration(new QuerySynonym("v2"));
  Query expected_query = Query(std::vector<QueryDeclaration *>{v1, v2},
                               SelectCall(v1, std::vector<QueryClause *>{new FollowsClause(v1, v2)}));

  // check declarations
  ASSERT_EQ(parser.getDeclarations()[0]->getSynonym()->toString(), v1->getSynonym()->toString());
  ASSERT_EQ(parser.getDeclarations()[1]->getSynonym()->toString(), v2->getSynonym()->toString());

  // check call
  ASSERT_EQ(parser.getQueryCalls()[0]->getType(), expected_query.getQueryCall().getType());

  // check clause
  ASSERT_EQ(parser.getQueryCalls()[0]->getClauseVector()[0]->getClauseType(),
            expected_query.getQueryCall().getClauseVector()[0]->getClauseType());

  ASSERT_EQ(static_cast<FollowsClause *>(parser.getQueryCalls()[0]->getClauseVector()[0])
                ->getFirst()->getSynonym()->toString(),
            static_cast<FollowsClause *>(expected_query.getQueryCall().getClauseVector()[0])
                ->getFirst()->getSynonym()->toString());

  ASSERT_EQ(static_cast<FollowsClause *>(parser.getQueryCalls()[0]->getClauseVector()[0])
                ->getSecond()->getSynonym()->toString(),
            static_cast<FollowsClause *>(expected_query.getQueryCall().getClauseVector()[0])
                ->getSecond()->getSynonym()->toString());
}

TEST(QueryParserTest, ValidSinglePatternClauseTest) {
  std::vector<Token *> single_clause_parent_query = {
      new KeywordToken("assign"), new SymbolToken("a"), new SemicolonToken(), new KeywordToken("Select"),
      new SymbolToken("a"), new KeywordToken("pattern"), new SymbolToken("a"), new RoundOpenBracketToken(),
      new QuoteToken(), new SymbolToken("x"), new QuoteToken(), new CommaToken(),
      new QuoteToken(), new SymbolToken("x"), new OperatorToken("+"), new SymbolToken("y"),
      new QuoteToken(), new RoundCloseBracketToken(), new EndOfFileToken()};
  QueryParser parser = QueryParser(single_clause_parent_query);
  parser.parse();

  QueryDeclaration *a = new AssignDeclaration(new QuerySynonym("a"));

  Query expected_query =
      Query(std::vector<QueryDeclaration *>{a},
            SelectCall(a, {new AssignPatternClause(a,
                                                   new StringDeclaration("x"),
                                                   new ExpressionDeclaration("((x)+(y))"))}));

  // check declarations
  ASSERT_EQ(parser.getDeclarations()[0]->getSynonym()->toString(), a->getSynonym()->toString());

  // check call
  ASSERT_EQ(parser.getQueryCalls()[0]->getType(), expected_query.getQueryCall().getType());

  // check clause
  ASSERT_EQ(parser.getQueryCalls()[0]->getClauseVector()[0]->getClauseType(),
            expected_query.getQueryCall().getClauseVector()[0]->getClauseType());

  ASSERT_EQ(static_cast<AssignPatternClause *>(parser.getQueryCalls()[0]->getClauseVector()[0])
                ->getFirst()->getSynonym()->toString(),
            static_cast<AssignPatternClause *>(expected_query.getQueryCall().getClauseVector()[0])
                ->getFirst()->getSynonym()->toString());

  ASSERT_EQ(static_cast<AssignPatternClause *>(parser.getQueryCalls()[0]->getClauseVector()[0])
                ->getSecond()->getString(),
            static_cast<AssignPatternClause *>(expected_query.getQueryCall().getClauseVector()[0])
                ->getSecond()->getString());

  ASSERT_EQ(static_cast<AssignPatternClause *>(parser.getQueryCalls()[0]->getClauseVector()[0])
                ->getThird()->getString(),
            static_cast<AssignPatternClause *>(expected_query.getQueryCall().getClauseVector()[0])
                ->getThird()->getString());
}

TEST(QueryParserTest, InvalidTokenTest1) {
  std::vector<Token *> invalid_tokens = {new KeywordToken("var"), new SymbolToken("v1"), new SemicolonToken(),
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
