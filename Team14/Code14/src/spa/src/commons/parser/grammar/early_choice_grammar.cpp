#include "early_choice_grammar.h"

#include "commons/parser/parser_exceptions.h"

Node *EarlyChoiceGrammarRule::parseNode(TokenIterator &tokenStream) {
  for (auto [condition, rule] : conditional_rules_) {
    if (condition(tokenStream)) {
      return rule->produce()->parseNode(tokenStream);
    }
  }
  throw ParseSyntaxError("Could not instantiate new node at " + (*tokenStream)->value);
}
