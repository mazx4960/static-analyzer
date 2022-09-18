#include <gtest/gtest.h>

#include "commons/lexer/token.h"
#include "commons/parser/expr_definition/expr_grammar.h"
#include "commons/parser/node/node_type.h"
#include "commons/parser/parser_exceptions.h"
#include "commons/reader.h"
#include "commons/types.h"
#include "sp/simple_definition/simple_ast.h"
#include "sp/simple_definition/simple_grammar.h"
#include "sp/simple_lexer.h"
#include "sp/simple_parser.h"

TEST(SimpleParserTest, TestBasic) {
  std::vector<Token *> input_tokens = {
      new KeywordToken("procedure"), new SymbolToken("main"),      new CurlyOpenBracketToken(),
      new SymbolToken("x"),          new OperatorToken("="),       new LiteralToken("1"),
      new SemicolonToken(),          new CurlyCloseBracketToken(), new EndOfFileToken()};
  Node *program_node = SimpleParser::ParseProgram(input_tokens);

  ASSERT_EQ(NodeType::kProgram, program_node->GetNodeType());
  auto program_children = program_node->GetChildren();
  ASSERT_EQ(1, program_children.size());
  auto *procedure_node = program_children[0];
  ASSERT_EQ(NodeType::kProcedure, procedure_node->GetNodeType());
  ASSERT_EQ("main", static_cast<ProcedureNode *>(procedure_node)->GetProcName());
  auto procedure_children = procedure_node->GetChildren();
  ASSERT_EQ(1, procedure_children.size());
  auto *statement_list_node = procedure_children[0];
  ASSERT_EQ(NodeType::kStatementList, statement_list_node->GetNodeType());
  auto statement_children = statement_list_node->GetChildren();
  ASSERT_EQ(1, statement_children.size());
  auto *statement_node = statement_children[0];
  ASSERT_EQ(NodeType::kStatement, statement_node->GetNodeType());
  ASSERT_EQ(EntityType::kAssignStmt, static_cast<StatementNode *>(statement_node)->GetStmtType());
  auto assign_children = statement_node->GetChildren();
  ASSERT_EQ(2, assign_children.size());
  auto *variable_node = assign_children[0];
  ASSERT_EQ(NodeType::kVariable, variable_node->GetNodeType());
  ASSERT_EQ("x", static_cast<VariableNode *>(variable_node)->GetVariableName());
  auto *constant_node = assign_children[1];
  ASSERT_EQ(NodeType::kConstant, constant_node->GetNodeType());
  ASSERT_EQ(1, static_cast<ConstantNode *>(constant_node)->GetValue());
}

TEST(SimpleParserTestAssignment, AdvancedTest) {
  std::istream *s = StreamReader::GetStreamFromFile("../Tests14/simple_code/assignment1.txt");
  SimpleLexer simple_lexer(s);
  std::vector<Token *> tokens = simple_lexer.lex();
  // The main purpose of this test is to see if it can parse complex programs
  Node *program_node = SimpleParser::ParseProgram(tokens);
  ASSERT_EQ(NodeType::kProgram, program_node->GetNodeType());
}

TEST(ReferenceParser, ConstReferenceTest) {
  std::vector<Token *> const_token = {new LiteralToken("87")};
  auto const_token_stream = const_token.begin();
  Node *const_node = (new ReferenceGrammarRule())->parseNode(const_token_stream);
  ASSERT_EQ(const_node->GetNodeType(), NodeType::kConstant);
  ASSERT_EQ(87, static_cast<ConstantNode *>(const_node)->GetValue());
}

TEST(ReferenceParser, VarReferenceTest) {
  std::vector<Token *> var_token = {new SymbolToken("qwerty")};
  auto var_token_stream = var_token.begin();
  Node *var_node = (new ReferenceGrammarRule())->parseNode(var_token_stream);
  ASSERT_EQ(var_node->GetNodeType(), NodeType::kVariable);
  ASSERT_EQ("qwerty", static_cast<VariableNode *>(var_node)->GetVariableName());
}

