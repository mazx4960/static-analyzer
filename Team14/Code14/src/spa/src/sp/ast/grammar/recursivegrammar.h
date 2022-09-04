#pragma once

#include "basesimplegrammar.h"

class RecursiveGrammarRule : public SimpleGrammarRule {
 private:
  // Use a producer to avoid recursion
  SimpleGrammarRule*(*baseRuleProducer_)();
  std::vector<std::pair<Token*, MergeFunction>> mergeRules_;
 public:
  RecursiveGrammarRule(SimpleGrammarRule*(*baseRuleProducer)(), std::vector<std::pair<Token*, MergeFunction>> mergeRules);
  SimpleAstNode* parseNode(TokenIterator& tokenStream) override;
};