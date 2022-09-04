#include "listgrammar.h"

ListGrammarRule::ListGrammarRule(GrammarRuleProducer* childRuleProducer)
    : childRuleProducer_(childRuleProducer) {}

SimpleAstNode *ListGrammarRule::parseNode(TokenIterator &tokenStream) {
  std::vector<SimpleAstNode*> children{};
  SimpleGrammarRule* child_rule = childRuleProducer_->produce();
  do {
    children.push_back(child_rule->parseNode(tokenStream));
  } while (!shouldStop(tokenStream));
  return assembleNode(children);
}