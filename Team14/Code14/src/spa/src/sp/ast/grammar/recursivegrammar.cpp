#include "recursivegrammar.h"

#include <utility>

RecursiveGrammarRule::RecursiveGrammarRule(GrammarRuleProducer* baseRuleProducer, std::vector<std::pair<Token*, MergeFunction>> mergeRules)
    : baseRuleProducer_(baseRuleProducer), mergeRules_(std::move(mergeRules)) {}

SimpleAstNode* RecursiveGrammarRule::parseNode(TokenIterator &tokenStream) {
  SimpleGrammarRule* base_rule = baseRuleProducer_->produce();
  SimpleAstNode* node = base_rule->parseNode(tokenStream);
  while (true) {
    for (auto [token, mergeFunction] : mergeRules_) {
      if (**tokenStream == *token) {
        tokenStream++;
        SimpleAstNode* next_node = base_rule->parseNode(tokenStream);
        node = mergeFunction(node, next_node);
        continue;
      }
    }
    return node;
  }
}