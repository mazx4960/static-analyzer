#pragma once

#include "basesimplegrammar.h"

class ListGrammarRule : public SimpleGrammarRule {
 private:
  SimpleGrammarRule* childRule_;
  virtual bool shouldStop(TokenIterator tokenStream) = 0;
  virtual SimpleAstNode* assembleNode(std::vector<SimpleAstNode*> children) = 0;
 public:
  ListGrammarRule(SimpleGrammarRule* childRule);
  SimpleAstNode* parseNode(TokenIterator &tokenStream) override;
};