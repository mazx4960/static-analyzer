// Copyright 2022 CS3203 Team14. All rights reserved.

#include "gtest/gtest.h"
#include "sp/extractor/entity_extractor.h"
#include "sp/extractor/relationship_extractor.h"
#include "sp/simple_definition/simple_ast.h"

TEST(ExtractorTest, TestParent) {
  auto *v1 = new VariableNode("v1");
  auto *v2 = new VariableNode("v2");
  auto *c = new ConstantNode(1);
  auto *a1 = new AssignNode(v1, c);
  auto *a2 = new AssignNode(v2, c);
  auto *cond = new EqualNode(v1, c);
  std::vector<StatementNode *> stmts = {a1, a2};
  auto *sl = new StatementListNode(stmts);
  auto *w = new WhileNode(cond, sl);
  std::vector<StatementNode *> stmts2 = {w};
  auto *sl2 = new StatementListNode(stmts2);
  auto *p1 = new ProcedureNode("main", sl2);
  std::vector<ProcedureNode *> procs = {p1};
  auto *program = new ProgramNode(procs);

  std::vector<Relationship *> relationships;
  auto const op = [&relationships](Node *node) {
    RelationshipExtractor::ExtractParent(relationships, node);
  };
  program->VisitAll(op);

  // DFS traversal should yield the following result
  std::vector<Relationship *> expected = {new ParentRelationship(new ProcedureEntity("main"), new WhileStmtEntity("1")),
                                          new ParentRelationship(new WhileStmtEntity("1"), new AssignStmtEntity("2")),
                                          new ParentRelationship(new WhileStmtEntity("1"), new AssignStmtEntity("3")),};

  ASSERT_EQ(relationships.size(), expected.size());
  for (int i = 0; i < relationships.size(); ++i) {
    ASSERT_EQ(relationships[i]->GetType(), expected[i]->GetType());
    ASSERT_EQ(relationships[i]->GetFirst()->GetValue(), expected[i]->GetFirst()->GetValue());
    ASSERT_EQ(relationships[i]->GetSecond()->GetValue(), expected[i]->GetSecond()->GetValue());
  }
}