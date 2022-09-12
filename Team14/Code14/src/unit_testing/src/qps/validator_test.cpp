#include <gtest/gtest.h>

#include <algorithm>

#include "qps/pql/query_call.h"
#include "qps/query_parser.h"

TEST(ValidatorTest, QueryValidatorTest) {
  std::vector<Token *> input_tokens_short = {new KeywordToken("assign"), new SymbolToken("v1"), new SemicolonToken(),
                                             new KeywordToken("assign"), new SymbolToken("v2"), new SemicolonToken(),
                                             new KeywordToken("Select"), new SymbolToken("v1"), new EndOfFileToken()};
  QueryParser short_validator = QueryParser(input_tokens_short);
  short_validator.parse();

  ASSERT_EQ(short_validator.getDeclarations()[0]->getSynonym(), QuerySynonym("v1"));
  ASSERT_EQ(short_validator.getDeclarations()[1]->getSynonym(), QuerySynonym("v2"));

  std::vector<Token *> input_tokens_long = {
      new KeywordToken("assign"), new SymbolToken("v1"),   new SemicolonToken(),       new KeywordToken("assign"),
      new SymbolToken("v2"),      new SemicolonToken(),    new KeywordToken("Select"), new SymbolToken("v1"),
      new SymbolToken("such"),    new SymbolToken("that"), new KeywordToken("Parent"), new RoundOpenBracketToken(),
      new SymbolToken("v1"),      new CommaToken(),        new SymbolToken("v2"),      new RoundCloseBracketToken(),
      new EndOfFileToken()};
  QueryParser long_validator = QueryParser(input_tokens_long);
  long_validator.parse();

  ASSERT_EQ(long_validator.getDeclarations()[0]->getSynonym(), QuerySynonym("v1"));
  ASSERT_EQ(long_validator.getDeclarations()[1]->getSynonym(), QuerySynonym("v2"));
}

TEST(ValidatorTest, InvalidTokenTest1) {
  std::vector<Token *> invalid_tokens = {new KeywordToken("var"),    new SymbolToken("v1"), new SemicolonToken(),
                                         new KeywordToken("assign"), new SymbolToken("v2"), new SemicolonToken(),
                                         new KeywordToken("Select"), new SymbolToken("v1"), new EndOfFileToken()};
  QueryParser validator = QueryParser(invalid_tokens);
  try {
    validator.parse();
    FAIL();
  } catch (const ParseSyntaxError &expected) {}
}

TEST(ValidatorTest, InvalidTokenTest2) {
  std::vector<Token *> invalid_tokens = {new KeywordToken("assign"), new SymbolToken("v1"), new SemicolonToken(),
                                         new KeywordToken("assign"), new SymbolToken("v1"), new SemicolonToken(),
                                         new KeywordToken("Select"), new SymbolToken("v1"), new EndOfFileToken()};
  QueryParser validator = QueryParser(invalid_tokens);
  try {
    validator.parse();
    FAIL();
  } catch (const ParseSemanticError &expected) {}
}
