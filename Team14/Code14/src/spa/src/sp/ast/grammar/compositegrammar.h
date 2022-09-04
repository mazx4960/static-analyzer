#pragma once

#include "commons/token/token.h"
#include "basesimplegrammar.h"

class RuleFragment {
 public:
  virtual void parseStream(TokenIterator& stream, std::vector<SimpleAstNode*>& childNodes) = 0;
};

class TokenRuleFragment : public RuleFragment {
 private:
  Token* tokenToMatch_;

 public:
  explicit TokenRuleFragment(Token* token);
  void parseStream(TokenIterator& stream, std::vector<SimpleAstNode*>& childNodes) override;
};

class NodeRuleFragment : public RuleFragment {
 private:
  SimpleGrammarRule* nextRule_;

 public:
  void parseStream(TokenIterator& stream, std::vector<SimpleAstNode*>& childNodes) override;
  explicit NodeRuleFragment(SimpleGrammarRule* rule);
};

// Defines a rule that reads multiple tokens and/or other AST nodes
class CompositeGrammarRule : public SimpleGrammarRule {
 private:
  std::vector<RuleFragment*> ruleFragments_;
  virtual SimpleAstNode* assembleNode(std::vector<SimpleAstNode*>) = 0;
 public:
  CompositeGrammarRule(std::vector<RuleFragment*> ruleFragments);
  SimpleAstNode* parseNode(TokenIterator& tokenStream) override;
};

class ParenthesizedGrammarRule : public CompositeGrammarRule {
 private:
  SimpleGrammarRule* parenthesizedNode_;
  SimpleAstNode* assembleNode(std::vector<SimpleAstNode*>) override;
 public:
  explicit ParenthesizedGrammarRule(SimpleGrammarRule* parenthesizedNode);
};

class BracedGrammarRule : public CompositeGrammarRule {
 private:
  SimpleGrammarRule* parenthesizedNode_;
  SimpleAstNode* assembleNode(std::vector<SimpleAstNode*>) override;
 public:
  explicit BracedGrammarRule(SimpleGrammarRule* parenthesizedNode);
};