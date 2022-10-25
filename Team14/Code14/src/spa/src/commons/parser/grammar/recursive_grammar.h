#pragma once

#include <utility>

#include "commons/parser/grammar/grammar.h"

class RecursiveGrammarRule : public GrammarRule {
 private:
  GrammarRuleProducer *base_rule_producer_;

  std::vector<std::pair<Token *, MergeFunction>> merge_rules_;

 public:
  RecursiveGrammarRule(GrammarRuleProducer *baseRuleProducer, std::vector<std::pair<Token *, MergeFunction>> mergeRules)
      : base_rule_producer_(baseRuleProducer), merge_rules_(std::move(mergeRules)) {
  };
  Node *parseNode(TokenIterator &tokenStream) override;
};