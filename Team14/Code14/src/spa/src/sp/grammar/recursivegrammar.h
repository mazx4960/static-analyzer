#pragma once

#include "basesimplegrammar.h"

class RecursiveGrammarRule : public SimpleGrammarRule {
 private:
  GrammarRuleProducer* base_rule_producer_;
  std::vector<std::pair<Token*, MergeFunction>> merge_rules_;
 public:
  RecursiveGrammarRule(GrammarRuleProducer* baseRuleProducer, std::vector<std::pair<Token*, MergeFunction>> mergeRules);
  SimpleAstNode* parseNode(TokenIterator& tokenStream) override;
};