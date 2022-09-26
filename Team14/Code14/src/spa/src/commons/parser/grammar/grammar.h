// Copyright 2022 CS3203 Team14. All rights reserved.

#pragma once

#include "commons/lexer/token.h"
#include "commons/parser/node/node.h"

using MergeFunction = Node *(*)(Node *, Node *);
using TokenIterator = std::vector<Token *>::iterator;

// Defines a rule that creates an AST node from a token stream
class GrammarRule {
 public:
  virtual Node *parseNode(TokenIterator &tokenStream) = 0;
};

// Necessary to avoid loops, and reduce unnecessary creation calls
class GrammarRuleProducer {
 public:
  virtual GrammarRule *produce() = 0;
};
