#include <gtest/gtest.h>

#include <algorithm>

#include "qps/query_parser/query_parser.h"

TEST(QueryParserTest, SynonymParseTest) {
  std::vector<Token *> tokens = {new KeywordToken("syn"), new EndOfFileToken()};
  auto *expected = new SynonymBlueprint("syn");
  QueryParser parser = QueryParser(tokens);
  auto *synonym = parser.ParseSynonym();
  ASSERT_EQ(*synonym, *expected);
}

TEST(QueryParserTest, ElemReferenceParseTest) {
  std::vector<Token *> tokens = {new SymbolToken("elem"), new EndOfFileToken()};
  auto *expected = new ElemBlueprint(new SynonymBlueprint("elem"), AttributeType::kNone);
  QueryParser parser = QueryParser(tokens);
  auto *reference = parser.ParseElem();
  ASSERT_EQ(*reference, *expected);
}

TEST(QueryParserTest, ExactExprReferenceParseTest) {
  std::vector<Token *> tokens =
      {new QuoteToken(), new SymbolToken("x"), new OperatorToken("+"), new SymbolToken("y"), new QuoteToken(),
       new SemicolonToken(), new EndOfFileToken()};
  auto *expected = new ExprBlueprint("((x)+(y))", true);
  QueryParser parser = QueryParser(tokens);
  auto *reference = parser.ParseExpr();
  ASSERT_EQ(*reference, *expected);
}

TEST(QueryParserTest, WildExprReferenceParseTest) {
  std::vector<Token *> tokens =
      {new WildCardToken(), new QuoteToken(), new SymbolToken("t"), new OperatorToken("+"), new SymbolToken("v"),
       new QuoteToken(), new WildCardToken(), new SemicolonToken(), new EndOfFileToken()};
  auto *expected = new ExprBlueprint("((t)+(v))", false);
  QueryParser parser = QueryParser(tokens);
  auto *reference = parser.ParseExpr();
  ASSERT_EQ(*reference, *expected);
}

TEST(QueryParserTest, IdentReferenceParseTest) {
  std::vector<Token *> tokens = {new QuoteToken(), new SymbolToken("ident"), new QuoteToken(), new EndOfFileToken()};
  auto *expected = new ReferenceBlueprint(ReferenceType::kIdent, "ident");
  QueryParser parser = QueryParser(tokens);
  auto *reference = parser.ParseReference();
  ASSERT_EQ(*reference, *expected);
}

TEST(QueryParserTest, IntegerReferenceParseTest) {
  std::vector<Token *> tokens = {new LiteralToken("123"), new EndOfFileToken()};
  auto *expected = new ReferenceBlueprint(ReferenceType::kInteger, "123");
  QueryParser parser = QueryParser(tokens);
  auto *reference = parser.ParseReference();
  ASSERT_EQ(*reference, *expected);
}

TEST(QueryParserTest, ValidParseTest) {
  std::vector<Token *> tokens = {new LiteralToken("123"), new EndOfFileToken()};
  auto *expected = new ReferenceBlueprint(ReferenceType::kInteger, "123");
  QueryParser parser = QueryParser(tokens);
  auto *reference = parser.ParseReference();
  ASSERT_EQ(*reference, *expected);
}

/*
 *  Select BOOLEAN
*/
TEST(QueryParserTest, NoDeclarationsValidParseTest) {
  std::vector<Token *> tokens =
      {new KeywordToken("Select"), new SymbolToken("BOOLEAN"), new EndOfFileToken()};
  SynonymSet declarations = {};
  ClauseVector clauses = {};
  QueryParser parser = QueryParser(tokens);
  auto *query = parser.Parse();
  auto *expected = new Query(declarations, new BooleanSelect(), clauses);
  ASSERT_EQ(query->toString(), expected->toString());
}

