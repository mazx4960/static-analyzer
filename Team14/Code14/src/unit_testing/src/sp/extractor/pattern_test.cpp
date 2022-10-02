// Copyright 2022 CS3203 Team14. All rights reserved.

#include "gtest/gtest.h"
#include "sp/extractor/pattern_extractor.h"
#include "sp/simple_definition/simple_ast.h"

TEST(ExtractorTest, TestAssignPattern) {
  auto *v1 = new VariableNode("v1");
  auto *v2 = new VariableNode("v2");
  auto *c = new ConstantNode(1);
  auto *a1 = new AssignNode(v1, c);
  auto *a2 = new AssignNode(v2, c);
  std::vector<StatementNode *> stmts = {a1, a2};
  auto *s = new StatementListNode(stmts);
  auto *p1 = new ProcedureNode("main", s);
  std::vector<ProcedureNode *> procs = {p1};
  auto *program = new ProgramNode(procs);

  std::vector<Pattern *> patterns;
  auto const op = [&patterns](Node *node) { PatternExtractor::ExtractAssign(patterns, node); };
  program->VisitAll(op);

  std::vector<Pattern *> expected = {
      new Pattern(new AssignStmtEntity("2"), new VariableEntity("v2"), "(1)"),
      new Pattern(new AssignStmtEntity("1"), new VariableEntity("v1"), "(1)"),
  };
  ASSERT_EQ(patterns.size(), expected.size());
  for (int i = 0; i < patterns.size(); ++i) {
    ASSERT_EQ(patterns[i]->GetStmt()->ToString(), expected[i]->GetStmt()->ToString());
    ASSERT_EQ(patterns[i]->GetVariable()->ToString(), expected[i]->GetVariable()->ToString());
    ASSERT_EQ(patterns[i]->GetExpr(), expected[i]->GetExpr());
  }
}

TEST(ExtractorTest, TestIfPattern) {
  auto *v1 = new VariableNode("v1");
  auto *v2 = new VariableNode("v2");
  auto *c = new ConstantNode(1);
  auto *iff = new IfNode(new EqualNode(v1, c), new StatementListNode({}), new StatementListNode({}));
  auto *a1 = new AssignNode(v1, c);
  auto *a2 = new AssignNode(v2, c);
  std::vector<StatementNode *> stmts = {a1, a2, iff};
  auto *s = new StatementListNode(stmts);
  auto *p1 = new ProcedureNode("main", s);
  std::vector<ProcedureNode *> procs = {p1};
  auto *program = new ProgramNode(procs);

  std::vector<Pattern *> patterns;
  auto const op = [&patterns](Node *node) { PatternExtractor::ExtractIf(patterns, node); };
  program->VisitAll(op);

  std::vector<Pattern *> expected = {
      new Pattern(new IfStmtEntity("3"), new VariableEntity("v1"), ""),
  };
  ASSERT_EQ(patterns.size(), expected.size());
  for (int i = 0; i < patterns.size(); ++i) {
    ASSERT_EQ(patterns[i]->GetStmt()->ToString(), expected[i]->GetStmt()->ToString());
    ASSERT_EQ(patterns[i]->GetVariable()->ToString(), expected[i]->GetVariable()->ToString());
    ASSERT_EQ(patterns[i]->GetExpr(), expected[i]->GetExpr());
  }
}

TEST(ExtractorTest, TestWhilePattern) {
  auto *v1 = new VariableNode("v1");
  auto *v2 = new VariableNode("v2");
  auto *c = new ConstantNode(1);
  auto *w = new WhileNode(new EqualNode(v1, c), new StatementListNode({}));
  auto *a1 = new AssignNode(v1, c);
  auto *a2 = new AssignNode(v2, c);
  std::vector<StatementNode *> stmts = {a1, a2, w};
  auto *s = new StatementListNode(stmts);
  auto *p1 = new ProcedureNode("main", s);
  std::vector<ProcedureNode *> procs = {p1};
  auto *program = new ProgramNode(procs);

  std::vector<Pattern *> patterns;
  auto const op = [&patterns](Node *node) { PatternExtractor::ExtractWhile(patterns, node); };
  program->VisitAll(op);

  std::vector<Pattern *> expected = {
      new Pattern(new WhileStmtEntity("3"), new VariableEntity("v1"), ""),
  };
  ASSERT_EQ(patterns.size(), expected.size());
  for (int i = 0; i < patterns.size(); ++i) {
    ASSERT_EQ(patterns[i]->GetStmt()->ToString(), expected[i]->GetStmt()->ToString());
    ASSERT_EQ(patterns[i]->GetVariable()->ToString(), expected[i]->GetVariable()->ToString());
    ASSERT_EQ(patterns[i]->GetExpr(), expected[i]->GetExpr());
  }
}
