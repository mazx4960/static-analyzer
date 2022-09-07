#include "simple_grammar.h"

#include "commons/lexer/token.h"
#include "commons/parser/expr_definition/expr_grammar_producer.h"
#include "commons/parser/parser_exceptions.h"
#include "simple_grammar_producer.h"

ProgramGrammarRule::ProgramGrammarRule() : ListGrammarRule(new ProcedureGrammarProducer()) {}

ProgramNode* ProgramGrammarRule::assembleNode(std::vector<Node*> children) {
  std::vector<ProcedureNode*> procedures{};
  procedures.reserve(children.size());
  for (auto* child : children) { procedures.push_back(static_cast<ProcedureNode*>(child)); }
  return new ProgramNode(procedures);
}

bool ProgramGrammarRule::shouldStop(TokenIterator token_stream) { return **token_stream == EndOfFileToken(); }

ProcedureGrammarRule::ProcedureGrammarRule()
    : CompositeGrammarRule(std::vector<RuleFragment*>{
        new TokenRuleFragment(new KeywordToken("procedure")), new NodeRuleFragment(new VariableGrammarProducer()),
        new NodeRuleFragment(new BracedGrammarProducer(new StatementListGrammarProducer()))}) {}

ProcedureNode* ProcedureGrammarRule::assembleNode(std::vector<Node*> child_nodes) {
  return new ProcedureNode(static_cast<VariableNode*>(child_nodes[0])->GetVariableName(),
                           static_cast<StatementListNode*>(child_nodes[1]));
}

StatementListGrammarRule::StatementListGrammarRule() : ListGrammarRule(new StatementGrammarProducer()) {}

StatementListNode* StatementListGrammarRule::assembleNode(std::vector<Node*> children) {
  std::vector<StatementNode*> statements{};
  statements.reserve(children.size());
  for (auto* child : children) { statements.push_back(static_cast<StatementNode*>(child)); }
  return new StatementListNode(statements);
}

bool StatementListGrammarRule::shouldStop(TokenIterator token_stream) {
  return **token_stream == EndOfFileToken()
      || !(**token_stream == KeywordToken("read") || **token_stream == KeywordToken("print")
           || **token_stream == KeywordToken("call") || **token_stream == KeywordToken("while")
           || **token_stream == KeywordToken("if") || **(token_stream + 1) == OperatorToken("="));
}

StatementGrammarRule::StatementGrammarRule()
    : EarlyChoiceGrammarRule(std::vector<ConditionalRule>{
        std::make_pair(
            [](TokenIterator tokenStream) -> bool {
              return !(**tokenStream == EndOfFileToken()) && **(tokenStream + 1) == OperatorToken("=");
            },
            new AssignGrammarProducer()),
        std::make_pair([](TokenIterator tokenStream) -> bool { return **tokenStream == KeywordToken("read"); },
                       new ReadGrammarProducer()),
        std::make_pair([](TokenIterator tokenStream) -> bool { return **tokenStream == KeywordToken("print"); },
                       new PrintGrammarProducer()),
        std::make_pair([](TokenIterator tokenStream) -> bool { return **tokenStream == KeywordToken("call"); },
                       new CallGrammarProducer()),
        std::make_pair([](TokenIterator tokenStream) -> bool { return **tokenStream == KeywordToken("while"); },
                       new WhileGrammarProducer()),
        std::make_pair([](TokenIterator tokenStream) -> bool { return **tokenStream == KeywordToken("if"); },
                       new IfGrammarProducer()),
    }) {}

ReadGrammarRule::ReadGrammarRule()
    : CompositeGrammarRule(std::vector<RuleFragment*>{new TokenRuleFragment(new KeywordToken("read")),
                                                      new NodeRuleFragment(new VariableGrammarProducer()),
                                                      new TokenRuleFragment(new SemicolonToken())}) {}

ReadNode* ReadGrammarRule::assembleNode(std::vector<Node*> childNodes) {
  return new ReadNode(static_cast<VariableNode*>(childNodes[0]));
}

PrintGrammarRule::PrintGrammarRule()
    : CompositeGrammarRule(std::vector<RuleFragment*>{new TokenRuleFragment(new KeywordToken("print")),
                                                      new NodeRuleFragment(new VariableGrammarProducer()),
                                                      new TokenRuleFragment(new SemicolonToken())}) {}

PrintNode* PrintGrammarRule::assembleNode(std::vector<Node*> childNodes) {
  return new PrintNode(static_cast<VariableNode*>(childNodes[0]));
}

CallGrammarRule::CallGrammarRule()
    : CompositeGrammarRule(std::vector<RuleFragment*>{new TokenRuleFragment(new KeywordToken("call")),
                                                      new NodeRuleFragment(new VariableGrammarProducer()),
                                                      new TokenRuleFragment(new SemicolonToken())}) {}

CallNode* CallGrammarRule::assembleNode(std::vector<Node*> childNodes) {
  return new CallNode(static_cast<VariableNode*>(childNodes[0])->GetVariableName());
}

WhileGrammarRule::WhileGrammarRule()
    : CompositeGrammarRule(std::vector<RuleFragment*>{
        new TokenRuleFragment(new KeywordToken("while")),
        new NodeRuleFragment(new ParenthesizedGrammarProducer(new CondExprGrammarProducer())),
        new NodeRuleFragment(new BracedGrammarProducer(new StatementListGrammarProducer()))}) {}