TEST(ReferenceParser, InvalidReferenceTest) {
  auto *parser = new ReferenceGrammarRule();
  std::vector<Token *> operator_tokens = {new OperatorToken("<"), new EndOfFileToken()};
  auto operator_stream = operator_tokens.begin();
  ASSERT_THROW(parser->parseNode(operator_stream), ParseSyntaxError);
  std::vector<Token *> quote_tokens = {new QuoteToken(), new EndOfFileToken()};
  auto quote_stream = quote_tokens.begin();
  ASSERT_THROW(parser->parseNode(quote_stream), ParseSyntaxError);
  std::vector<Token *> semicolon_tokens = {new SemicolonToken(), new EndOfFileToken()};
  auto semicolon_stream = semicolon_tokens.begin();
  ASSERT_THROW(parser->parseNode(semicolon_stream), ParseSyntaxError);
  std::vector<Token *> eof_tokens = {new EndOfFileToken(), new EndOfFileToken()};
  auto eof_stream = eof_tokens.begin();
  ASSERT_THROW(parser->parseNode(eof_stream), ParseSyntaxError);
}

TEST(ExprParser, RefExprTest) {
  auto *expr_parser = new ExprGrammarRule();
  std::vector<Token *> const_token = {new LiteralToken("87"), new EndOfFileToken()};
  auto const_token_stream = const_token.begin();
  Node *const_node = expr_parser->parseNode(const_token_stream);
  ASSERT_EQ(const_node->GetNodeType(), NodeType::kConstant);
  ASSERT_EQ(87, static_cast<ConstantNode *>(const_node)->GetValue());
  std::vector<Token *> var_token = {new SymbolToken("qwerty"), new EndOfFileToken()};
  auto var_token_stream = var_token.begin();
  Node *var_node = expr_parser->parseNode(var_token_stream);
  ASSERT_EQ(var_node->GetNodeType(), NodeType::kVariable);
  ASSERT_EQ("qwerty", static_cast<VariableNode *>(var_node)->GetVariableName());
}

TEST(ExprParser, BasicExprTest) {
  auto *expr_parser = new ExprGrammarRule();
  std::vector<std::vector<Token *>> token_sets = {
      {new LiteralToken("9"), new OperatorToken("+"), new SymbolToken("aasdf"), new EndOfFileToken()},
      {new LiteralToken("8"), new OperatorToken("-"), new LiteralToken("4"), new EndOfFileToken()},
      {new SymbolToken("rfv"), new OperatorToken("*"), new LiteralToken("55"), new EndOfFileToken()},
      {new SymbolToken("ed"), new OperatorToken("/"), new SymbolToken("aasdf"), new EndOfFileToken()},
      {new LiteralToken("9"), new OperatorToken("%"), new SymbolToken("aasdf"), new EndOfFileToken()},
  };
  std::vector<ExprType> expected_expr_types = {ExprType::kPlus, ExprType::kMinus, ExprType::kTimes, ExprType::kDiv,
                                               ExprType::kMod};
  for (int i = 0; i < token_sets.size(); ++i) {
    auto token_stream = token_sets[i].begin();
    Node *node = expr_parser->parseNode(token_stream);
    ASSERT_EQ(node->GetNodeType(), NodeType::kExpr);
    ASSERT_EQ(static_cast<ExprNode *>(node)->GetExprType(), expected_expr_types[i]);
  }
}

TEST(ExprParser, LongExprTest) {
  auto *expr_parser = new ExprGrammarRule();
  std::vector<Token *> long_plus_minus_tokens = {new SymbolToken("aaa"), new OperatorToken("+"), new LiteralToken("44"),
                                                 new OperatorToken("+"), new SymbolToken("aaa"), new OperatorToken("-"),
                                                 new SymbolToken("aaa"), new OperatorToken("-"), new LiteralToken("44"),
                                                 new OperatorToken("+"), new LiteralToken("44"), new OperatorToken("-"),
                                                 new LiteralToken("44"), new EndOfFileToken()};
  std::vector<ExprType> expr_types = {ExprType::kMinus, ExprType::kPlus, ExprType::kMinus,
                                      ExprType::kMinus, ExprType::kPlus, ExprType::kPlus};
  std::vector<NodeType> node_types = {NodeType::kConstant, NodeType::kConstant, NodeType::kConstant,
                                      NodeType::kVariable, NodeType::kVariable, NodeType::kConstant,
                                      NodeType::kVariable};
  auto token_stream = long_plus_minus_tokens.begin();
  Node *node = expr_parser->parseNode(token_stream);
  for (int i = 0; i < 6; i++) {
    ASSERT_EQ(node->GetNodeType(), NodeType::kExpr);
    ASSERT_EQ(static_cast<ExprNode *>(node)->GetExprType(), expr_types[i]);
    std::vector<Node *> children = node->GetChildren();
    ASSERT_EQ(children.size(), 2);
    ASSERT_EQ(children[1]->GetNodeType(), node_types[i]);
    node = children[0];
  }
  ASSERT_EQ(node->GetNodeType(), node_types[6]);
}

