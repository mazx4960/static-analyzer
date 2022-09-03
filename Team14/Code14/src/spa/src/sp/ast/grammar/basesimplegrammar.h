#pragma once

#include "sp/ast/node/simpleast.h"
#include "commons/token/token.h"


using TokenIterator = std::vector<Token*>::iterator;

// Defines a rule that creates an AST node from a token stream
class SimpleGrammarRule {
 public:
  virtual SimpleAstNode* parseNode(TokenIterator& tokenStream) = 0;
};
