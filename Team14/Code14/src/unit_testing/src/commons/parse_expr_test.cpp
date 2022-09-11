#include <gtest/gtest.h>

#include "commons/parser/parser.h"

TEST(ExprParserTest, Variable) {
  std::vector<Token *> tokens = {new SymbolToken("x")};
  Node *expr = Parser::ParseExpression(tokens);
  std::string actual = expr->ToString();
  std::string expected = "(x)";
  ASSERT_EQ(actual, expected);

  tokens = {new LiteralToken("11")};
  expr = Parser::ParseExpression(tokens);
  actual = expr->ToString();
  expected = "(11)";
  ASSERT_EQ(actual, expected);
}

TEST(ExprParserTest, Plus) {
  // x + 1
  std::vector<Token *> tokens = {new SymbolToken("x"), new OperatorToken("+"), new LiteralToken("1"),
                                 new EndOfFileToken()};
  Node *expr = Parser::ParseExpression(tokens);
  std::string actual = expr->ToString();
  std::string expected = "((x)+(1))";
  ASSERT_EQ(actual, expected);

  // x + y + z
  tokens = {new SymbolToken("x"),   new OperatorToken("+"), new SymbolToken("y"),
            new OperatorToken("+"), new SymbolToken("z"),   new EndOfFileToken()};
  expr = Parser::ParseExpression(tokens);
  actual = expr->ToString();
  expected = "(((x)+(y))+(z))";
  ASSERT_EQ(actual, expected);

  // x + (y + z)
  tokens = {new SymbolToken("x"),   new OperatorToken("+"), new RoundOpenBracketToken(),  new SymbolToken("y"),
            new OperatorToken("+"), new SymbolToken("z"),   new RoundCloseBracketToken(), new EndOfFileToken()};
  expr = Parser::ParseExpression(tokens);
  actual = expr->ToString();
  expected = "((x)+((y)+(z)))";
  ASSERT_EQ(actual, expected);
}

TEST(ExprParserTest, Minus) {
  // x - 1
  std::vector<Token *> tokens = {new SymbolToken("x"), new OperatorToken("-"), new LiteralToken("1"),
                                 new EndOfFileToken()};
  Node *expr = Parser::ParseExpression(tokens);
  std::string actual = expr->ToString();
  std::string expected = "((x)-(1))";
  ASSERT_EQ(actual, expected);

  // x - y - z
  tokens = {new SymbolToken("x"),   new OperatorToken("-"), new SymbolToken("y"),
            new OperatorToken("-"), new SymbolToken("z"),   new EndOfFileToken()};
  expr = Parser::ParseExpression(tokens);
  actual = expr->ToString();
  expected = "(((x)-(y))-(z))";
  ASSERT_EQ(actual, expected);

  // x - (y - z)
  tokens = {new SymbolToken("x"),   new OperatorToken("-"), new RoundOpenBracketToken(),  new SymbolToken("y"),
            new OperatorToken("-"), new SymbolToken("z"),   new RoundCloseBracketToken(), new EndOfFileToken()};
  expr = Parser::ParseExpression(tokens);
  actual = expr->ToString();
  expected = "((x)-((y)-(z)))";
  ASSERT_EQ(actual, expected);
}

TEST(ExprParserTest, Times) {
  // x * 1
  std::vector<Token *> tokens = {new SymbolToken("x"), new OperatorToken("*"), new LiteralToken("1"),
                                 new EndOfFileToken()};
  Node *expr = Parser::ParseExpression(tokens);
  std::string actual = expr->ToString();
  std::string expected = "((x)*(1))";
  ASSERT_EQ(actual, expected);

  // x * y * z
  tokens = {new SymbolToken("x"),   new OperatorToken("*"), new SymbolToken("y"),
            new OperatorToken("*"), new SymbolToken("z"),   new EndOfFileToken()};
  expr = Parser::ParseExpression(tokens);
  actual = expr->ToString();
  expected = "(((x)*(y))*(z))";
  ASSERT_EQ(actual, expected);

  // x * (y * z)
  tokens = {new SymbolToken("x"),   new OperatorToken("*"), new RoundOpenBracketToken(),  new SymbolToken("y"),
            new OperatorToken("*"), new SymbolToken("z"),   new RoundCloseBracketToken(), new EndOfFileToken()};
  expr = Parser::ParseExpression(tokens);
  actual = expr->ToString();
  expected = "((x)*((y)*(z)))";
  ASSERT_EQ(actual, expected);
}