TEST(ExprParser, MixedExprTest) {
  auto *expr_parser = new ExprGrammarRule();
  std::vector<Token *> plus_times_expr_tokens = {new SymbolToken("aaa"), new OperatorToken("+"), new LiteralToken("44"),
                                                 new OperatorToken("*"), new SymbolToken("aaa"), new EndOfFileToken()};
  auto pt_token_stream = plus_times_expr_tokens.begin();
  Node *pt_node = expr_parser->parseNode(pt_token_stream);
  ASSERT_EQ(pt_node->GetNodeType(), NodeType::kExpr);
  ASSERT_EQ(static_cast<ExprNode *>(pt_node)->GetExprType(), ExprType::kPlus);
  std::vector<Token *> times_plus_expr_tokens = {new SymbolToken("aaa"), new OperatorToken("+"), new LiteralToken("44"),
                                                 new OperatorToken("*"), new SymbolToken("aaa"), new EndOfFileToken()};
  auto tp_token_stream = times_plus_expr_tokens.begin();
  Node *tp_node = expr_parser->parseNode(tp_token_stream);
  ASSERT_EQ(tp_node->GetNodeType(), NodeType::kExpr);
  ASSERT_EQ(static_cast<ExprNode *>(tp_node)->GetExprType(), ExprType::kPlus);
}

TEST(ExprParser, BracketedExprTest) {
  auto *expr_parser = new ExprGrammarRule();
  std::vector<Token *> tokens = {new RoundOpenBracketToken(), new LiteralToken("9"),        new OperatorToken("+"),
                                 new SymbolToken("aasdf"),    new RoundCloseBracketToken(), new EndOfFileToken()};
  auto token_stream = tokens.begin();
  Node *node = expr_parser->parseNode(token_stream);
  ASSERT_EQ(node->GetNodeType(), NodeType::kExpr);
  ASSERT_EQ(static_cast<ExprNode *>(node)->GetExprType(), ExprType::kPlus);
}

TEST(RelExprParser, BasicRelExprTest) {
  auto *expr_parser = new RelExprGrammarRule();
  std::vector<std::vector<Token *>> token_sets = {
      {new LiteralToken("9"), new OperatorToken("<"), new SymbolToken("aasdf"), new EndOfFileToken()},
      {new LiteralToken("8"), new OperatorToken("<="), new LiteralToken("4"), new EndOfFileToken()},
      {new SymbolToken("rfv"), new OperatorToken(">"), new LiteralToken("55"), new EndOfFileToken()},
      {new SymbolToken("ed"), new OperatorToken(">="), new SymbolToken("aasdf"), new EndOfFileToken()},
      {new LiteralToken("9"), new OperatorToken("=="), new SymbolToken("aasdf"), new EndOfFileToken()},
      {new LiteralToken("9"), new OperatorToken("!="), new SymbolToken("aasdf"), new EndOfFileToken()},
  };
  std::vector<CondExprType> expected_expr_types = {CondExprType::kLt,  CondExprType::kLte, CondExprType::kGt,
                                                   CondExprType::kGte, CondExprType::kEq,  CondExprType::kNeq};
  for (int i = 0; i < token_sets.size(); ++i) {
    auto token_stream = token_sets[i].begin();
    Node *node = expr_parser->parseNode(token_stream);
    ASSERT_EQ(node->GetNodeType(), NodeType::kCondExpr);
    ASSERT_EQ(static_cast<CondExprNode *>(node)->GetCondExprType(), expected_expr_types[i]);
  }
}

