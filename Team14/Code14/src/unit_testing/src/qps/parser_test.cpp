#include <gtest/gtest.h>

#include <algorithm>

#include "qps/query_parser/query_parser.h"

TEST(QueryParserTest, SynonymParseTest) {
  std::vector<Token *> tokens = {new KeywordToken("syn")};
  auto *expected = new QuerySynonym("syn");
  QueryParser parser = QueryParser(tokens);
  auto *synonym = parser.parseSynonym();
  ASSERT_EQ(*synonym, *expected);
}

TEST(QueryParserTest, SelectCallParseTest) {
  std::vector<Token *> tokens = {new KeywordToken("Select"), new SymbolToken("v")};
  auto *expected = new SelectCall(new SynonymReference(new QuerySynonym("v")));
  QueryParser parser = QueryParser(tokens);
  auto *select = parser.parseQueryCall();
  ASSERT_EQ(*select->getReferences().front()->getSynonym(), *expected->getReferences().front()->getSynonym());
}

TEST(QueryParserTest, AssignDeclarationParseTest) {
  std::vector<Token *> tokens = {new KeywordToken("assign"), new SymbolToken("a"), new SemicolonToken()};
  auto *expected = new AssignDeclaration(new QuerySynonym("a"));
  QueryParser parser = QueryParser(tokens);
  auto declarations = parser.parseDeclarations();
  ASSERT_EQ(*declarations.front(), *expected);
}

TEST(QueryParserTest, StmtDeclarationParseTest) {
  std::vector<Token *> tokens = {new KeywordToken("stmt"), new SymbolToken("s"), new SemicolonToken()};
  auto *expected = new StatementDeclaration(new QuerySynonym("s"));
  QueryParser parser = QueryParser(tokens);
  auto declarations = parser.parseDeclarations();
  ASSERT_EQ(*declarations.front(), *expected);
}

TEST(QueryParserTest, VariableDeclarationParseTest) {
  std::vector<Token *> tokens = {new KeywordToken("variable"), new SymbolToken("v"), new SemicolonToken()};
  auto *expected = new VariableDeclaration(new QuerySynonym("v"));
  QueryParser parser = QueryParser(tokens);
  auto declarations = parser.parseDeclarations();
  ASSERT_EQ(*declarations.front(), *expected);
}

TEST(QueryParserTest, ConstantDeclarationParseTest) {
  std::vector<Token *> tokens = {new KeywordToken("constant"), new SymbolToken("c"), new SemicolonToken()};
  auto *expected = new ConstantDeclaration(new QuerySynonym("c"));
  QueryParser parser = QueryParser(tokens);
  auto declarations = parser.parseDeclarations();
  ASSERT_EQ(*declarations.front(), *expected);
}

TEST(QueryParserTest, ProcedureDeclarationParseTest) {
  std::vector<Token *> tokens = {new KeywordToken("procedure"), new SymbolToken("p"), new SemicolonToken()};
  auto *expected = new ProcedureDeclaration(new QuerySynonym("p"));
  QueryParser parser = QueryParser(tokens);
  auto declarations = parser.parseDeclarations();
  ASSERT_EQ(*declarations.front(), *expected);
}

TEST(QueryParserTest, ReadDeclarationParseTest) {
  std::vector<Token *> tokens = {new KeywordToken("read"), new SymbolToken("r"), new SemicolonToken()};
  auto *expected = new ReadDeclaration(new QuerySynonym("r"));
  QueryParser parser = QueryParser(tokens);
  auto declarations = parser.parseDeclarations();
  ASSERT_EQ(*declarations.front(), *expected);
}

TEST(QueryParserTest, PrintDeclarationParseTest) {
  std::vector<Token *> tokens = {new KeywordToken("print"), new SymbolToken("pr"), new SemicolonToken()};
  auto *expected = new PrintDeclaration(new QuerySynonym("pr"));
  QueryParser parser = QueryParser(tokens);
  auto declarations = parser.parseDeclarations();
  ASSERT_EQ(*declarations.front(), *expected);
}

