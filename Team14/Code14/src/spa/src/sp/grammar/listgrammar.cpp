#include "listgrammar.h"

ListGrammarRule::ListGrammarRule(GrammarRuleProducer* childRuleProducer)
    : child_rule_producer_(childRuleProducer) {}

SimpleAstNode *ListGrammarRule::parseNode(TokenIterator &tokenStream) {
  std::vector<SimpleAstNode*> children{};
  SimpleGrammarRule* child_rule = child_rule_producer_->produce();
  do {
    children.push_back(child_rule->parseNode(tokenStream));
  } while (!shouldStop(tokenStream));
  return assembleNode(children);
}