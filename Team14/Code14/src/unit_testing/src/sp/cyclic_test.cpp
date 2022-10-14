// Copyright 2022 CS3203 Team14. All rights reserved.

#include <gtest/gtest.h>

#include "commons/lexer/token.h"
#include "commons/reader.h"
#include "sp/simple_lexer.h"
#include "sp/simple_parser.h"

TEST(CyclicTest, TestCylicChain) {
  std::istream* s = StreamReader::GetStreamFromFile("../Tests14/simple_code/call/cyclic.txt");
  SimpleLexer simple_lexer(s);

  std::vector<Token*> tokens = simple_lexer.lex();
  ASSERT_THROW(SimpleParser::ParseProgram(tokens), ParseSemanticError);
}

TEST(CyclicTest, TestRecurse) {
  std::istream* s = StreamReader::GetStreamFromFile("../Tests14/simple_code/call/cyclic2.txt");
  SimpleLexer simple_lexer(s);

  std::vector<Token*> tokens = simple_lexer.lex();
  ASSERT_THROW(SimpleParser::ParseProgram(tokens), ParseSemanticError);
}

TEST(CyclicTest, TestChain) {
  std::istream* s = StreamReader::GetStreamFromFile("../Tests14/simple_code/call/chain.txt");
  SimpleLexer simple_lexer(s);

  std::vector<Token*> tokens = simple_lexer.lex();
  auto* node = SimpleParser::ParseProgram(tokens);
  ASSERT_TRUE(SimpleParser::ValidateProgram(node));
}