TEST(ExprParserTest, Div) {
  // x / 1
  std::vector<Token *> tokens = {new SymbolToken("x"), new OperatorToken("/"), new LiteralToken("1"),
                                 new EndOfFileToken()};
  Node *expr = Parser::ParseExpression(tokens);
  std::string actual = expr->ToString();
  std::string expected = "((x)/(1))";
  ASSERT_EQ(actual, expected);

  // x / y / z
  tokens = {new SymbolToken("x"),   new OperatorToken("/"), new SymbolToken("y"),
            new OperatorToken("/"), new SymbolToken("z"),   new EndOfFileToken()};
  expr = Parser::ParseExpression(tokens);
  actual = expr->ToString();
  expected = "(((x)/(y))/(z))";
  ASSERT_EQ(actual, expected);

  // x / (y / z)
  tokens = {new SymbolToken("x"),   new OperatorToken("/"), new RoundOpenBracketToken(),  new SymbolToken("y"),
            new OperatorToken("/"), new SymbolToken("z"),   new RoundCloseBracketToken(), new EndOfFileToken()};
  expr = Parser::ParseExpression(tokens);
  actual = expr->ToString();
  expected = "((x)/((y)/(z)))";
  ASSERT_EQ(actual, expected);
}

TEST(ExprParserTest, Mod) {
  // x % 1
  std::vector<Token *> tokens = {new SymbolToken("x"), new OperatorToken("%"), new LiteralToken("1"),
                                 new EndOfFileToken()};
  Node *expr = Parser::ParseExpression(tokens);
  std::string actual = expr->ToString();
  std::string expected = "((x)%(1))";
  ASSERT_EQ(actual, expected);

  // x % y % z
  tokens = {new SymbolToken("x"),   new OperatorToken("%"), new SymbolToken("y"),
            new OperatorToken("%"), new SymbolToken("z"),   new EndOfFileToken()};
  expr = Parser::ParseExpression(tokens);
  actual = expr->ToString();
  expected = "(((x)%(y))%(z))";
  ASSERT_EQ(actual, expected);

  // x % (y % z)
  tokens = {new SymbolToken("x"),   new OperatorToken("%"), new RoundOpenBracketToken(),  new SymbolToken("y"),
            new OperatorToken("%"), new SymbolToken("z"),   new RoundCloseBracketToken(), new EndOfFileToken()};
  expr = Parser::ParseExpression(tokens);
  actual = expr->ToString();
  expected = "((x)%((y)%(z)))";
  ASSERT_EQ(actual, expected);
}

TEST(ExprParserTest, Mixed) {
  // x + (y + 1) / (z - 1)
  std::vector<Token *> tokens = {new SymbolToken("x"),         new OperatorToken("+"), new RoundOpenBracketToken(),
                                 new SymbolToken("y"),         new OperatorToken("+"), new LiteralToken("1"),
                                 new RoundCloseBracketToken(), new OperatorToken("/"), new RoundOpenBracketToken(),
                                 new SymbolToken("z"),         new OperatorToken("-"), new LiteralToken("1"),
                                 new RoundCloseBracketToken(), new EndOfFileToken()};
  Node *expr = Parser::ParseExpression(tokens);
  std::string actual = expr->ToString();
  std::string expected = "((x)+(((y)+(1))/((z)-(1))))";
  ASSERT_EQ(actual, expected);
}
