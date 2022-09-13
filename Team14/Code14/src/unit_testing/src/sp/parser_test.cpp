#include <gtest/gtest.h>

#include "commons/lexer/token.h"
#include "commons/parser/node/node_type.h"
#include "commons/types.h"
#include "sp/simple_definition/simple_ast.h"
#include "sp/simple_lexer.h"
#include "sp/simple_parser.h"

TEST(SimpleParserTest, TestBasic) {
  std::vector<Token *> input_tokens = {
      new KeywordToken("procedure"), new SymbolToken("main"), new CurlyOpenBracketToken(),
      new SymbolToken("x"), new OperatorToken("="), new LiteralToken("1"),
      new SemicolonToken(), new CurlyCloseBracketToken(), new EndOfFileToken()};
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
  ASSERT_EQ(EntityType::kAssign, static_cast<StatementNode *>(statement_node)->GetStmtType());
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
  auto *s = new std::ifstream("Team14/Tests14/simple_code/assignment1.txt");
  SimpleLexer simple_lexer(s);
  std::vector<Token *> tokens = simple_lexer.lex();
  // The main purpose of this test is to see if it can parse complex programs
  Node *program_node = SimpleParser::ParseProgram(tokens);
  ASSERT_EQ(NodeType::kProgram, program_node->GetNodeType());
}
