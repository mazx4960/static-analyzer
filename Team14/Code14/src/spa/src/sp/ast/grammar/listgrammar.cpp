#pragma once

#include "listgrammar.h"

ListGrammarRule::ListGrammarRule(SimpleGrammarRule *childRule)
    : childRule_(childRule) {}

SimpleAstNode *ListGrammarRule::parseNode(TokenIterator &tokenStream) {
  std::vector<SimpleAstNode*> children{};
  do {
    children.push_back(childRule_->parseNode(tokenStream));
  } while (!shouldStop(tokenStream));
  return assembleNode(children);
}