TEST(QueryParserTest, CallDeclarationParseTest) {
  std::vector<Token *> tokens = {new KeywordToken("call"), new SymbolToken("cl"), new SemicolonToken()};
  auto *expected = new CallDeclaration(new QuerySynonym("cl"));
  QueryParser parser = QueryParser(tokens);
  auto declarations = parser.parseDeclarations();
  ASSERT_EQ(*declarations.front(), *expected);
}

TEST(QueryParserTest, WhileDeclarationParseTest) {
  std::vector<Token *> tokens = {new KeywordToken("while"), new SymbolToken("wh"), new SemicolonToken()};
  auto *expected = new WhileDeclaration(new QuerySynonym("wh"));
  QueryParser parser = QueryParser(tokens);
  auto declarations = parser.parseDeclarations();
  ASSERT_EQ(*declarations.front(), *expected);
}

TEST(QueryParserTest, IfDeclarationParseTest) {
  std::vector<Token *> tokens = {new KeywordToken("if"), new SymbolToken("wh"), new SemicolonToken()};
  auto *expected = new IfDeclaration(new QuerySynonym("wh"));
  QueryParser parser = QueryParser(tokens);
  auto declarations = parser.parseDeclarations();
  ASSERT_EQ(*declarations.front(), *expected);
}

TEST(QueryParserTest, MultiAssignDeclarationParseTest) {
  std::vector<Token *> tokens =
      {new KeywordToken("assign"), new SymbolToken("a1"), new CommaToken(), new SymbolToken("a2"), new CommaToken(),
       new SymbolToken("a3"), new SemicolonToken()};
  SynonymReferences expected =
      {new AssignDeclaration(new QuerySynonym("a1")), new AssignDeclaration(new QuerySynonym("a2")),
       new AssignDeclaration(new QuerySynonym("a3"))};
  QueryParser parser = QueryParser(tokens);
  auto declarations = parser.parseDeclarations();
  for (int i = 0; i < expected.size(); ++i) {
    ASSERT_EQ(*declarations[i], *expected[i]);
  }
}

TEST(QueryParserTest, MultiStmtDeclarationParseTest) {
  std::vector<Token *> tokens =
      {new KeywordToken("stmt"), new SymbolToken("s1"), new CommaToken(), new SymbolToken("s2"), new CommaToken(),
       new SymbolToken("s3"), new SemicolonToken()};
  SynonymReferences expected =
      {new StatementDeclaration(new QuerySynonym("s1")), new StatementDeclaration(new QuerySynonym("s2")),
       new StatementDeclaration(new QuerySynonym("s3"))};
  QueryParser parser = QueryParser(tokens);
  auto declarations = parser.parseDeclarations();
  for (int i = 0; i < expected.size(); ++i) {
    ASSERT_EQ(*declarations[i], *expected[i]);
  }
}

TEST(QueryParserTest, MultiVariableDeclarationParseTest) {
  std::vector<Token *> tokens =
      {new KeywordToken("variable"), new SymbolToken("v1"), new CommaToken(), new SymbolToken("v2"), new CommaToken(),
       new SymbolToken("v3"), new SemicolonToken()};
  SynonymReferences expected =
      {new VariableDeclaration(new QuerySynonym("v1")), new VariableDeclaration(new QuerySynonym("v2")),
       new VariableDeclaration(new QuerySynonym("v3"))};
  QueryParser parser = QueryParser(tokens);
  auto declarations = parser.parseDeclarations();
  for (int i = 0; i < expected.size(); ++i) {
    ASSERT_EQ(*declarations[i], *expected[i]);
  }
}

