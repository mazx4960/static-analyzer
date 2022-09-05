#pragma once

#include "base_simple_grammar.h"

class ListGrammarRule : public SimpleGrammarRule {
 private:
  GrammarRuleProducer* child_rule_producer_;
  virtual bool shouldStop(TokenIterator tokenStream) = 0;
  virtual SimpleAstNode* assembleNode(std::vector<SimpleAstNode*> children) = 0;

 public:
  explicit ListGrammarRule(GrammarRuleProducer* childRuleProducer);
  SimpleAstNode* parseNode(TokenIterator& tokenStream) override;
};