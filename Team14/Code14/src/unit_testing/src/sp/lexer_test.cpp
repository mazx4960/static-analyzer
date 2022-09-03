#include "sp/simple_lexer.h"
#include "commons/token/token.h"

#include <gtest/gtest.h>

TEST(LexerTest, TestBasic) {
  auto* s = new std::ifstream("Team14/Tests14/simple_code/basic.txt");
  Lexer lexer(s);

  std::vector<Token*> tokens = lexer.lex();
  std::vector<Token*> expected = {
      new KeywordToken("procedure"), new SymbolToken("main"),
      new CurlyOpenBracketToken(),       new SymbolToken("x"),
      new OperatorToken("="),        new LiteralToken("1"),
      new SemicolonToken(),       new CurlyCloseBracket(),
      new EndOfFileToken()};
  ASSERT_EQ(tokens.size(), expected.size());
  for (int i = 0; i < tokens.size(); ++i) {
    EXPECT_EQ(tokens[i]->type, expected[i]->type)
        << "Token " << i << " is not the same";
    EXPECT_EQ(tokens[i]->value, expected[i]->value)
        << "Value " << i << " is not the same";
  }
  delete s;
}