TEST(QueryParserTest, MultiConstantDeclarationParseTest) {
  std::vector<Token *> tokens =
      {new KeywordToken("constant"), new SymbolToken("c1"), new CommaToken(), new SymbolToken("c2"), new CommaToken(),
       new SymbolToken("c3"), new SemicolonToken()};
  SynonymReferences expected =
      {new ConstantDeclaration(new QuerySynonym("c1")), new ConstantDeclaration(new QuerySynonym("c2")),
       new ConstantDeclaration(new QuerySynonym("c3"))};
  QueryParser parser = QueryParser(tokens);
  auto declarations = parser.parseDeclarations();
  for (int i = 0; i < expected.size(); ++i) {
    ASSERT_EQ(*declarations[i], *expected[i]);
  }
}

TEST(QueryParserTest, MultiProcedureDeclarationParseTest) {
  std::vector<Token *> tokens =
      {new KeywordToken("procedure"), new SymbolToken("p1"), new CommaToken(), new SymbolToken("p2"), new CommaToken(),
       new SymbolToken("p3"), new SemicolonToken()};
  SynonymReferences expected =
      {new ProcedureDeclaration(new QuerySynonym("p1")), new ProcedureDeclaration(new QuerySynonym("p2")),
       new ProcedureDeclaration(new QuerySynonym("p3"))};
  QueryParser parser = QueryParser(tokens);
  auto declarations = parser.parseDeclarations();
  for (int i = 0; i < expected.size(); ++i) {
    ASSERT_EQ(*declarations[i], *expected[i]);
  }
}

TEST(QueryParserTest, MultiReadDeclarationParseTest) {
  std::vector<Token *> tokens =
      {new KeywordToken("read"), new SymbolToken("rd1"), new CommaToken(), new SymbolToken("rd2"), new CommaToken(),
       new SymbolToken("rd3"), new SemicolonToken()};
  SynonymReferences expected =
      {new ReadDeclaration(new QuerySynonym("rd1")), new ReadDeclaration(new QuerySynonym("rd2")),
       new ReadDeclaration(new QuerySynonym("rd3"))};
  QueryParser parser = QueryParser(tokens);
  auto declarations = parser.parseDeclarations();
  for (int i = 0; i < expected.size(); ++i) {
    ASSERT_EQ(*declarations[i], *expected[i]);
  }
}

TEST(QueryParserTest, MultiPrintDeclarationParseTest) {
  std::vector<Token *> tokens =
      {new KeywordToken("print"), new SymbolToken("pr1"), new CommaToken(), new SymbolToken("pr2"), new CommaToken(),
       new SymbolToken("pr3"), new SemicolonToken()};
  SynonymReferences expected =
      {new PrintDeclaration(new QuerySynonym("pr1")), new PrintDeclaration(new QuerySynonym("pr2")),
       new PrintDeclaration(new QuerySynonym("pr3"))};
  QueryParser parser = QueryParser(tokens);
  auto declarations = parser.parseDeclarations();
  for (int i = 0; i < expected.size(); ++i) {
    ASSERT_EQ(*declarations[i], *expected[i]);
  }
}

TEST(QueryParserTest, MultiCallDeclarationParseTest) {
  std::vector<Token *> tokens =
      {new KeywordToken("call"), new SymbolToken("cl1"), new CommaToken(), new SymbolToken("cl2"), new CommaToken(),
       new SymbolToken("cl3"), new SemicolonToken()};
  SynonymReferences expected =
      {new CallDeclaration(new QuerySynonym("cl1")), new CallDeclaration(new QuerySynonym("cl2")),
       new CallDeclaration(new QuerySynonym("cl3"))};
  QueryParser parser = QueryParser(tokens);
  auto declarations = parser.parseDeclarations();
  for (int i = 0; i < expected.size(); ++i) {
    ASSERT_EQ(*declarations[i], *expected[i]);
  }
}

