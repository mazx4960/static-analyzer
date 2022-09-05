#include "compositegrammar.h"
#include "commons/parser/parser_exceptions.h"

#include <utility>

TokenRuleFragment::TokenRuleFragment(Token* token) : token_to_match_(std::move(token)) {}

void TokenRuleFragment::parseStream(TokenIterator& stream, std::vector<SimpleAstNode*>&  /*childNodes*/) {
  if (!(**stream == *token_to_match_)) {
    throw ParseSyntaxError("Expected " + token_to_match_->value + ", got " + (*stream)->value);
  }
  ++stream;
}

NodeRuleFragment::NodeRuleFragment(GrammarRuleProducer* ruleProducer) : next_rule_producer_(ruleProducer) {}

void NodeRuleFragment::parseStream(TokenIterator& stream, std::vector<SimpleAstNode*>& childNodes) {
  childNodes.push_back(next_rule_producer_->produce()->parseNode(stream));
}

CompositeGrammarRule::CompositeGrammarRule(std::vector<RuleFragment*> ruleFragments)
    : rule_fragments_(std::move(ruleFragments)) {}

SimpleAstNode* CompositeGrammarRule::parseNode(TokenIterator& tokenStream) {
  std::vector<SimpleAstNode*> child_nodes{};
  for (auto *rule_fragment : rule_fragments_) {
    rule_fragment->parseStream(tokenStream, child_nodes);
  }
  return assembleNode(child_nodes);
}

ParenthesizedGrammarRule::ParenthesizedGrammarRule(GrammarRuleProducer* parenthesizedNode)
    : CompositeGrammarRule(std::vector<RuleFragment*>{
        new TokenRuleFragment(new RoundOpenBracketToken()),
        new NodeRuleFragment(parenthesizedNode),
        new TokenRuleFragment(new RoundCloseBracketToken()),
    }) {}

SimpleAstNode* ParenthesizedGrammarRule::assembleNode(std::vector<SimpleAstNode*> children) {
  return children[0];
}

BracedGrammarRule::BracedGrammarRule(GrammarRuleProducer* parenthesizedNode)
    : CompositeGrammarRule(std::vector<RuleFragment*>{
        new TokenRuleFragment(new CurlyOpenBracketToken()),
        new NodeRuleFragment(parenthesizedNode),
        new TokenRuleFragment(new CurlyCloseBracket()),
    }) {}

SimpleAstNode* BracedGrammarRule::assembleNode(std::vector<SimpleAstNode*> children) {
  return children[0];
}