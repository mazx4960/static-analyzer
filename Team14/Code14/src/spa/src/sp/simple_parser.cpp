// Copyright 2022 CS3203 Team14. All rights reserved.

#include "simple_parser.h"

#include <utility>

#include "sp/simple_definition/simple_grammar.h"

Node* SimpleParser::ParseProgram(const std::vector<Token*>& tokens) {
  return Parser::Parse(new ProgramGrammarRule(), std::move(tokens));
}
