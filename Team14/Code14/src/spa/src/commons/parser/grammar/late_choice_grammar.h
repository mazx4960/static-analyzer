#pragma once

#include "commons/parser/grammar/grammar.h"

class LateChoiceGrammarRule : public GrammarRule {
  GrammarRuleProducer *half_grammar_rule_producer_;

  std::vector<std::pair<Token *, MergeFunction>> merge_rules_;

 public:
  LateChoiceGrammarRule(GrammarRuleProducer *halfGrammarRuleProducer,
                        std::vector<std::pair<Token *, MergeFunction>> mergeRules);
  Node *parseNode(TokenIterator &tokenStream) override;
};