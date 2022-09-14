// Copyright 2022 CS3203 Team14. All rights reserved.

#include "gtest/gtest.h"
#include "sp/extractor/entity_extractor.h"
#include "sp/simple_definition/simple_ast.h"

TEST(ExtractorTest, TestEntity) {
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

  std::vector<Entity *> entities = EntityExtractor::ExtractAll(program);
  // DFS traversal should yield the following result
  std::vector<Entity *> expected = {
      new ProcedureEntity("main"), new AssignStmtEntity(0), new VariableEntity("v1"), new ConstantEntity("1"),
      new AssignStmtEntity(0), new VariableEntity("v2"), new ConstantEntity("1"),
  };
  ASSERT_EQ(entities.size(), expected.size());
  for (int i = 0; i < entities.size(); ++i) {
    ASSERT_EQ(entities[i]->GetType(), expected[i]->GetType());
    ASSERT_EQ(entities[i]->GetName(), expected[i]->GetName());
  }
}
