#include <gtest/gtest.h>

#include "commons/lexer/token.h"
#include "commons/reader.h"
#include "qps/query_lexer.h"

TEST(QueryLexerTest, TestBasic) {
  std::istream* s = StreamReader::GetStreamFromFile("../Tests14/pql_code/basic.txt");
  QueryLexer lexer(s);

  std::vector<Token*> tokens = lexer.lex();
  std::vector<Token*> expected = {new KeywordToken("variable"), new SymbolToken("a"), new SemicolonToken(),
                                  new KeywordToken("Select"),   new SymbolToken("a"), new EndOfFileToken()};
  ASSERT_EQ(tokens.size(), expected.size());
  for (int i = 0; i < tokens.size(); ++i) {
    EXPECT_EQ(tokens[i]->type, expected[i]->type) << "Token " << i << " is not the same";
    EXPECT_EQ(tokens[i]->value, expected[i]->value) << "Value " << i << " is not the same";
  }
  delete s;
}