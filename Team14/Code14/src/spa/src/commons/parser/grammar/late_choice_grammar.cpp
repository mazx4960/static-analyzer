//
// Created by gabri on 4/9/2022.
//

#include "late_choice_grammar.h"

#include <utility>

#include "commons/parser/parser_exceptions.h"
LateChoiceGrammarRule::LateChoiceGrammarRule(GrammarRuleProducer* halfGrammarRuleProducer,
                                             std::vector<std::pair<Token*, MergeFunction>> mergeRules)
    : half_grammar_rule_producer_(halfGrammarRuleProducer),
      merge_rules_(std::move(mergeRules)) {}

Node* LateChoiceGrammarRule::parseNode(TokenIterator& tokenStream) {
  GrammarRule* half_grammar_rule = half_grammar_rule_producer_->produce();
  Node* first_node = half_grammar_rule->parseNode(tokenStream);
  MergeFunction merge_function_to_use = nullptr;
  for (auto [token, merge_function] : merge_rules_) {
    if (**tokenStream == *token) {
      tokenStream++;
      merge_function_to_use = merge_function;
      break;
    }
  }
  if (merge_function_to_use == nullptr) {
    throw ParseSyntaxError("Invalid token for late choice grammar: " + (*tokenStream)->value);
  }
  Node* second_node = half_grammar_rule->parseNode(tokenStream);
  return merge_function_to_use(first_node, second_node);
}
