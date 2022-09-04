#pragma once

#include "basesimplegrammar.h"

class LateChoiceGrammarRule : public SimpleGrammarRule {
  GrammarRuleProducer* half_grammar_rule_producer_;
  std::vector<std::pair<Token*, MergeFunction>> merge_rules_;
 public:
  LateChoiceGrammarRule(GrammarRuleProducer* halfGrammarRuleProducer, std::vector<std::pair<Token*, MergeFunction>> mergeRules);
  SimpleAstNode* parseNode(TokenIterator& tokenStream) override;
};