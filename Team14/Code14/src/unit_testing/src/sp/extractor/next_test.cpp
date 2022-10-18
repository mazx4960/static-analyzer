// Copyright 2022 CS3203 Team14. All rights reserved.

#include "commons/reader.h"
#include "gtest/gtest.h"
#include "sp/extractor/relationship_extractor.h"
#include "sp/simple_definition/simple_ast.h"
#include "sp/simple_lexer.h"
#include "sp/simple_parser.h"

TEST(ExtractorTest, TestNextSingleBlock) {
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

  std::vector<Relationship *> relationships;
  auto const op = [&relationships](Node *node) {
    RelationshipExtractor::ExtractNext(relationships, node);
  };
  program->VisitAll(op);

  // DFS traversal should yield the following result
  std::vector<Relationship *> expected = {
      new NextRelationship(new AssignStmtEntity("1"), new AssignStmtEntity("2")),
  };

  ASSERT_EQ(relationships.size(), expected.size());
  for (int i = 0; i < relationships.size(); ++i) {
    ASSERT_EQ(relationships[i]->GetType(), expected[i]->GetType());
    ASSERT_EQ(relationships[i]->GetFirst()->ToString(), expected[i]->GetFirst()->ToString());
    ASSERT_EQ(relationships[i]->GetSecond()->ToString(), expected[i]->GetSecond()->ToString());
  }
}

TEST(ExtractorTest, TestNextIfBlock) {
  auto *v1 = new VariableNode("v1");
  auto *v2 = new VariableNode("v2");
  auto *c = new ConstantNode(1);
  auto *a3 = new AssignNode(v2, c);
  auto *a2 = new AssignNode(v2, c);
  auto *iff = new IfNode(new EqualNode(v1, c), new StatementListNode({a2}), new StatementListNode({a3}));
  auto *a1 = new AssignNode(v1, c);
  std::vector<StatementNode *> stmts = {a1, iff};
  auto *s = new StatementListNode(stmts);
  auto *p1 = new ProcedureNode("main", s);
  std::vector<ProcedureNode *> procs = {p1};
  auto *program = new ProgramNode(procs);

  std::vector<Relationship *> relationships;
  auto const op = [&relationships](Node *node) {
    RelationshipExtractor::ExtractNext(relationships, node);
  };
  program->VisitAll(op);

  // DFS traversal should yield the following result
  std::vector<Relationship *> expected = {
      new NextRelationship(new AssignStmtEntity("1"), new IfStmtEntity("2")),
      new NextRelationship(new IfStmtEntity("2"), new AssignStmtEntity("3")),
      new NextRelationship(new IfStmtEntity("2"), new AssignStmtEntity("4")),
  };

  ASSERT_EQ(relationships.size(), expected.size());
  for (int i = 0; i < relationships.size(); ++i) {
    ASSERT_EQ(relationships[i]->GetType(), expected[i]->GetType());
    ASSERT_EQ(relationships[i]->GetFirst()->ToString(), expected[i]->GetFirst()->ToString());
    ASSERT_EQ(relationships[i]->GetSecond()->ToString(), expected[i]->GetSecond()->ToString());
  }
}

TEST(ExtractorTest, TestNextWhileBlock) {
  auto *v1 = new VariableNode("v1");
  auto *v2 = new VariableNode("v2");
  auto *c = new ConstantNode(1);
  auto *a4 = new AssignNode(v2, c);
  auto *a3 = new AssignNode(v2, c);
  auto *a2 = new AssignNode(v2, c);
  auto *w = new WhileNode(new EqualNode(v1, c), new StatementListNode({a2, a3}));
  auto *a1 = new AssignNode(v1, c);
  std::vector<StatementNode *> stmts = {a1, w, a4};
  auto *s = new StatementListNode(stmts);
  auto *p1 = new ProcedureNode("main", s);
  std::vector<ProcedureNode *> procs = {p1};
  auto *program = new ProgramNode(procs);

  std::vector<Relationship *> relationships;
  auto const op = [&relationships](Node *node) {
    RelationshipExtractor::ExtractNext(relationships, node);
  };
  program->VisitAll(op);

  // DFS traversal should yield the following result
  std::vector<Relationship *> expected = {
      new NextRelationship(new AssignStmtEntity("1"), new WhileStmtEntity("2")),
      new NextRelationship(new WhileStmtEntity("2"), new AssignStmtEntity("3")),
      new NextRelationship(new WhileStmtEntity("2"), new AssignStmtEntity("5")),
      new NextRelationship(new AssignStmtEntity("3"), new AssignStmtEntity("4")),
      new NextRelationship(new AssignStmtEntity("4"), new WhileStmtEntity("2")),
  };

  ASSERT_EQ(relationships.size(), expected.size());
  for (int i = 0; i < relationships.size(); ++i) {
    ASSERT_EQ(relationships[i]->GetType(), expected[i]->GetType());
    ASSERT_EQ(relationships[i]->GetFirst()->ToString(), expected[i]->GetFirst()->ToString());
    ASSERT_EQ(relationships[i]->GetSecond()->ToString(), expected[i]->GetSecond()->ToString());
  }
}

