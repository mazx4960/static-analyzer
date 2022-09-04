#pragma once

#include "basesimplegrammar.h"

class ListGrammarRule : public SimpleGrammarRule {
 private:
  GrammarRuleProducer* childRuleProducer_;
  virtual bool shouldStop(TokenIterator tokenStream) = 0;
  virtual SimpleAstNode* assembleNode(std::vector<SimpleAstNode*> children) = 0;
 public:
  ListGrammarRule(GrammarRuleProducer* childRuleProducer);
  SimpleAstNode* parseNode(TokenIterator &tokenStream) override;
};