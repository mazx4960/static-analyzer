#pragma once

#include "basesimplegrammar.h"

using ConditionalRule = std::pair<bool(*)(TokenIterator), GrammarRuleProducer*>;

class EarlyChoiceGrammarRule : public SimpleGrammarRule {
 private:
  std::vector<ConditionalRule> conditional_rules_;
 public:
  explicit EarlyChoiceGrammarRule(std::vector<ConditionalRule> conditionalRules);
  SimpleAstNode* parseNode(TokenIterator &tokenStream) override;
};