TEST(RelExprParser, LongRelExprTest) {
  auto *expr_parser = new RelExprGrammarRule();
  std::vector<Token *> tokens = {
      new LiteralToken("9"), new OperatorToken("+"), new SymbolToken("aasdf"), new OperatorToken("<="),
      new LiteralToken("8"), new OperatorToken("-"), new LiteralToken("4"),    new EndOfFileToken(),
  };
  auto token_stream = tokens.begin();
  Node *node = expr_parser->parseNode(token_stream);
  ASSERT_EQ(node->GetNodeType(), NodeType::kCondExpr);
  ASSERT_EQ(static_cast<CondExprNode *>(node)->GetCondExprType(), CondExprType::kLte);
  std::vector<Node *> children = node->GetChildren();
  ASSERT_EQ(children.size(), 2);
  ASSERT_EQ(children[0]->GetNodeType(), NodeType::kExpr);
  ASSERT_EQ(static_cast<ExprNode *>(children[0])->GetExprType(), ExprType::kPlus);
  ASSERT_EQ(children[1]->GetNodeType(), NodeType::kExpr);
  ASSERT_EQ(static_cast<ExprNode *>(children[1])->GetExprType(), ExprType::kMinus);
}

TEST(CondParser, BasicCondTest) {
  std::vector<Token *> cond1_tokens = {new LiteralToken("9"), new OperatorToken(">="), new SymbolToken("l"),
                                       new EndOfFileToken()};
  auto iter1 = cond1_tokens.begin();
  Node *node = static_cast<CondExprNode *>((new CondExprGrammarRule())->parseNode(iter1));
  ASSERT_EQ(node->GetNodeType(), NodeType::kCondExpr);
  std::vector<Token *> cond2_tokens = {new LiteralToken("9"), new OperatorToken(">="), new SymbolToken("l"),
                                       new EndOfFileToken()};
  auto iter2 = cond2_tokens.begin();
  node = static_cast<CondExprNode *>((new CondExprGrammarRule())->parseNode(iter2));
  ASSERT_EQ(node->GetNodeType(), NodeType::kCondExpr);
}

TEST(CondParser, AndCondTest) {
  auto *cond_parser = new CondExprGrammarRule();
  std::vector<Token *> tokens = {new RoundOpenBracketToken(), new LiteralToken("6"),        new OperatorToken("<"),
                                 new SymbolToken("a"),        new RoundCloseBracketToken(), new OperatorToken("&&"),
                                 new RoundOpenBracketToken(), new LiteralToken("6"),        new OperatorToken(">"),
                                 new SymbolToken("a"),        new RoundCloseBracketToken(), new EndOfFileToken()};
  auto token_stream = tokens.begin();
  Node *node = cond_parser->parseNode(token_stream);
  ASSERT_EQ(node->GetNodeType(), NodeType::kCondExpr);
  ASSERT_EQ(static_cast<CondExprNode *>(node)->GetCondExprType(), CondExprType::kAnd);
}

TEST(CondParser, OrCondTest) {
  auto *cond_parser = new CondExprGrammarRule();
  std::vector<Token *> tokens = {new RoundOpenBracketToken(), new LiteralToken("6"),        new OperatorToken("<"),
                                 new SymbolToken("a"),        new RoundCloseBracketToken(), new OperatorToken("||"),
                                 new RoundOpenBracketToken(), new LiteralToken("6"),        new OperatorToken(">"),
                                 new SymbolToken("a"),        new RoundCloseBracketToken(), new EndOfFileToken()};
  auto token_stream = tokens.begin();
  Node *node = cond_parser->parseNode(token_stream);
  ASSERT_EQ(node->GetNodeType(), NodeType::kCondExpr);
  ASSERT_EQ(static_cast<CondExprNode *>(node)->GetCondExprType(), CondExprType::kOr);
}

TEST(CondParser, NotCondTest) {
  auto *cond_parser = new CondExprGrammarRule();
  std::vector<Token *> tokens = {new OperatorToken("!"), new RoundOpenBracketToken(), new LiteralToken("6"),
                                 new OperatorToken("<"), new SymbolToken("a"),        new RoundCloseBracketToken(),
                                 new EndOfFileToken()};
  auto token_stream = tokens.begin();
  Node *node = cond_parser->parseNode(token_stream);
  ASSERT_EQ(node->GetNodeType(), NodeType::kCondExpr);
  ASSERT_EQ(static_cast<CondExprNode *>(node)->GetCondExprType(), CondExprType::kNot);
}

