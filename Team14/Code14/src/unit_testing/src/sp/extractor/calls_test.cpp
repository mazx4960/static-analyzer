// Copyright 2022 CS3203 Team14. All rights reserved.

#include "gtest/gtest.h"
#include "sp/extractor/relationship_extractor.h"
#include "sp/simple_definition/simple_ast.h"

TEST(ExtractorTest, TestCalls) {
  auto *c = new CallNode("test");
  std::vector<StatementNode *> stmts = {c};
  auto *s = new StatementListNode(stmts);
  auto *p1 = new ProcedureNode("main", s);
  std::vector<ProcedureNode *> procs = {p1};
  auto *program = new ProgramNode(procs);

  std::vector<Relationship *> relationships;
  auto const op = [&relationships](Node *node) { RelationshipExtractor::ExtractCalls(relationships, node); };
  program->VisitAll(op);

  // DFS traversal should yield the following result
  std::vector<Relationship *> expected = {
      new CallsRelationship(new ProcedureEntity("main"), new ProcedureEntity("test")),
  };

  ASSERT_EQ(relationships.size(), expected.size());
  for (int i = 0; i < relationships.size(); ++i) {
    ASSERT_EQ(relationships[i]->GetType(), expected[i]->GetType());
    ASSERT_EQ(relationships[i]->GetFirst()->ToString(), expected[i]->GetFirst()->ToString());
    ASSERT_EQ(relationships[i]->GetSecond()->ToString(), expected[i]->GetSecond()->ToString());
  }
}

TEST(ExtractorTest, TestCallsNested) {
  auto *c2 = new CallNode("test2");
  std::vector<StatementNode *> stmts2 = {c2};
  auto *s1 = new StatementListNode(stmts2);
  auto *w = new WhileNode(new EqualNode(new VariableNode("v1"), new ConstantNode(1)), s1);
  auto *c1 = new CallNode("test1");
  std::vector<StatementNode *> stmts = {c1, w};
  auto *s = new StatementListNode(stmts);
  auto *p1 = new ProcedureNode("main", s);
  std::vector<ProcedureNode *> procs = {p1};
  auto *program = new ProgramNode(procs);

  std::vector<Relationship *> relationships;
  auto const op = [&relationships](Node *node) { RelationshipExtractor::ExtractCalls(relationships, node); };
  program->VisitAll(op);

  // DFS traversal should yield the following result
  std::vector<Relationship *> expected = {
      new CallsRelationship(new ProcedureEntity("main"), new ProcedureEntity("test2")),
      new CallsRelationship(new ProcedureEntity("main"), new ProcedureEntity("test1")),
  };

  ASSERT_EQ(relationships.size(), expected.size());
  for (int i = 0; i < relationships.size(); ++i) {
    ASSERT_EQ(relationships[i]->GetType(), expected[i]->GetType());
    ASSERT_EQ(relationships[i]->GetFirst()->ToString(), expected[i]->GetFirst()->ToString());
    ASSERT_EQ(relationships[i]->GetSecond()->ToString(), expected[i]->GetSecond()->ToString());
  }
}