WhileNode* WhileGrammarRule::assembleNode(std::vector<Node*> childNodes) {
  return new WhileNode(static_cast<CondExprNode*>(childNodes[0]), static_cast<StatementListNode*>(childNodes[1]));
}

IfGrammarRule::IfGrammarRule()
    : CompositeGrammarRule(std::vector<RuleFragment*>{
        new TokenRuleFragment(new KeywordToken("if")),
        new NodeRuleFragment(new ParenthesizedGrammarProducer(new CondExprGrammarProducer())),
        new TokenRuleFragment(new KeywordToken("then")),
        new NodeRuleFragment(new BracedGrammarProducer(new StatementListGrammarProducer())),
        new TokenRuleFragment(new KeywordToken("else")),
        new NodeRuleFragment(new BracedGrammarProducer(new StatementListGrammarProducer()))}) {}

IfNode* IfGrammarRule::assembleNode(std::vector<Node*> childNodes) {
  return new IfNode(static_cast<CondExprNode*>(childNodes[0]), static_cast<StatementListNode*>(childNodes[1]),
                    static_cast<StatementListNode*>(childNodes[2]));
}

AssignGrammarRule::AssignGrammarRule()
    : CompositeGrammarRule(std::vector<RuleFragment*>{
        new NodeRuleFragment(new VariableGrammarProducer()), new TokenRuleFragment(new OperatorToken("=")),
        new NodeRuleFragment(new RelFactorGrammarProducer()), new TokenRuleFragment(new SemicolonToken())}) {}

AssignNode* AssignGrammarRule::assembleNode(std::vector<Node*> childNodes) {
  return new AssignNode(static_cast<VariableNode*>(childNodes[0]), static_cast<RelFactorNode*>(childNodes[1]));
}

CondExprGrammarRule::CondExprGrammarRule()
    : EarlyChoiceGrammarRule(std::vector<ConditionalRule>{
        std::make_pair([](TokenIterator tokenStream) -> bool { return (**tokenStream).type == TokenType::kSymbol; },
                       new RelExprGrammarProducer()),
        std::make_pair([](TokenIterator tokenStream) -> bool { return **tokenStream == OperatorToken("!"); },
                       new NotExprGrammarProducer()),
        std::make_pair([](TokenIterator tokenStream) -> bool { return **tokenStream == RoundOpenBracketToken(); },
                       new BinaryCondGrammarProducer()),
    }) {}

NotExprGrammarRule::NotExprGrammarRule()
    : CompositeGrammarRule(std::vector<RuleFragment*>{
        new TokenRuleFragment(new OperatorToken("!")),
        new NodeRuleFragment(new ParenthesizedGrammarProducer(new CondExprGrammarProducer()))}) {}

NotExprNode* NotExprGrammarRule::assembleNode(std::vector<Node*> children) {
  return new NotExprNode(static_cast<CondExprNode*>(children[0]));
}

BinaryCondGrammarRule::BinaryCondGrammarRule()
    : LateChoiceGrammarRule(new ParenthesizedGrammarProducer(new CondExprGrammarProducer()),
                            std::vector<std::pair<Token*, MergeFunction>>{
                                std::make_pair(new OperatorToken("&&"),
                                               [](Node* first, Node* second) -> Node* {
                                                 return new AndExprNode(static_cast<CondExprNode*>(first),
                                                                        static_cast<CondExprNode*>(second));
                                               }),
                                std::make_pair(new OperatorToken("||"), [](Node* first, Node* second) -> Node* {
                                  return new OrExprNode(static_cast<CondExprNode*>(first),
                                                        static_cast<CondExprNode*>(second));
                                })}) {}

RelExprGrammarRule::RelExprGrammarRule()
    : LateChoiceGrammarRule(new RelFactorGrammarProducer(),
                            std::vector<std::pair<Token*, MergeFunction>>{
                                std::make_pair(new OperatorToken(">"),
                                               [](Node* first, Node* second) -> Node* {
                                                 return new GreaterThanNode(static_cast<RelFactorNode*>(first),
                                                                            static_cast<RelFactorNode*>(second));
                                               }),
                                std::make_pair(new OperatorToken(">="),
                                               [](Node* first, Node* second) -> Node* {
                                                 return new GreaterThanEqualNode(static_cast<RelFactorNode*>(first),
                                                                                 static_cast<RelFactorNode*>(second));
                                               }),
                                std::make_pair(new OperatorToken("<"),
                                               [](Node* first, Node* second) -> Node* {
                                                 return new LessThanNode(static_cast<RelFactorNode*>(first),
                                                                         static_cast<RelFactorNode*>(second));
                                               }),
                                std::make_pair(new OperatorToken("<="),
                                               [](Node* first, Node* second) -> Node* {
                                                 return new LessThanEqualNode(static_cast<RelFactorNode*>(first),
                                                                              static_cast<RelFactorNode*>(second));
                                               }),
                                std::make_pair(new OperatorToken("=="),
                                               [](Node* first, Node* second) -> Node* {
                                                 return new EqualNode(static_cast<RelFactorNode*>(first),
                                                                      static_cast<RelFactorNode*>(second));
                                               }),
                                std::make_pair(new OperatorToken("!="),
                                               [](Node* first, Node* second) -> Node* {
                                                 return new NotEqualNode(static_cast<RelFactorNode*>(first),
                                                                         static_cast<RelFactorNode*>(second));
                                               }),
                            }) {}
