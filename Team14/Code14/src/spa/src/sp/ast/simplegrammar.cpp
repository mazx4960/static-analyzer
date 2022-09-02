#include "simplegrammar.h"
#include "token.h"
#include "exceptions.h"

CompositeGrammarRule::CompositeGrammarRule(std::vector<RuleFragment*> ruleFragments)
    : ruleFragments(ruleFragments) {}

SimpleAstNode* CompositeGrammarRule::parseNode(TokenIterator& tokenStream) {
  std::vector<SimpleAstNode*> childNodes{};
  for (auto ruleFragment : ruleFragments) {
    ruleFragment->parseStream(tokenStream, childNodes);
  }
  return assembleNode(childNodes);
}

ProgramNode* ProgramGrammarRule::parseNode(TokenIterator& tokenStream) {
  std::vector<ProcedureNode*> procedures{};
  ProcedureGrammarRule procedureParser{};
  procedures.push_back(procedureParser.parseNode(tokenStream));
  while (*tokenStream != EndOfFileToken()) {
    procedures.push_back(procedureParser.parseNode(tokenStream));
  }
  return new ProgramNode(procedures);
}

ProcedureGrammarRule::ProcedureGrammarRule()
    : CompositeGrammarRule(
        new TokenRuleFragment(new Token(TokenType.KEYWORD, "procedure")),
        new NodeRuleFragment(new VariableGrammarRule()),
        new TokenRuleFragment(new Token(TokenType.SEPARATOR, "{")),
        new NodeRuleFragment(new StatementListGrammarRule()),
        new TokenRuleFragment(new Token(TokenType.SEPARATOR, "{"))) {}

ProcedureNode* ProcedureGrammarRule::assembleNode(std::vector<SimpleAstNode*> childNodes) {
  return new ProcedureNode(
      ((VariableNode*) childNodes[0])->getName(),
      (StatementListNode*) childNodes[1]);
}