TEST(ExtractorTest, TestNextIfNested) {
  std::istream *s = StreamReader::GetStreamFromFile("../Tests14/simple_code/next/nested_if.txt");
  SimpleLexer simple_lexer(s);
  std::vector<Token *> tokens = simple_lexer.lex();
  Node *program_node = SimpleParser::ParseProgram(tokens);

  std::vector<Relationship *> relationships;
  auto const op = [&relationships](Node *node) {
    RelationshipExtractor::ExtractNext(relationships, node);
  };
  program_node->VisitAll(op);

  // DFS traversal should yield the following result
  std::vector<Relationship *> expected = {
      new NextRelationship(new AssignStmtEntity("1"), new IfStmtEntity("2")),
      new NextRelationship(new IfStmtEntity("2"), new AssignStmtEntity("3")),
      new NextRelationship(new IfStmtEntity("2"), new ReadStmtEntity("7", std::string())),
      new NextRelationship(new AssignStmtEntity("3"), new IfStmtEntity("4")),
      new NextRelationship(new IfStmtEntity("4"), new AssignStmtEntity("5")),
      new NextRelationship(new IfStmtEntity("4"), new ReadStmtEntity("6", std::string())),
  };

  ASSERT_EQ(relationships.size(), expected.size());
  for (int i = 0; i < relationships.size(); ++i) {
    ASSERT_EQ(relationships[i]->GetType(), expected[i]->GetType());
    ASSERT_EQ(relationships[i]->GetFirst()->ToString(), expected[i]->GetFirst()->ToString());
    ASSERT_EQ(relationships[i]->GetSecond()->ToString(), expected[i]->GetSecond()->ToString());
  }
}

TEST(ExtractorTest, TestNextWhileNested) {
  std::istream *s = StreamReader::GetStreamFromFile("../Tests14/simple_code/next/nested_while.txt");
  SimpleLexer simple_lexer(s);
  std::vector<Token *> tokens = simple_lexer.lex();
  Node *program_node = SimpleParser::ParseProgram(tokens);

  std::vector<Relationship *> relationships;
  auto const op = [&relationships](Node *node) {
    RelationshipExtractor::ExtractNext(relationships, node);
  };
  program_node->VisitAll(op);

  // DFS traversal should yield the following result
  std::vector<Relationship *> expected = {
      new NextRelationship(new WhileStmtEntity("1"), new WhileStmtEntity("2")),
      new NextRelationship(new WhileStmtEntity("1"), new AssignStmtEntity("6")),
      new NextRelationship(new WhileStmtEntity("2"), new WhileStmtEntity("3")),
      new NextRelationship(new WhileStmtEntity("2"), new WhileStmtEntity("1")),
      new NextRelationship(new WhileStmtEntity("3"), new WhileStmtEntity("4")),
      new NextRelationship(new WhileStmtEntity("3"), new WhileStmtEntity("2")),
      new NextRelationship(new WhileStmtEntity("4"), new PrintStmtEntity("5", std::string())),
      new NextRelationship(new WhileStmtEntity("4"), new WhileStmtEntity("3")),
      new NextRelationship(new PrintStmtEntity("5", std::string()), new WhileStmtEntity("4")),
  };

  ASSERT_EQ(relationships.size(), expected.size());
  for (int i = 0; i < relationships.size(); ++i) {
    ASSERT_EQ(relationships[i]->GetType(), expected[i]->GetType());
    ASSERT_EQ(relationships[i]->GetFirst()->ToString(), expected[i]->GetFirst()->ToString());
    ASSERT_EQ(relationships[i]->GetSecond()->ToString(), expected[i]->GetSecond()->ToString());
  }
}

TEST(ExtractorTest, TestNextIfWhileNested) {
  std::istream *s = StreamReader::GetStreamFromFile("../Tests14/simple_code/next/nested_if_while.txt");
  SimpleLexer simple_lexer(s);
  std::vector<Token *> tokens = simple_lexer.lex();
  Node *program_node = SimpleParser::ParseProgram(tokens);

  std::vector<Relationship *> relationships;
  auto const op = [&relationships](Node *node) {
    RelationshipExtractor::ExtractNext(relationships, node);
  };
  program_node->VisitAll(op);

  // DFS traversal should yield the following result
  std::vector<Relationship *> expected = {
      new NextRelationship(new AssignStmtEntity("1"), new WhileStmtEntity("2")),
      new NextRelationship(new WhileStmtEntity("2"), new AssignStmtEntity("3")),
      new NextRelationship(new WhileStmtEntity("2"), new PrintStmtEntity("7", std::string())),
      new NextRelationship(new AssignStmtEntity("3"), new IfStmtEntity("4")),
      new NextRelationship(new IfStmtEntity("4"), new PrintStmtEntity("5", std::string())),
      new NextRelationship(new IfStmtEntity("4"), new PrintStmtEntity("6", std::string())),
      new NextRelationship(new PrintStmtEntity("6", std::string()), new WhileStmtEntity("2")),
      new NextRelationship(new PrintStmtEntity("5", std::string()), new WhileStmtEntity("2")),
  };

  ASSERT_EQ(relationships.size(), expected.size());
  for (int i = 0; i < relationships.size(); ++i) {
    ASSERT_EQ(relationships[i]->GetType(), expected[i]->GetType());
    ASSERT_EQ(relationships[i]->GetFirst()->ToString(), expected[i]->GetFirst()->ToString());
    ASSERT_EQ(relationships[i]->GetSecond()->ToString(), expected[i]->GetSecond()->ToString());
  }
}
