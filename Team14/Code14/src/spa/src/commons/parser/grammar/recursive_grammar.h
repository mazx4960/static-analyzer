#pragma once

#include "commons/parser/grammar/grammar.h"

class RecursiveGrammarRule : public GrammarRule {
 private:
  GrammarRuleProducer* base_rule_producer_;
  std::vector<std::pair<Token*, MergeFunction>> merge_rules_;

 public:
  RecursiveGrammarRule(GrammarRuleProducer* baseRuleProducer, std::vector<std::pair<Token*, MergeFunction>> mergeRules);
  Node* parseNode(TokenIterator& tokenStream) override;
};