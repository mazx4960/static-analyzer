#include <gtest/gtest.h>

#include "commons/lexer/token.h"
#include "commons/reader.h"
#include "sp/simple_lexer.h"

TEST(LexerTest, TestBasic) {
  std::istream* s = StreamReader::GetStreamFromFile("Team14/Tests14/simple_code/basic.txt");
  SimpleLexer simple_lexer(s);

  std::vector<Token*> tokens = simple_lexer.lex();
  std::vector<Token*> expected = {new KeywordToken("procedure"), new SymbolToken("main"), new CurlyOpenBracketToken(),
                                  new SymbolToken("x"),          new OperatorToken("="),  new SymbolToken("x"),
                                  new OperatorToken("+"),        new LiteralToken("1"),   new SemicolonToken(),
                                  new CurlyCloseBracketToken(),  new EndOfFileToken()};
  ASSERT_EQ(tokens.size(), expected.size());
  for (int i = 0; i < tokens.size(); ++i) {
    EXPECT_EQ(tokens[i]->type, expected[i]->type) << "Token " << i << " is not the same";
    EXPECT_EQ(tokens[i]->value, expected[i]->value) << "Value " << i << " is not the same";
  }
  delete s;
}