TEST(QueryParserTest, MultiWhileDeclarationParseTest) {
  std::vector<Token *> tokens =
      {new KeywordToken("while"), new SymbolToken("wh1"), new CommaToken(), new SymbolToken("wh2"), new CommaToken(),
       new SymbolToken("wh3"), new SemicolonToken()};
  SynonymReferences expected =
      {new WhileDeclaration(new QuerySynonym("wh1")), new WhileDeclaration(new QuerySynonym("wh2")),
       new WhileDeclaration(new QuerySynonym("wh3"))};
  QueryParser parser = QueryParser(tokens);
  auto declarations = parser.parseDeclarations();
  for (int i = 0; i < expected.size(); ++i) {
    ASSERT_EQ(*declarations[i], *expected[i]);
  }
}

TEST(QueryParserTest, MultiIfDeclarationParseTest) {
  std::vector<Token *> tokens =
      {new KeywordToken("if"), new SymbolToken("wh1"), new CommaToken(), new SymbolToken("wh2"), new CommaToken(),
       new SymbolToken("wh3"), new SemicolonToken()};
  SynonymReferences expected = {new IfDeclaration(new QuerySynonym("wh1")), new IfDeclaration(new QuerySynonym("wh2")),
                                new IfDeclaration(new QuerySynonym("wh3"))};
  QueryParser parser = QueryParser(tokens);
  auto declarations = parser.parseDeclarations();
  for (int i = 0; i < expected.size(); ++i) {
    ASSERT_EQ(*declarations[i], *expected[i]);
  }
}

TEST(QueryParserTest, MultiMixedDeclarationParseTest) {
  std::vector<Token *> tokens =
      {new KeywordToken("stmt"), new SymbolToken("s1"), new SemicolonToken(), new KeywordToken("variable"),
       new SymbolToken("v1"), new CommaToken(), new SymbolToken("v2"), new SemicolonToken(), new KeywordToken("call"),
       new SymbolToken("c1"), new CommaToken(), new SymbolToken("c2"), new SemicolonToken(),
       new KeywordToken("procedure"), new SymbolToken("p1"), new CommaToken(), new SymbolToken("p2"), new CommaToken(),
       new SymbolToken("p3"), new SemicolonToken()};
  SynonymReferences expected =
      {new StatementDeclaration(new QuerySynonym("s1")), new VariableDeclaration(new QuerySynonym("v1")),
       new VariableDeclaration(new QuerySynonym("v2")), new CallDeclaration(new QuerySynonym("c1")),
       new CallDeclaration(new QuerySynonym("c2")), new ProcedureDeclaration(new QuerySynonym("p1")),
       new ProcedureDeclaration(new QuerySynonym("p2")), new ProcedureDeclaration(new QuerySynonym("p3"))};
  QueryParser parser = QueryParser(tokens);
  auto declarations = parser.parseDeclarations();
  for (int i = 0; i < expected.size(); ++i) {
    ASSERT_EQ(*declarations[i], *expected[i]);
  }
}

// Declaration semantic error should not be caught by parser
TEST(QueryParserTest, SameSynonymMultiMixedDeclarationParseTest) {
  std::vector<Token *> tokens =
      {new KeywordToken("stmt"), new SymbolToken("s"), new SemicolonToken(), new KeywordToken("variable"),
       new SymbolToken("s"), new CommaToken(), new SymbolToken("s"), new SemicolonToken(), new KeywordToken("call"),
       new SymbolToken("s"), new CommaToken(), new SymbolToken("s"), new SemicolonToken(),
       new KeywordToken("procedure"), new SymbolToken("s"), new CommaToken(), new SymbolToken("s"), new CommaToken(),
       new SymbolToken("s"), new SemicolonToken()};
  SynonymReferences expected =
      {new StatementDeclaration(new QuerySynonym("s")), new VariableDeclaration(new QuerySynonym("s")),
       new VariableDeclaration(new QuerySynonym("s")), new CallDeclaration(new QuerySynonym("s")),
       new CallDeclaration(new QuerySynonym("s")), new ProcedureDeclaration(new QuerySynonym("s")),
       new ProcedureDeclaration(new QuerySynonym("s")), new ProcedureDeclaration(new QuerySynonym("s"))};
  QueryParser parser = QueryParser(tokens);
  auto declarations = parser.parseDeclarations();
  for (int i = 0; i < expected.size(); ++i) {
    ASSERT_EQ(*declarations[i], *expected[i]);
  }
}