TEST(StatementParser, ReadTest) {
  auto *stmt_parser = new StatementGrammarRule();
  std::vector<Token *> tokens{new KeywordToken("read"), new KeywordToken("a"), new SemicolonToken(),
                              new EndOfFileToken()};
  auto token_stream = tokens.begin();
  Node *node = stmt_parser->parseNode(token_stream);
  ASSERT_EQ(node->GetNodeType(), NodeType::kStatement);
  ASSERT_EQ(static_cast<StatementNode *>(node)->GetStmtType(), EntityType::kReadStmt);
}

TEST(StatementParser, PrintTest) {
  auto *stmt_parser = new StatementGrammarRule();
  std::vector<Token *> tokens{new KeywordToken("print"), new KeywordToken("a"), new SemicolonToken(),
                              new EndOfFileToken()};
  auto token_stream = tokens.begin();
  Node *node = stmt_parser->parseNode(token_stream);
  ASSERT_EQ(node->GetNodeType(), NodeType::kStatement);
  ASSERT_EQ(static_cast<StatementNode *>(node)->GetStmtType(), EntityType::kPrintStmt);
}

TEST(StatementParser, CallTest) {
  auto *stmt_parser = new StatementGrammarRule();
  std::vector<Token *> tokens{new KeywordToken("call"), new KeywordToken("a"), new SemicolonToken(),
                              new EndOfFileToken()};
  auto token_stream = tokens.begin();
  Node *node = stmt_parser->parseNode(token_stream);
  ASSERT_EQ(node->GetNodeType(), NodeType::kStatement);
  ASSERT_EQ(static_cast<StatementNode *>(node)->GetStmtType(), EntityType::kCallStmt);
}

TEST(StatementParser, AssignTest) {
  auto *stmt_parser = new StatementGrammarRule();
  std::vector<Token *> tokens{new SymbolToken("a"), new OperatorToken("="), new LiteralToken("12"),
                              new SemicolonToken(), new EndOfFileToken()};
  auto token_stream = tokens.begin();
  Node *node = stmt_parser->parseNode(token_stream);
  ASSERT_EQ(node->GetNodeType(), NodeType::kStatement);
  ASSERT_EQ(static_cast<StatementNode *>(node)->GetStmtType(), EntityType::kAssignStmt);
}

TEST(StatementParser, AssignKeywordTest) {
  auto *stmt_parser = new StatementGrammarRule();
  std::vector<Token *> keyword_tokens{new KeywordToken("read"), new KeywordToken("print"), new KeywordToken("call"),
                                      new KeywordToken("while"), new KeywordToken("if")};
  std::vector<Token *> tokens{new KeywordToken("read"), new OperatorToken("="), new LiteralToken("12"),
                              new SemicolonToken(), new EndOfFileToken()};
  for (auto *keyword : keyword_tokens) {
    tokens[0] = keyword;
    auto token_stream = tokens.begin();
    Node *node = stmt_parser->parseNode(token_stream);
    ASSERT_EQ(node->GetNodeType(), NodeType::kStatement);
    ASSERT_EQ(static_cast<StatementNode *>(node)->GetStmtType(), EntityType::kAssignStmt);
    ASSERT_EQ(static_cast<AssignNode *>(node)->GetVariable()->GetVariableName(), keyword->value);
  }
}

TEST(StatementParser, IfStatementBasicTest) {
  auto *stmt_parser = new StatementGrammarRule();
  std::vector<Token *> tokens{new KeywordToken("if"),       new RoundOpenBracketToken(), new LiteralToken("12"),
                              new OperatorToken("<"),       new SymbolToken("s"),        new RoundCloseBracketToken(),
                              new KeywordToken("then"),     new CurlyOpenBracketToken(), new SymbolToken("x"),
                              new OperatorToken("="),       new LiteralToken("12"),      new SemicolonToken(),
                              new CurlyCloseBracketToken(), new KeywordToken("else"),    new CurlyOpenBracketToken(),
                              new KeywordToken("print"),    new SymbolToken("a"),        new SemicolonToken(),
                              new CurlyCloseBracketToken(), new EndOfFileToken()};
  auto token_stream = tokens.begin();
  Node *node = stmt_parser->parseNode(token_stream);
  ASSERT_EQ(node->GetNodeType(), NodeType::kStatement);
  ASSERT_EQ(static_cast<StatementNode *>(node)->GetStmtType(), EntityType::kIfStmt);
}

