// Copyright 2022 CS3203 Team14. All rights reserved.

#pragma once

#include <utility>
#include <vector>

#include "commons/lexer/token.h"
#include "commons/parser/expr_definition/expr_grammar.h"
#include "commons/parser/grammar/grammar.h"
#include "commons/parser/node/node.h"

class Parser {
 public:
  static Node *Parse(GrammarRule *grammar_rule, std::vector<Token *> &tokens);
  static Node *ParseExpression(std::vector<Token *> &tokens);
};
