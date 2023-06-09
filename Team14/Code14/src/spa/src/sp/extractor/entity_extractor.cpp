// Copyright 2022 CS3203 Team14. All rights reserved.

#include "entity_extractor.h"

#include <spdlog/spdlog.h>

/**
 * Extracts all entities from a given AST of a program.
 */
std::vector<Entity *> EntityExtractor::ExtractAll(Node *node) {
  std::vector<Entity *> entities;
  auto const op = [&entities](Node *node) {
    ExtractProcedure(entities, node);
    ExtractStatement(entities, node);
    ExtractVariable(entities, node);
    ExtractConstant(entities, node);
  };
  node->VisitAll(op);
  return entities;
}
/**
 * Extracts all variables from a given AST of a program.
 */
std::vector<Entity *> EntityExtractor::ExtractAllVariables(Node *node) {
  std::vector<Entity *> entities;
  auto const op = [&entities](Node *node) {
    ExtractVariable(entities, node);
  };
  node->VisitAll(op);
  return entities;
}
std::vector<Entity *> EntityExtractor::ExtractProcedureCalls(Node *node) {
  std::vector<Entity *> entities;
  auto const op = [&entities](Node *node) {
    if (node->GetNodeType() != NodeType::kStatement) {
      return;
    }
    auto *stmt = static_cast<StatementNode *>(node);
    auto stmt_type = stmt->GetStmtType();
    if (stmt_type != EntityType::kCallStmt) {
      return;
    }
    auto proc_name = static_cast<CallNode *>(node)->GetProcedureName();
    Entity *entity = new ProcedureEntity(proc_name);
    entities.push_back(entity);
  };
  node->VisitAll(op);
  return entities;
}
void EntityExtractor::ExtractProcedure(std::vector<Entity *> &entities, Node *node) {
  if (node->GetNodeType() != NodeType::kProcedure) {
    return;
  }
  auto proc_name = static_cast<ProcedureNode *>(node)->GetProcName();
  Entity *entity = new ProcedureEntity(proc_name);
  entities.push_back(entity);
}
void EntityExtractor::ExtractStatement(std::vector<Entity *> &entities, Node *node) {
  if (node->GetNodeType() != NodeType::kStatement) {
    return;
  }
  Entity *entity = GetStmtEntity(node);
  entities.push_back(entity);
}
Entity *EntityExtractor::GetStmtEntity(Node *node) {
  if (node->GetNodeType() != NodeType::kStatement) {
    spdlog::error("Node " + node->ToString() + " is not a statement");
    return nullptr;
  }
  auto *stmt = static_cast<StatementNode *>(node);
  auto stmt_type = stmt->GetStmtType();
  auto stmt_no = stmt->GetStmtNo();
  Entity *entity;
  VariableNode *var_node;
  switch (stmt_type) {
    case EntityType::kCallStmt: {
      auto proc_name = static_cast<CallNode *>(stmt)->GetProcedureName();
      entity = new CallStmtEntity(std::to_string(stmt_no), proc_name);
      break;
    }
    case EntityType::kReadStmt: {
      var_node = static_cast<ReadNode *>(stmt)->GetVariable();
      entity = new ReadStmtEntity(std::to_string(stmt_no), var_node->GetVariableName());
      break;
    }
    case EntityType::kPrintStmt: {
      var_node = static_cast<PrintNode *>(stmt)->GetVariable();
      entity = new PrintStmtEntity(std::to_string(stmt_no), var_node->GetVariableName());
      break;
    }
    default: {
      entity = new Entity(stmt_type, std::to_string(stmt_no));
    }
  }
  return entity;
}
void EntityExtractor::ExtractVariable(std::vector<Entity *> &entities, Node *node) {
  if (node->GetNodeType() != NodeType::kVariable) {
    return;
  }
  auto var_name = static_cast<VariableNode *>(node)->GetVariableName();
  Entity *entity = new VariableEntity(var_name);
  entities.push_back(entity);
}
void EntityExtractor::ExtractConstant(std::vector<Entity *> &entities, Node *node) {
  if (node->GetNodeType() != NodeType::kConstant) {
    return;
  }
  auto const_name = static_cast<ConstantNode *>(node)->GetValue();
  Entity *entity = new ConstantEntity(const_name);
  entities.push_back(entity);
}
