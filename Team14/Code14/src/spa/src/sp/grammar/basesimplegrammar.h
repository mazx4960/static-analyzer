#pragma once

#include "commons/lexer/token.h"
#include "sp/ast/simpleast.h"

using MergeFunction = SimpleAstNode*(*)(SimpleAstNode*, SimpleAstNode*);
using TokenIterator = std::vector<Token*>::iterator;

// Defines a rule that creates an AST node from a token stream
class SimpleGrammarRule {
 public:
  virtual SimpleAstNode* parseNode(TokenIterator& tokenStream) = 0;
};

// Necessary to avoid loops, and reduce unnecessary creational calls
class GrammarRuleProducer {
 public:
  virtual SimpleGrammarRule* produce() = 0;
};
