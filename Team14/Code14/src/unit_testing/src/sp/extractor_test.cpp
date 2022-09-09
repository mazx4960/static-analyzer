// Copyright 2022 CS3203 Team14. All rights reserved.

#include "sp/extractor/extractor.h"

#include "gtest/gtest.h"
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

  std::vector<Entity *> entities = EntityExtractor::Extract(program);
  // DFS traversal should yield the following result
  std::vector<Entity *> expected = {
      new ProcedureEntity("main"), new AssignEntity(0),      new VariableEntity("v1"), new ConstantEntity("1"),
      new AssignEntity(0),         new VariableEntity("v2"), new ConstantEntity("1"),
  };
  ASSERT_EQ(entities.size(), expected.size());
  for (int i = 0; i < entities.size(); ++i) {
    ASSERT_EQ(entities[i]->GetType(), expected[i]->GetType());
    if (entities[i]->GetType() == EntityType::kStatement) {
      ASSERT_EQ(static_cast<StatementEntity*>(entities[i])->GetStmtNo(),
                static_cast<StatementEntity*>(expected[i])->GetStmtNo());
    }
    ASSERT_EQ(entities[i]->GetName(), expected[i]->GetName());
  }
}

TEST(ExtractorTest, TestRelationship) {
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

  std::vector<Relationship *> relationships = RelationshipExtractor::Extract(program);
  // DFS traversal should yield the following result
  std::vector<Relationship *> expected = {
      new ParentRelationship(new ProcedureEntity("main"), new AssignEntity(0)),
      new ParentRelationship(new ProcedureEntity("main"), new AssignEntity(0)),
      new FollowsRelationship(new AssignEntity(0), new AssignEntity(0)),
  };
  ASSERT_EQ(relationships.size(), expected.size());
  for (int i = 0; i < relationships.size(); ++i) { ASSERT_EQ(relationships[i]->GetType(), expected[i]->GetType()); }
}