TEST(StatementParser, WhileStatementBasicTest) {
  auto *stmt_parser = new StatementGrammarRule();
  std::vector<Token *> tokens{new KeywordToken("while"),   new RoundOpenBracketToken(),  new LiteralToken("12"),
                              new OperatorToken("<"),      new SymbolToken("s"),         new RoundCloseBracketToken(),
                              new CurlyOpenBracketToken(), new KeywordToken("call"),     new KeywordToken("a"),
                              new SemicolonToken(),        new CurlyCloseBracketToken(), new EndOfFileToken()};
  auto token_stream = tokens.begin();
  Node *node = stmt_parser->parseNode(token_stream);
  ASSERT_EQ(node->GetNodeType(), NodeType::kStatement);
  ASSERT_EQ(static_cast<StatementNode *>(node)->GetStmtType(), EntityType::kWhileStmt);
}

TEST(StatementParser, IfWhileAdvancedTest) {
  auto *stmt_parser = new StatementGrammarRule();
  std::vector<Token *> tokens{new KeywordToken("if"),       new RoundOpenBracketToken(),  new LiteralToken("1"),
                              new OperatorToken("<"),       new SymbolToken("s"),         new RoundCloseBracketToken(),
                              new KeywordToken("then"),     new CurlyOpenBracketToken(),  new KeywordToken("if"),
                              new RoundOpenBracketToken(),  new LiteralToken("2"),        new OperatorToken("<"),
                              new SymbolToken("s"),         new RoundCloseBracketToken(), new KeywordToken("then"),
                              new CurlyOpenBracketToken(),  new KeywordToken("if"),       new RoundOpenBracketToken(),
                              new LiteralToken("3"),        new OperatorToken("<"),       new SymbolToken("s"),
                              new RoundCloseBracketToken(), new KeywordToken("then"),     new CurlyOpenBracketToken(),
                              new SymbolToken("x"),         new OperatorToken("="),       new LiteralToken("4"),
                              new SemicolonToken(),         new CurlyCloseBracketToken(), new KeywordToken("else"),
                              new CurlyOpenBracketToken(),  new KeywordToken("print"),    new SymbolToken("a"),
                              new SemicolonToken(),         new CurlyCloseBracketToken(), new CurlyCloseBracketToken(),
                              new KeywordToken("else"),     new CurlyOpenBracketToken(),  new KeywordToken("read"),
                              new SymbolToken("a"),         new SemicolonToken(),         new CurlyCloseBracketToken(),
                              new CurlyCloseBracketToken(), new KeywordToken("else"),     new CurlyOpenBracketToken(),
                              new KeywordToken("while"),    new RoundOpenBracketToken(),  new LiteralToken("5"),
                              new OperatorToken("<"),       new SymbolToken("s"),         new RoundCloseBracketToken(),
                              new CurlyOpenBracketToken(),  new KeywordToken("while"),    new RoundOpenBracketToken(),
                              new LiteralToken("6"),        new OperatorToken("<"),       new SymbolToken("s"),
                              new RoundCloseBracketToken(), new CurlyOpenBracketToken(),  new KeywordToken("call"),
                              new KeywordToken("a"),        new SemicolonToken(),         new CurlyCloseBracketToken(),
                              new CurlyCloseBracketToken(), new CurlyCloseBracketToken(), new EndOfFileToken()};
  auto token_stream = tokens.begin();
  Node *node = stmt_parser->parseNode(token_stream);
  ASSERT_EQ(node->GetNodeType(), NodeType::kStatement);
  ASSERT_EQ(static_cast<StatementNode *>(node)->GetStmtType(), EntityType::kIfStmt);
}