TEST(QueryParserTest, IntegerStmtReferenceParseTest) {
  std::vector<Token *> tokens = {new LiteralToken("1")};
  auto *expected = new IntegerReference("1");
  QueryParser parser = QueryParser(tokens);
  auto *reference = parser.parseClauseReference();
  ASSERT_EQ(*reference, *expected);
}

TEST(QueryParserTest, IdentEntReferenceParseTest) {
  std::vector<Token *> tokens = {new QuoteToken(), new SymbolToken("ident"), new QuoteToken()};
  auto *expected = new IdentReference("ident");
  QueryParser parser = QueryParser(tokens);
  auto *reference = parser.parseClauseReference();
  ASSERT_EQ(*reference, *expected);
}

TEST(QueryParserTest, SynonymReferenceParseTest) {
  std::vector<Token *> tokens = {new SymbolToken("s")};
  auto *expected = new SynonymReference(new QuerySynonym("s"));
  QueryParser parser = QueryParser(tokens);
  auto *reference = parser.parseClauseReference();
  ASSERT_EQ(*reference, *expected);
}

TEST(QueryParserTest, WildcardReferenceParseTest) {
  std::vector<Token *> tokens = {new WildCardToken()};
  auto *expected = new WildcardReference();
  QueryParser parser = QueryParser(tokens);
  auto *reference = parser.parseClauseReference();
  ASSERT_EQ(*reference, *expected);
}

/*
 *  "x + 6 * y"
*/
TEST(QueryParserTest, ExactExpressionParseTest) {
  std::vector<Token *> tokens =
      {new QuoteToken(), new SymbolToken("x"), new OperatorToken("+"), new LiteralToken("6"), new OperatorToken("*"),
       new SymbolToken("y"), new QuoteToken(), new EndOfFileToken()};
  auto *expected = new ExactExpression("((x)+((6)*(y)))");
  QueryParser parser = QueryParser(tokens);
  auto *expr = parser.parseExpression();
  ASSERT_EQ(*expr, *expected);
}

/*
 *  _"x + 6 * y"_
*/
TEST(QueryParserTest, WildExpressionParseTest) {
  std::vector<Token *> tokens =
      {new WildCardToken(), new QuoteToken(), new SymbolToken("x"), new OperatorToken("+"), new LiteralToken("6"),
       new OperatorToken("*"), new SymbolToken("y"), new QuoteToken(), new WildCardToken(), new EndOfFileToken()};
  auto *expected = new WildExpression("((x)+((6)*(y)))");
  QueryParser parser = QueryParser(tokens);
  auto *expr = parser.parseExpression();
  ASSERT_EQ(*expr, *expected);
}

/*
 *  such that Modifies(p, "x")
*/
TEST(QueryParserTest, SingleSuchThatClauseParseTest) {
  std::vector<Token *> tokens =
      {new SymbolToken("such"), new SymbolToken("that"), new KeywordToken("Modifies"), new RoundOpenBracketToken(),
       new SymbolToken("p"), new CommaToken(), new QuoteToken(), new SymbolToken("x"), new QuoteToken(),
       new RoundCloseBracketToken(), new EndOfFileToken()};
  auto *synonym_reference_1 = new SynonymReference(new QuerySynonym("p"));
  auto *ident_reference_1 = new IdentReference("x");
  auto *expected = new ModifiesClause(synonym_reference_1, ident_reference_1);
  QueryParser parser = QueryParser(tokens);
  auto clauses = parser.parseClauses();
  ASSERT_EQ(*clauses.front(), *expected);
}

