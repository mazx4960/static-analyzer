#include <gtest/gtest.h>

#include "commons/lexer/lexer_exceptions.h"
#include "commons/lexer/token.h"
#include "commons/reader.h"
#include "qps/query_lexer.h"

TEST(QueryLexerTest, TestBasic) {
  const std::string query_string = "variable a;\nSelect a";
  std::istream *s = StreamReader::GetStreamFromString(query_string);
  QueryLexer lexer(s);

  std::vector<Token *> tokens = lexer.Lex();
  std::vector<Token *> expected = {new KeywordToken("variable"), new SymbolToken("a"), new SemicolonToken(),
                                   new KeywordToken("Select"), new SymbolToken("a"), new EndOfFileToken()};
  ASSERT_EQ(tokens.size(), expected.size());
  for (int i = 0; i < tokens.size(); ++i) {
    EXPECT_EQ(tokens[i]->type, expected[i]->type) << "Token " << i << " is not the same";
    EXPECT_EQ(tokens[i]->value, expected[i]->value) << "Value " << i << " is not the same";
  }
  delete s;
}

TEST(QueryLexerTest, InvalidNumeric) {

  const std::string query_string = "variable v;\n"
                                   "Select v such that Modifies(0123, v)";
  std::istream *s = StreamReader::GetStreamFromString(query_string);
  QueryLexer lexer(s);

  ASSERT_THROW(lexer.Lex(), LexSyntaxError);
  delete s;
}