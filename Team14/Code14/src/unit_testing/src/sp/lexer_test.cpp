#include <gtest/gtest.h>

#include "commons/lexer/token.h"
#include "sp/simple_lexer.h"

TEST(LexerTest, TestBasic) {
  auto *s = new std::ifstream("Team14/Tests14/simple_code/basic_assign.txt");
  SimpleLexer simple_lexer(s);

  std::vector<Token *> tokens = simple_lexer.lex();
  std::vector<Token *> expected = {
      new KeywordToken("procedure"), new SymbolToken("main"),
      new CurlyOpenBracketToken(), new SymbolToken("x"),
      new OperatorToken("="), new LiteralToken("1"),
      new SemicolonToken(), new CurlyCloseBracket(),
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