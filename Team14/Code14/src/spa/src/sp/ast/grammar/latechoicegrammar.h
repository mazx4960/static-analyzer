#pragma once

#include "basesimplegrammar.h"

class LateChoiceGrammarRule : public SimpleGrammarRule {
  SimpleGrammarRule*(*halfGrammarRuleProducer_)();
  std::vector<std::pair<Token*, MergeFunction>> mergeRules_;
 public:
  LateChoiceGrammarRule(SimpleGrammarRule*(*halfGrammarRuleProducer)(), std::vector<std::pair<Token*, MergeFunction>> mergeRules);
  SimpleAstNode* parseNode(TokenIterator& tokenStream) override;
};