#pragma once

#include "commons/parser/grammar/grammar.h"

using ConditionalRule = std::pair<bool (*)(TokenIterator), GrammarRuleProducer *>;

class EarlyChoiceGrammarRule : public GrammarRule {
 private:
  std::vector<ConditionalRule> conditional_rules_;

 public:
  explicit EarlyChoiceGrammarRule(std::vector<ConditionalRule> conditionalRules);
  Node *parseNode(TokenIterator &tokenStream) override;
};