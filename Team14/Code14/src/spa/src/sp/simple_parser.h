// Copyright 2022 CS3203 Team14. All rights reserved.

#pragma once

#include "commons/lexer/token.h"
#include "commons/parser/parser.h"

class SimpleParser : public Parser {
 public:
  static Node *ParseProgram(const std::vector<Token *> &tokens);
};
