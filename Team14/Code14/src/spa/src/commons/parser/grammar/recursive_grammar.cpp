#include "recursive_grammar.h"

Node *RecursiveGrammarRule::parseNode(TokenIterator &tokenStream) {
  GrammarRule *base_rule = base_rule_producer_->produce();
  Node *node = base_rule->parseNode(tokenStream);
  while (true) {
    bool match = false;
    for (auto [token, mergeFunction] : merge_rules_) {
      if (**tokenStream == *token) {
        tokenStream++;
        Node *next_node = base_rule->parseNode(tokenStream);
        node = mergeFunction(node, next_node);
        match = true;
        break;
      }
    }
    if (!match) {
      return node;
    }
  }
}