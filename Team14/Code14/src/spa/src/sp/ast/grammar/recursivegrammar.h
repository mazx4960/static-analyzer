#pragma once

#include "basesimplegrammar.h"

class RecursiveGrammarRule : public SimpleGrammarRule {
 private:
  GrammarRuleProducer* baseRuleProducer_;
  std::vector<std::pair<Token*, MergeFunction>> mergeRules_;
 public:
  RecursiveGrammarRule(GrammarRuleProducer* baseRuleProducer, std::vector<std::pair<Token*, MergeFunction>> mergeRules);
  SimpleAstNode* parseNode(TokenIterator& tokenStream) override;
};