#include "simplegrammar.h"

#include <utility>
#include "commons/token/token.h"

SimpleAstNode* CompositeGrammarRule::parseNode(TokenIterator& tokenStream) {
  std::vector<SimpleAstNode*> child_nodes{};
  for (auto *rule_fragment : ruleFragments_) {
    rule_fragment->parseStream(tokenStream, child_nodes);
  }
  return assembleNode(child_nodes);
}

ProgramNode* ProgramGrammarRule::parseNode(TokenIterator& tokenStream) {
  std::vector<ProcedureNode*> procedures{};
  ProcedureGrammarRule procedure_parser{};
  procedures.push_back(procedure_parser.parseNode(tokenStream));
  while (!(**tokenStream == EndOfFileToken())) {
    procedures.push_back(procedure_parser.parseNode(tokenStream));
  }
  return new ProgramNode(procedures);
}

ProcedureGrammarRule::ProcedureGrammarRule()
    : CompositeGrammarRule(std::vector<RuleFragment*>{
        new TokenRuleFragment(new LiteralToken("procedure")),
        new NodeRuleFragment(new VariableGrammarRule()),
        new TokenRuleFragment(new RoundOpenBracketToken()),
        new NodeRuleFragment(new StatementGrammarRule()),
        new TokenRuleFragment(new RoundCloseBracketToken())}) {}

ProcedureNode* ProcedureGrammarRule::assembleNode(std::vector<SimpleAstNode*> childNodes) {
  return new ProcedureNode(
      ((VariableNode*) childNodes[0])->getName(),
      (StatementListNode*) childNodes[1]);
}