/*
 *  pattern a1(_, "x")
*/
TEST(QueryParserTest, SingleExactPatternClauseParseTest) {
  std::vector<Token *> tokens =
      {new KeywordToken("pattern"), new SymbolToken("a1"), new RoundOpenBracketToken(), new WildCardToken(),
       new CommaToken(), new QuoteToken(), new SymbolToken("x"), new QuoteToken(), new RoundCloseBracketToken(),
       new EndOfFileToken()};
  auto *synonym_reference_1 = new SynonymReference(new QuerySynonym("a1"));
  auto *wildcard_reference = new WildcardReference();
  auto *expected = new PatternClause(synonym_reference_1, wildcard_reference, new ExactExpression("(x)"));
  QueryParser parser = QueryParser(tokens);
  auto clauses = parser.parseClauses();
  ASSERT_EQ(*clauses.front(), *expected);
}

/*
 *  pattern a1(_, _"x"_)
*/
TEST(QueryParserTest, SingleWildPatternClauseParseTest) {
  std::vector<Token *> tokens =
      {new KeywordToken("pattern"), new SymbolToken("a1"), new RoundOpenBracketToken(), new WildCardToken(),
       new CommaToken(), new WildCardToken(), new QuoteToken(), new SymbolToken("x"), new QuoteToken(),
       new WildCardToken(), new RoundCloseBracketToken(), new EndOfFileToken()};
  auto *synonym_reference_1 = new SynonymReference(new QuerySynonym("a1"));
  auto *wildcard_reference = new WildcardReference();
  auto *expected = new PatternClause(synonym_reference_1, wildcard_reference, new WildExpression("(x)"));
  QueryParser parser = QueryParser(tokens);
  auto clauses = parser.parseClauses();
  ASSERT_EQ(*clauses.front(), *expected);
}

/*
 *  such that Modifies(p, "x") such that Parent(7, 2) such that Parent*(2, 7)
*/
TEST(QueryParserTest, MultipleSuchThatClauseParseTest) {
  std::vector<Token *> tokens =
      {new SymbolToken("such"), new SymbolToken("that"), new KeywordToken("Modifies"), new RoundOpenBracketToken(),
       new SymbolToken("p"), new CommaToken(), new QuoteToken(), new SymbolToken("x"), new QuoteToken(),
       new RoundCloseBracketToken(), new SymbolToken("such"), new SymbolToken("that"), new KeywordToken("Parent"),
       new RoundOpenBracketToken(), new LiteralToken("7"), new CommaToken(), new LiteralToken("2"),
       new RoundCloseBracketToken(), new SymbolToken("such"), new SymbolToken("that"), new KeywordToken("Parent"),
       new OperatorToken("*"), new RoundOpenBracketToken(), new LiteralToken("2"), new CommaToken(),
       new LiteralToken("7"), new RoundCloseBracketToken(), new EndOfFileToken()};
  auto *synonym_reference_1 = new SynonymReference(new QuerySynonym("p"));
  auto *ident_reference_1 = new IdentReference("x");
  auto *int_reference_1 = new IntegerReference("7");
  auto *int_reference_2 = new IntegerReference("2");
  Clauses expected =
      {new ModifiesClause(synonym_reference_1, ident_reference_1), new ParentClause(int_reference_1, int_reference_2),
       new ParentTClause(int_reference_2, int_reference_1)};
  QueryParser parser = QueryParser(tokens);
  auto clauses = parser.parseClauses();
  for (int i = 0; i < expected.size(); ++i) {
    ASSERT_EQ(*clauses[i], *expected[i]);
  }
}

