#pragma once

#include "commons/parser/grammar/grammar.h"

class ListGrammarRule : public GrammarRule {
 private:
  GrammarRuleProducer* child_rule_producer_;
  virtual bool shouldStop(TokenIterator tokenStream) = 0;
  virtual Node* assembleNode(std::vector<Node*> children) = 0;

 public:
  explicit ListGrammarRule(GrammarRuleProducer* childRuleProducer);
  Node* parseNode(TokenIterator& tokenStream) override;
};