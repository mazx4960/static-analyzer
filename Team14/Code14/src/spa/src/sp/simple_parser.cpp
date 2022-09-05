// Copyright 2022 CS3203 Team14. All rights reserved.

#include "simple_parser.h"

#include <utility>

#include "sp/grammar/simple_grammar.h"

SimpleParser::SimpleParser(std::vector<Token*> tokens) {
  this->tokens_ = std::move(tokens);
}
SimpleAstNode* SimpleParser::Parse() {
  auto token_stream = this->tokens_.begin();
  return (new ProgramGrammarRule())->parseNode(token_stream);
}
