#include <gtest/gtest.h>

#include "sp/lexer.h"
#include "sp/exceptions.h"

TEST(LexerTest, TestBasic) {
  auto* s = new std::ifstream("tests/simple_code/basic.txt");
  Lexer lexer(s);

  std::vector<Token*> tokens = lexer.lex();
  std::vector<Token*> expected = {
    new KeywordToken("procedure"),
    new SymbolToken("main"),
    new SeparatorToken("{"),
    new SymbolToken("x"),
    new OperatorToken("="),
    new LiteralToken("1"),
    new SeparatorToken(";"),
    new SeparatorToken("}"),
    new EndOfFileToken()
  };
  ASSERT_EQ(tokens.size(), expected.size());
  for (int i = 0; i < tokens.size(); ++i) {
    EXPECT_EQ(tokens[i]->type, expected[i]->type) << "Token " << i << " is not the same";
    EXPECT_EQ(tokens[i]->value, expected[i]->value) << "Value " << i << " is not the same";
  }
  delete s;
}