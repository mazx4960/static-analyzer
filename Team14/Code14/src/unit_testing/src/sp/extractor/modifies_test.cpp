// Copyright 2022 CS3203 Team14. All rights reserved.

#include "gtest/gtest.h"
#include "sp/extractor/entity_extractor.h"
#include "sp/extractor/relationship_extractor.h"
#include "sp/simple_definition/simple_ast.h"

TEST(ExtractorTest, TestModifies) {
  auto *v1 = new VariableNode("v1");
  auto *v2 = new VariableNode("v2");
  auto *a1 = new AssignNode(v1, v2);
  std::vector<StatementNode *> stmts = {a1};
  auto *sl = new StatementListNode(stmts);
  auto *p1 = new ProcedureNode("main", sl);
  std::vector<ProcedureNode *> procs = {p1};
  auto *program = new ProgramNode(procs);

  std::vector<Relationship *> relationships;
  auto const op = [&relationships](Node *node) { RelationshipExtractor::ExtractModifies(relationships, node); };
  program->VisitAll(op);

  // DFS traversal should yield the following result
  std::vector<Relationship *> expected = {
      new ModifiesRelationship(new ProcedureEntity("main"), new VariableEntity("v1")),
      new ModifiesRelationship(new AssignEntity(1), new VariableEntity("v1")),
  };

  ASSERT_EQ(relationships.size(), expected.size());
  for (int i = 0; i < relationships.size(); ++i) {
    ASSERT_EQ(relationships[i]->GetType(), expected[i]->GetType());
    ASSERT_EQ(relationships[i]->GetFirst()->ToString(), expected[i]->GetFirst()->ToString());
    ASSERT_EQ(relationships[i]->GetSecond()->ToString(), expected[i]->GetSecond()->ToString());
  }
}
