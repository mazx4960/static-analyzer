// Copyright 2022 CS3203 Team14. All rights reserved.

#include <gtest/gtest.h>

#include "commons/lexer/token.h"
#include "commons/reader.h"
#include "sp/simple_lexer.h"
#include "sp/simple_parser.h"

TEST(SemanticTest, TestSameProcedureName) {
  std::istream* s = StreamReader::GetStreamFromFile("../Tests14/simple_code/call/same_name.txt");
  SimpleLexer simple_lexer(s);

  std::vector<Token*> tokens = simple_lexer.lex();
  ASSERT_THROW(SimpleParser::ParseProgram(tokens), ParseSemanticError);
}

TEST(SemanticTest, TestInvalidCalls) {
  std::istream* s = StreamReader::GetStreamFromFile("../Tests14/simple_code/call/invalid_calls.txt");
  SimpleLexer simple_lexer(s);

  std::vector<Token*> tokens = simple_lexer.lex();
  ASSERT_THROW(SimpleParser::ParseProgram(tokens), ParseSemanticError);
}

TEST(SemanticTest, TestCylicChain) {
  std::istream* s = StreamReader::GetStreamFromFile("../Tests14/simple_code/call/cyclic.txt");
  SimpleLexer simple_lexer(s);

  std::vector<Token*> tokens = simple_lexer.lex();
  ASSERT_THROW(SimpleParser::ParseProgram(tokens), ParseSemanticError);
}

TEST(SemanticTest, TestRecurse) {
  std::istream* s = StreamReader::GetStreamFromFile("../Tests14/simple_code/call/cyclic2.txt");
  SimpleLexer simple_lexer(s);

  std::vector<Token*> tokens = simple_lexer.lex();
  ASSERT_THROW(SimpleParser::ParseProgram(tokens), ParseSemanticError);
}

TEST(SemanticTest, TestChain) {
  std::istream* s = StreamReader::GetStreamFromFile("../Tests14/simple_code/call/chain.txt");
  SimpleLexer simple_lexer(s);

  std::vector<Token*> tokens = simple_lexer.lex();
  SimpleParser::ParseProgram(tokens);
}