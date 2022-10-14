// Copyright 2022 CS3203 Team14. All rights reserved.
#include "pattern_extractor.h"

#include <spdlog/spdlog.h>

#include <iostream>

#include "entity_extractor.h"
#include "sp/simple_definition/simple_ast.h"

std::vector<Pattern *> PatternExtractor::ExtractAll(Node *node) {
  std::vector<Pattern *> patterns;
  auto const op = [&patterns](Node *node) {
    ExtractAssign(patterns, node);
    ExtractIf(patterns, node);
    ExtractWhile(patterns, node);
  };
  node->VisitAll(op);
  return patterns;
}
void PatternExtractor::ExtractAssign(std::vector<Pattern *> &patterns, Node *node) {
  if (node->GetNodeType() != NodeType::kStatement
      || static_cast<StatementNode *>(node)->GetStmtType() != EntityType::kAssignStmt) {
    return;
  }
  auto *assign = static_cast<AssignNode *>(node);
  auto *variable = new VariableEntity(assign->GetVariable()->GetVariableName());
  auto *stmt = new AssignStmtEntity(std::to_string(assign->GetStmtNo()));
  patterns.push_back(new Pattern(stmt, variable, assign->GetExpression()->ToString()));
}
void PatternExtractor::ExtractIf(std::vector<Pattern *> &patterns, Node *node) {
  if (node->GetNodeType() != NodeType::kStatement
      || static_cast<StatementNode *>(node)->GetStmtType() != EntityType::kIfStmt) {
    return;
  }
  auto *if_stmt = static_cast<IfNode *>(node);
  auto *stmt = new IfStmtEntity(std::to_string(if_stmt->GetStmtNo()));
  std::vector<Entity *> vars = EntityExtractor::ExtractAllVariables(if_stmt->GetConditional());
  for (auto *var : vars) {
    patterns.push_back(new Pattern(stmt, var, ""));
  }
}
void PatternExtractor::ExtractWhile(std::vector<Pattern *> &patterns, Node *node) {
  if (node->GetNodeType() != NodeType::kStatement
      || static_cast<StatementNode *>(node)->GetStmtType() != EntityType::kWhileStmt) {
    return;
  }
  auto *while_stmt = static_cast<WhileNode *>(node);
  auto *stmt = new WhileStmtEntity(std::to_string(while_stmt->GetStmtNo()));
  std::vector<Entity *> vars = EntityExtractor::ExtractAllVariables(while_stmt->GetConditional());
  for (auto *var : vars) {
    patterns.push_back(new Pattern(stmt, var, ""));
  }
}
