//
// Created by gabri on 4/9/2022.
//

#include "latechoicegrammar.h"
#include "commons/parser/parser_exceptions.h"

#include <utility>
LateChoiceGrammarRule::LateChoiceGrammarRule(GrammarRuleProducer* halfGrammarRuleProducer, std::vector<std::pair<Token*, MergeFunction>> mergeRules)
    : halfGrammarRuleProducer_(halfGrammarRuleProducer), mergeRules_(std::move(mergeRules)) {}

SimpleAstNode* LateChoiceGrammarRule::parseNode(TokenIterator& tokenStream) {
  SimpleGrammarRule* half_grammar_rule = halfGrammarRuleProducer_->produce();
  SimpleAstNode* first_node = half_grammar_rule->parseNode(tokenStream);
  MergeFunction merge_function_to_use = nullptr;
  for (auto [token, merge_function] : mergeRules_) {
    if (**tokenStream == *token) {
      merge_function_to_use = merge_function;
      break;
    }
  }
  if (merge_function_to_use == nullptr) {
    throw ParseSyntaxError("Invalid token for late choice grammar: " + (*tokenStream)->value);
  }
  SimpleAstNode* second_node = half_grammar_rule->parseNode(tokenStream);
  return merge_function_to_use(first_node, second_node);
}
