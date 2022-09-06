#include <gtest/gtest.h>

#include "commons/lexer/token.h"
#include "qps/query_parser/query_lexer.h"

TEST(QueryLexerTest, BasicAssign) {
  auto *s = new std::ifstream("Team14/Tests14/pql_code/basic_assign.txt");
  QueryLexer lexer(s);

  std::vector<Token *> tokens = lexer.lex();
  std::vector<Token *> expected = {
      new KeywordToken("assign"),
      new SymbolToken("a"),
      new SemicolonToken(),
      new KeywordToken("Select"),
      new SymbolToken("a"),
      new EndOfFileToken()
  };
  ASSERT_EQ(tokens.size(), expected.size());

  for (int i = 0; i < tokens.size(); ++i) {
    EXPECT_EQ(tokens[i]->type, expected[i]->type) << "Token " << i << " is not the same";
    EXPECT_EQ(tokens[i]->value, expected[i]->value) << "Value " << i << " is not the same";
  }
  delete s;
}

TEST(QueryLexerTest, BasicVariable) {
  auto *s = new std::ifstream("Team14/Tests14/pql_code/basic_variable.txt");
  QueryLexer lexer(s);

  std::vector<Token *> tokens = lexer.lex();
  std::vector<Token *> expected = {
      new KeywordToken("variable"),
      new SymbolToken("a"),
      new SemicolonToken(),
      new KeywordToken("Select"),
      new SymbolToken("a"),
      new EndOfFileToken()
  };
  ASSERT_EQ(tokens.size(), expected.size());

  for (int i = 0; i < tokens.size(); ++i) {
    EXPECT_EQ(tokens[i]->type, expected[i]->type) << "Token " << i << " is not the same";
    EXPECT_EQ(tokens[i]->value, expected[i]->value) << "Value " << i << " is not the same";
  }
  delete s;
}