#pragma once

#include "basesimplegrammar.h"

using ConditionalRule = std::pair<bool(*)(TokenIterator), SimpleGrammarRule*>;

class EarlyChoiceGrammarRule : public SimpleGrammarRule {
 private:
  std::vector<ConditionalRule> conditionalRules_;
 public:
  explicit EarlyChoiceGrammarRule(std::vector<ConditionalRule> conditionalRules);
  SimpleAstNode* parseNode(TokenIterator &tokenStream) override;
};