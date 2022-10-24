#pragma once

#include <utility>

#include "commons/parser/grammar/grammar.h"
#include "commons/parser/parser_exceptions.h"

class LateChoiceGrammarRule : public GrammarRule {
  GrammarRuleProducer *half_grammar_rule_producer_;

  std::vector<std::pair<Token *, MergeFunction>> merge_rules_;

 public:
  LateChoiceGrammarRule(GrammarRuleProducer *halfGrammarRuleProducer, std::vector<std::pair<Token *, MergeFunction>> mergeRules)
      : half_grammar_rule_producer_(halfGrammarRuleProducer), merge_rules_(std::move(mergeRules)) {
  };
  Node *parseNode(TokenIterator &tokenStream) override;
};