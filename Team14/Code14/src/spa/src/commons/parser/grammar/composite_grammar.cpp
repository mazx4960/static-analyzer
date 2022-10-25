#include "composite_grammar.h"

#include <utility>

#include "commons/parser/parser_exceptions.h"

TokenRuleFragment::TokenRuleFragment(Token *token) : token_to_match_(std::move(token)) {
}

void TokenRuleFragment::parseStream(TokenIterator &stream, std::vector<Node *> & /*childNodes*/) {
  if (!(**stream == *token_to_match_)) {
    throw ParseSyntaxError("Expected " + token_to_match_->value + ", got " + (*stream)->value);
  }
  ++stream;
}

NodeRuleFragment::NodeRuleFragment(GrammarRuleProducer *ruleProducer) : next_rule_producer_(ruleProducer) {
}

void NodeRuleFragment::parseStream(TokenIterator &stream, std::vector<Node *> &childNodes) {
  childNodes.push_back(next_rule_producer_->produce()->parseNode(stream));
}

CompositeGrammarRule::CompositeGrammarRule(std::vector<RuleFragment *> ruleFragments)
    : rule_fragments_(std::move(ruleFragments)) {
}

Node *CompositeGrammarRule::parseNode(TokenIterator &tokenStream) {
  std::vector<Node *> child_nodes{};
  for (auto *rule_fragment : rule_fragments_) {
    rule_fragment->parseStream(tokenStream, child_nodes);
  }
  return assembleNode(child_nodes);
}

ParenthesizedGrammarRule::ParenthesizedGrammarRule(GrammarRuleProducer *parenthesizedNode)
    : CompositeGrammarRule(std::vector<RuleFragment *>{
    new TokenRuleFragment(new RoundOpenBracketToken()),
    new NodeRuleFragment(parenthesizedNode),
    new TokenRuleFragment(new RoundCloseBracketToken()),
}) {
}

Node *ParenthesizedGrammarRule::assembleNode(std::vector<Node *> &children) {
  return children[0];
}

BracedGrammarRule::BracedGrammarRule(GrammarRuleProducer *parenthesizedNode)
    : CompositeGrammarRule(std::vector<RuleFragment *>{
    new TokenRuleFragment(new CurlyOpenBracketToken()),
    new NodeRuleFragment(parenthesizedNode),
    new TokenRuleFragment(new CurlyCloseBracketToken()),
}) {
}

Node *BracedGrammarRule::assembleNode(std::vector<Node *> &children) {
  return children[0];
}