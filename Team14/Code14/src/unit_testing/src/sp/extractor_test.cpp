// Copyright 2022 CS3203 Team14. All rights reserved.

#include "sp/extractor/extractor.h"

#include "gtest/gtest.h"
#include "sp/ast/simple_ast.h"

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

  std::vector<Entity *> entities = EntityExtractor::Extract(program);
  // DFS traversal should yield the following result
  std::vector<Entity *> expected = {
      new ProcedureEntity("main"),
      new AssignEntity(0),
      new VariableEntity("v1"),
      new ConstantEntity("1"),
      new AssignEntity(0),
      new VariableEntity("v2"),
      new ConstantEntity("1"),
  };
  ASSERT_EQ(entities.size(), expected.size());
  for (int i = 0; i < entities.size(); ++i) {
    EXPECT_EQ(entities[i]->GetType(), expected[i]->GetType())
        << "Entity " << i << " does not have the right type";
    EXPECT_EQ(entities[i]->GetName(), expected[i]->GetName())
        << "Entity " << i << " does not have the right name";
  }
};
