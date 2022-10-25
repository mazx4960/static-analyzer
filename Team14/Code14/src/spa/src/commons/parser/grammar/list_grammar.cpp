#include "list_grammar.h"

Node *ListGrammarRule::parseNode(TokenIterator &tokenStream) {
  std::vector<Node *> children{};
  GrammarRule *child_rule = child_rule_producer_->produce();
  do {
    children.push_back(child_rule->parseNode(tokenStream));
  } while (!shouldStop(tokenStream));

  return assembleNode(children);
}