#include "recursivegrammar.h"

#include <utility>

RecursiveGrammarRule::RecursiveGrammarRule(GrammarRuleProducer* baseRuleProducer, std::vector<std::pair<Token*, MergeFunction>> mergeRules)
    : base_rule_producer_(baseRuleProducer),
      merge_rules_(std::move(mergeRules)) {}

SimpleAstNode* RecursiveGrammarRule::parseNode(TokenIterator &tokenStream) {
  SimpleGrammarRule* base_rule = base_rule_producer_->produce();
  SimpleAstNode* node = base_rule->parseNode(tokenStream);
  while (true) {
    for (auto [token, mergeFunction] : merge_rules_) {
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