/*
 *  such that Modifies(p, "x") and Parent(7, 2) and Parent*(2, 7)
*/
TEST(QueryParserTest, MultipleSuchThatAndClauseParseTest) {
  std::vector<Token *> tokens =
      {new SymbolToken("such"), new SymbolToken("that"), new KeywordToken("Modifies"), new RoundOpenBracketToken(),
       new SymbolToken("p"), new CommaToken(), new QuoteToken(), new SymbolToken("x"), new QuoteToken(),
       new RoundCloseBracketToken(), new SymbolToken("and"), new KeywordToken("Parent"), new RoundOpenBracketToken(),
       new LiteralToken("7"), new CommaToken(), new LiteralToken("2"), new RoundCloseBracketToken(),
       new SymbolToken("and"), new KeywordToken("Parent"), new OperatorToken("*"), new RoundOpenBracketToken(),
       new LiteralToken("2"), new CommaToken(), new LiteralToken("7"), new RoundCloseBracketToken(),
       new EndOfFileToken()};
  auto *synonym_reference_1 = new SynonymReference(new QuerySynonym("p"));
  auto *ident_reference_1 = new IdentReference("x");
  auto *int_reference_1 = new IntegerReference("7");
  auto *int_reference_2 = new IntegerReference("2");
  Clauses expected =
      {new ModifiesClause(synonym_reference_1, ident_reference_1), new ParentClause(int_reference_1, int_reference_2),
       new ParentTClause(int_reference_2, int_reference_1)};
  QueryParser parser = QueryParser(tokens);
  auto clauses = parser.parseClauses();
  for (int i = 0; i < expected.size(); ++i) {
    ASSERT_EQ(*clauses[i], *expected[i]);
  }
}

/*
 *  such that Modifies(p, "x") and Parent(7, 2) and Parent*(2, 7) pattern a1(_, "x") and a2(_, "y")
*/
TEST(QueryParserTest, MultipleMixedAndClauseParseTest) {
  std::vector<Token *> tokens =
      {new SymbolToken("such"), new SymbolToken("that"), new KeywordToken("Modifies"), new RoundOpenBracketToken(),
       new SymbolToken("p"), new CommaToken(), new QuoteToken(), new SymbolToken("x"), new QuoteToken(),
       new RoundCloseBracketToken(), new SymbolToken("and"), new KeywordToken("Parent"), new RoundOpenBracketToken(),
       new LiteralToken("7"), new CommaToken(), new LiteralToken("2"), new RoundCloseBracketToken(),
       new SymbolToken("and"), new KeywordToken("Parent"), new OperatorToken("*"), new RoundOpenBracketToken(),
       new LiteralToken("2"), new CommaToken(), new LiteralToken("7"), new RoundCloseBracketToken(),
       new KeywordToken("pattern"), new SymbolToken("a1"), new RoundOpenBracketToken(), new WildCardToken(),
       new CommaToken(), new QuoteToken(), new SymbolToken("x"), new QuoteToken(), new RoundCloseBracketToken(),
       new KeywordToken("and"), new SymbolToken("a2"), new RoundOpenBracketToken(), new WildCardToken(),
       new CommaToken(), new QuoteToken(), new SymbolToken("y"), new QuoteToken(), new RoundCloseBracketToken(),
       new EndOfFileToken()};
  auto *synonym_reference_1 = new SynonymReference(new QuerySynonym("p"));
  auto *synonym_reference_2 = new SynonymReference(new QuerySynonym("a1"));
  auto *synonym_reference_3 = new SynonymReference(new QuerySynonym("a2"));
  auto *ident_reference_1 = new IdentReference("x");
  auto *int_reference_1 = new IntegerReference("7");
  auto *int_reference_2 = new IntegerReference("2");
  auto *wildcard_reference = new WildcardReference();
  Clauses expected =
      {new ModifiesClause(synonym_reference_1, ident_reference_1), new ParentClause(int_reference_1, int_reference_2),
       new ParentTClause(int_reference_2, int_reference_1),
       new PatternClause(synonym_reference_2, wildcard_reference, new ExactExpression("(x)")),
       new PatternClause(synonym_reference_3, wildcard_reference, new ExactExpression("(y)"))};
  QueryParser parser = QueryParser(tokens);
  auto clauses = parser.parseClauses();
  for (int i = 0; i < expected.size(); ++i) {
    ASSERT_EQ(*clauses[i], *expected[i]);
  }
}