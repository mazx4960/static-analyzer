#pragma once

#include <utility>

#include "commons/parser/grammar/grammar.h"

using ConditionalRule = std::pair<bool (*)(TokenIterator), GrammarRuleProducer *>;

class EarlyChoiceGrammarRule : public GrammarRule {
 private:
  std::vector<ConditionalRule> conditional_rules_;

 public:
  explicit EarlyChoiceGrammarRule(std::vector<ConditionalRule> conditionalRules) : conditional_rules_(std::move(conditionalRules)) {
  };
  Node *parseNode(TokenIterator &tokenStream) override;
};