/*
 *  procedure p; assign a1, a2;
 *  Select p
*/
TEST(QueryParserTest, NoClausesValidParseTest) {
  std::vector<Token *> tokens =
      {new KeywordToken("procedure"), new SymbolToken("p"), new SemicolonToken(), new KeywordToken("assign"),
       new SymbolToken("a1"), new CommaToken(), new SymbolToken("a2"), new SemicolonToken(),
       new KeywordToken("Select"), new SymbolToken("p"), new EndOfFileToken()};

  auto *synonym_1 = new QuerySynonym("p", EntityType::kProcedure);
  auto *synonym_2 = new QuerySynonym("a1", EntityType::kAssignStmt);
  auto *synonym_3 = new QuerySynonym("a2", EntityType::kAssignStmt);
  SynonymSet declarations = {synonym_1, synonym_2, synonym_3};
  ClauseVector clauses = {};
  QueryParser parser = QueryParser(tokens);
  auto *query = parser.Parse();
  auto *expected = new Query(declarations, new ElemSelect({new AttrReference(synonym_1, AttributeType::kNone)}), clauses);
  ASSERT_EQ(query->toString(), expected->toString());
}


/*
 *  procedure p; assign a1, a2;
 *  Select p such that Modifies(p, "x") and Parent(7, 2) and Parent*(2, 7) pattern a1(_, "x") and a2(_, "y")
*/
TEST(QueryParserTest, ComplexValidParseTest1) {
  std::vector<Token *> tokens =
      {new KeywordToken("procedure"), new SymbolToken("p"), new SemicolonToken(), new KeywordToken("assign"),
       new SymbolToken("a1"), new CommaToken(), new SymbolToken("a2"), new SemicolonToken(),
       new KeywordToken("Select"), new SymbolToken("p"), new SymbolToken("such"), new SymbolToken("that"),
       new KeywordToken("Modifies"), new RoundOpenBracketToken(),
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

  auto *synonym_1 = new QuerySynonym("p", EntityType::kProcedure);
  auto *synonym_2 = new QuerySynonym("a1", EntityType::kAssignStmt);
  auto *synonym_3 = new QuerySynonym("a2", EntityType::kAssignStmt);
  auto *synonym_reference_1 = new SynonymReference(synonym_1);
  auto *synonym_reference_2 = new SynonymReference(synonym_2);
  auto *synonym_reference_3 = new SynonymReference(synonym_3);
  auto *ident_reference_1 = new IdentReference("x");
  auto *int_reference_1 = new IntegerReference("7");
  auto *int_reference_2 = new IntegerReference("2");
  auto *wildcard_reference = new WildcardReference(EntityType::kVariable);
  SynonymSet declarations = {synonym_1, synonym_2, synonym_3};
  ClauseVector clauses = {new ModifiesClause(synonym_reference_1, ident_reference_1), new ParentClause(int_reference_1, int_reference_2),
                         new ParentTClause(int_reference_2, int_reference_1),
                         new PatternClause(synonym_reference_2, wildcard_reference, new ExactExpression("(x)"),
                                           nullptr),
                         new PatternClause(synonym_reference_3, wildcard_reference, new ExactExpression("(y)"),
                                           nullptr)};
  QueryParser parser = QueryParser(tokens);
  auto *query = parser.Parse();
  auto *expected = new Query(declarations, new ElemSelect({new AttrReference(synonym_1, AttributeType::kNone)}), clauses);
  ASSERT_EQ(query->toString(), expected->toString());
}


/*
 *  procedure p; assign a1, a2;
 *  Select p such that Modifies(p, "x") and Parent(7, 2) and Parent*(2, 7) pattern a1(_, "x") and a2(_, "y") with a1.stmt# = 1 and p.procName = "proc"
*/

TEST(QueryParserTest, ComplexValidParseTest2) {
  std::vector<Token *> tokens =
      {new KeywordToken("procedure"), new SymbolToken("p"), new SemicolonToken(), new KeywordToken("assign"),
       new SymbolToken("a1"), new CommaToken(), new SymbolToken("a2"), new SemicolonToken(),
       new KeywordToken("Select"), new SymbolToken("p"), new SymbolToken("such"), new SymbolToken("that"),
       new KeywordToken("Modifies"), new RoundOpenBracketToken(),
       new SymbolToken("p"), new CommaToken(), new QuoteToken(), new SymbolToken("x"), new QuoteToken(),
       new RoundCloseBracketToken(), new SymbolToken("and"), new KeywordToken("Parent"), new RoundOpenBracketToken(),
       new LiteralToken("7"), new CommaToken(), new LiteralToken("2"), new RoundCloseBracketToken(),
       new SymbolToken("and"), new KeywordToken("Parent"), new OperatorToken("*"), new RoundOpenBracketToken(),
       new LiteralToken("2"), new CommaToken(), new LiteralToken("7"), new RoundCloseBracketToken(),
       new KeywordToken("pattern"), new SymbolToken("a1"), new RoundOpenBracketToken(), new WildCardToken(),
       new CommaToken(), new QuoteToken(), new SymbolToken("x"), new QuoteToken(), new RoundCloseBracketToken(),
       new KeywordToken("and"), new SymbolToken("a2"), new RoundOpenBracketToken(), new WildCardToken(),
       new CommaToken(), new QuoteToken(), new SymbolToken("y"), new QuoteToken(), new RoundCloseBracketToken(),
       new SymbolToken("with"),
       new SymbolToken("a1"), new DotToken(), new SymbolToken("stmt"), new HashtagToken(), new ComparatorToken("="),
       new LiteralToken("1"), new SymbolToken("and"), new SymbolToken("p"), new DotToken(), new SymbolToken("procName"),
       new ComparatorToken("="),
       new QuoteToken(), new SymbolToken("proc"), new QuoteToken(),
       new EndOfFileToken()};

  auto *synonym_1 = new QuerySynonym("p", EntityType::kProcedure);
  auto *synonym_2 = new QuerySynonym("a1", EntityType::kAssignStmt);
  auto *synonym_3 = new QuerySynonym("a2", EntityType::kAssignStmt);
  auto *synonym_reference_1 = new SynonymReference(synonym_1);
  auto *synonym_reference_2 = new SynonymReference(synonym_2);
  auto *synonym_reference_3 = new SynonymReference(synonym_3);
  auto *attr_ref_1 = new AttrReference(synonym_1, AttributeType::kProcName);
  auto *attr_ref_2 = new AttrReference(synonym_2, AttributeType::kStmtNo);
  auto *ident_reference_1 = new IdentReference("x");
  auto *ident_reference_2 = new IdentReference("proc");
  auto *int_reference_1 = new IntegerReference("7");
  auto *int_reference_2 = new IntegerReference("2");
  auto *int_reference_3 = new IntegerReference("1");
  auto *wildcard_reference = new WildcardReference(EntityType::kVariable);
  SynonymSet declarations = {synonym_1, synonym_2, synonym_3};
  ClauseVector clauses = {new ModifiesClause(synonym_reference_1, ident_reference_1), new ParentClause(int_reference_1, int_reference_2),
                          new ParentTClause(int_reference_2, int_reference_1),
                          new PatternClause(synonym_reference_2, wildcard_reference, new ExactExpression("(x)"),
                                            nullptr),
                          new PatternClause(synonym_reference_3, wildcard_reference, new ExactExpression("(y)"),
                                            nullptr),
                          new WithClause(Comparator::kEquals, attr_ref_2, int_reference_3),
                          new WithClause(Comparator::kEquals, attr_ref_1, ident_reference_2)};
  QueryParser parser = QueryParser(tokens);
  auto *query = parser.Parse();
  auto *expected = new Query(declarations, new ElemSelect({new AttrReference(synonym_1, AttributeType::kNone)}), clauses);
  ASSERT_EQ(query->toString(), expected->toString());
}

/*
 *  procedure p; assign a1, a2;
 *  Select p such that Modifies(p, "x") and Parent(7, 2) and Parent(7, 2) and Parent*(2, 7) and Parent*(2, 7) pattern a1(_, "x") and a2(_, "y") and a2(_, "y") with a1.stmt# = 1 and p.procName = "proc" and p.procName = "proc"
*/

TEST(QueryParserTest, DuplicateComplexValidParseTest) {
  std::vector<Token *> tokens =
      {new KeywordToken("procedure"), new SymbolToken("p"), new SemicolonToken(), new KeywordToken("assign"),
       new SymbolToken("a1"), new CommaToken(), new SymbolToken("a2"), new SemicolonToken(),
       new KeywordToken("Select"), new SymbolToken("p"), new SymbolToken("such"), new SymbolToken("that"),
       new KeywordToken("Modifies"), new RoundOpenBracketToken(),
       new SymbolToken("p"), new CommaToken(), new QuoteToken(), new SymbolToken("x"), new QuoteToken(),
       new RoundCloseBracketToken(), new SymbolToken("and"), new KeywordToken("Parent"), new RoundOpenBracketToken(),
       new LiteralToken("7"), new CommaToken(), new LiteralToken("2"), new RoundCloseBracketToken(),
          // duplicate
       new SymbolToken("and"), new KeywordToken("Parent"), new RoundOpenBracketToken(),
       new LiteralToken("7"), new CommaToken(), new LiteralToken("2"), new RoundCloseBracketToken(),
       new SymbolToken("and"), new KeywordToken("Parent"), new OperatorToken("*"), new RoundOpenBracketToken(),
       new LiteralToken("2"), new CommaToken(), new LiteralToken("7"), new RoundCloseBracketToken(),
          // duplicate
       new SymbolToken("and"), new KeywordToken("Parent"), new OperatorToken("*"), new RoundOpenBracketToken(),
       new LiteralToken("2"), new CommaToken(), new LiteralToken("7"), new RoundCloseBracketToken(),
       new KeywordToken("pattern"), new SymbolToken("a1"), new RoundOpenBracketToken(), new WildCardToken(),
       new CommaToken(), new QuoteToken(), new SymbolToken("x"), new QuoteToken(), new RoundCloseBracketToken(),
       new KeywordToken("and"), new SymbolToken("a2"), new RoundOpenBracketToken(), new WildCardToken(),
       new CommaToken(), new QuoteToken(), new SymbolToken("y"), new QuoteToken(), new RoundCloseBracketToken(),
       // duplicate
       new KeywordToken("and"), new SymbolToken("a2"), new RoundOpenBracketToken(), new WildCardToken(),
       new CommaToken(), new QuoteToken(), new SymbolToken("y"), new QuoteToken(), new RoundCloseBracketToken(),
       new SymbolToken("with"),
       new SymbolToken("a1"), new DotToken(), new SymbolToken("stmt"), new HashtagToken(), new ComparatorToken("="),
       new LiteralToken("1"), new SymbolToken("and"), new SymbolToken("p"), new DotToken(), new SymbolToken("procName"),
       new ComparatorToken("="),
       new QuoteToken(), new SymbolToken("proc"), new QuoteToken(),
       // duplicate
       new SymbolToken("and"), new SymbolToken("p"), new DotToken(), new SymbolToken("procName"),
       new ComparatorToken("="),
       new QuoteToken(), new SymbolToken("proc"), new QuoteToken(),
       new EndOfFileToken()};

  auto *synonym_1 = new QuerySynonym("p", EntityType::kProcedure);
  auto *synonym_2 = new QuerySynonym("a1", EntityType::kAssignStmt);
  auto *synonym_3 = new QuerySynonym("a2", EntityType::kAssignStmt);
  auto *synonym_reference_1 = new SynonymReference(synonym_1);
  auto *synonym_reference_2 = new SynonymReference(synonym_2);
  auto *synonym_reference_3 = new SynonymReference(synonym_3);
  auto *attr_ref_1 = new AttrReference(synonym_1, AttributeType::kProcName);
  auto *attr_ref_2 = new AttrReference(synonym_2, AttributeType::kStmtNo);
  auto *ident_reference_1 = new IdentReference("x");
  auto *ident_reference_2 = new IdentReference("proc");
  auto *int_reference_1 = new IntegerReference("7");
  auto *int_reference_2 = new IntegerReference("2");
  auto *int_reference_3 = new IntegerReference("1");
  auto *wildcard_reference = new WildcardReference(EntityType::kVariable);
  SynonymSet declarations = {synonym_1, synonym_2, synonym_3};
  ClauseVector clauses = {new ModifiesClause(synonym_reference_1, ident_reference_1), new ParentClause(int_reference_1, int_reference_2),
                          new ParentTClause(int_reference_2, int_reference_1),
                          new PatternClause(synonym_reference_2, wildcard_reference, new ExactExpression("(x)"),
                                            nullptr),
                          new PatternClause(synonym_reference_3, wildcard_reference, new ExactExpression("(y)"),
                                            nullptr),
                          new WithClause(Comparator::kEquals, attr_ref_2, int_reference_3),
                          new WithClause(Comparator::kEquals, attr_ref_1, ident_reference_2)};
  QueryParser parser = QueryParser(tokens);
  auto *query = parser.Parse();
  auto *expected = new Query(declarations, new ElemSelect({new AttrReference(synonym_1, AttributeType::kNone)}), clauses);
  ASSERT_EQ(query->toString(), expected->toString());
}
