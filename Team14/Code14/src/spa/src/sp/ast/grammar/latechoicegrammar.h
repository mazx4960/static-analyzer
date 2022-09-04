#pragma once

#include "basesimplegrammar.h"

class LateChoiceGrammarRule : public SimpleGrammarRule {
  GrammarRuleProducer* halfGrammarRuleProducer_;
  std::vector<std::pair<Token*, MergeFunction>> mergeRules_;
 public:
  LateChoiceGrammarRule(GrammarRuleProducer* halfGrammarRuleProducer, std::vector<std::pair<Token*, MergeFunction>> mergeRules);
  SimpleAstNode* parseNode(TokenIterator& tokenStream) override;
};