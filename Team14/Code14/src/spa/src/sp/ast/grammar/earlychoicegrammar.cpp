#include "earlychoicegrammar.h"
#include "commons/parser/parser_exceptions.h"

EarlyChoiceGrammarRule::EarlyChoiceGrammarRule(std::vector<ConditionalRule> conditionalRules)
    : conditionalRules_(std::move(conditionalRules)) {}
SimpleAstNode* EarlyChoiceGrammarRule::parseNode(TokenIterator& tokenStream) {
  for (auto [condition, rule] : conditionalRules_) {
    if (condition(tokenStream)) {
      return rule->parseNode(tokenStream);
    }
  }
  throw ParseSyntaxError("Could not instantiate new node at " + (*tokenStream)->value);
}
