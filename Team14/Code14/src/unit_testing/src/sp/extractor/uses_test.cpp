// Copyright 2022 CS3203 Team14. All rights reserved.

#include "gtest/gtest.h"
#include "sp/extractor/entity_extractor.h"
#include "sp/extractor/relationship_extractor.h"
#include "sp/simple_definition/simple_ast.h"

TEST(ExtractorTest, TestUses) {
  auto *v1 = new VariableNode("v1");
  auto *v2 = new VariableNode("v2");
  auto *a1 = new AssignNode(v1, v2);
  std::vector<StatementNode *> stmts = {a1};
  auto *sl = new StatementListNode(stmts);
  auto *p1 = new ProcedureNode("main", sl);
  std::vector<ProcedureNode *> procs = {p1};
  auto *program = new ProgramNode(procs);

  std::vector<Relationship *> relationships;
  auto const op = [&relationships](Node *node) {
    RelationshipExtractor::ExtractUses(relationships, node);
  };
  program->VisitAll(op);

  // DFS traversal should yield the following result
  std::vector<Relationship *> expected = {new UsesRelationship(new AssignStmtEntity("1"), new VariableEntity("v2")),};

  ASSERT_EQ(relationships.size(), expected.size());
  for (int i = 0; i < relationships.size(); ++i) {
    ASSERT_EQ(relationships[i]->GetType(), expected[i]->GetType());
    ASSERT_EQ(relationships[i]->GetFirst()->GetValue(), expected[i]->GetFirst()->GetValue());
    ASSERT_EQ(relationships[i]->GetSecond()->GetValue(), expected[i]->GetSecond()->GetValue());
  }
}
