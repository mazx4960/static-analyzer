// Copyright 2022 CS3203 Team14. All rights reserved.

#include "relationship_extractor.h"

#include <spdlog/spdlog.h>

#include "control_flow.h"
#include "entity_extractor.h"
#include "sp/simple_definition/simple_ast.h"

/**
 * Extracts all relationships from a given AST of a program.
 */
std::vector<Relationship *> RelationshipExtractor::ExtractAll(Node *node) {
  std::vector<Relationship *> relationships;
  auto const op = [&relationships](Node *node) {
    ExtractFollows(relationships, node);
    ExtractParent(relationships, node);
    ExtractUses(relationships, node);
    ExtractModifies(relationships, node);
    ExtractCalls(relationships, node);
    ExtractNext(relationships, node);
  };
  node->VisitAll(op);
  return relationships;
}
/**
 * Matches the parent to the children.
 *
 * @param relationships vector to populate
 * @param parent entity
 * @param children vector of entities
 */
void RelationshipExtractor::Match(std::vector<Relationship *> &relationships, RsType rs_type, Entity *parent,
                                  const std::vector<Entity *> &children) {
  for (auto *child : children) {
    auto *match = new Relationship(rs_type, parent, child);
    relationships.push_back(match);
  }
}
/**
 * Extracts all follows relationships from a given statement list.
 *
 * @param relationships vector to populate
 * @param node
 */
void RelationshipExtractor::ExtractFollows(std::vector<Relationship *> &relationships, Node *node) {
  if (node->GetNodeType() != NodeType::kStatementList) { return; }
  Entity *prev_entity = nullptr;
  for (auto *stmt : static_cast<StatementListNode *>(node)->GetStatements()) {
    auto stmt_type = stmt->GetStmtType();
    auto stmt_no = stmt->GetStmtNo();
    auto *cur_entity = new Entity(stmt_type, std::to_string(stmt_no));
    if (prev_entity != nullptr) {
      Relationship *follows = new FollowsRelationship(prev_entity, cur_entity);
      relationships.push_back(follows);
    }
    prev_entity = cur_entity;
  }
}
/**
 * Extracts all immediate parent relationships from a given AST node.
 *
 * @param relationships vector to populate
 * @param node
 */
void RelationshipExtractor::ExtractParent(std::vector<Relationship *> &relationships, Node *node) {
  switch (node->GetNodeType()) {
    case NodeType::kProcedure: {
      auto *proc = static_cast<ProcedureNode *>(node);
      auto *stmt_list = proc->GetStatementList();
      auto proc_name = proc->GetProcName();
      Entity *parent = new ProcedureEntity(proc_name);
      ExtractParentHelper(relationships, parent, stmt_list);
      break;
    }
    case NodeType::kStatement: {
      auto *stmt = static_cast<StatementNode *>(node);
      auto stmt_type = stmt->GetStmtType();
      switch (stmt_type) {
        case EntityType::kWhileStmt: {
          auto *while_stmt = static_cast<WhileNode *>(stmt);
          auto *stmt_list = while_stmt->GetStatementList();
          Entity *parent = new WhileStmtEntity(std::to_string(while_stmt->GetStmtNo()));
          ExtractParentHelper(relationships, parent, stmt_list);
          break;
        }
        case EntityType::kIfStmt: {
          auto *if_stmt = static_cast<IfNode *>(stmt);
          Entity *parent = new IfStmtEntity(std::to_string(if_stmt->GetStmtNo()));
          auto *then_stmt_list = if_stmt->GetThenStatementList();
          auto *else_stmt_list = if_stmt->GetElseStatementList();
          ExtractParentHelper(relationships, parent, then_stmt_list);
          ExtractParentHelper(relationships, parent, else_stmt_list);
          break;
        }
        default: break;// other statement entity types are ignored.
      }
      break;
    }
    default: break;// other node types are ignored.
  }
}
/**
 * Extracts all parent relationships from a given parent entity and statement list.
 * Helper function for ExtractParent
 *
 * @param relationships vector to populate
 * @param parent
 * @param node statement list node
 */
void RelationshipExtractor::ExtractParentHelper(std::vector<Relationship *> &relationships, Entity *parent,
                                                Node *node) {
  if (node->GetNodeType() != NodeType::kStatementList) { return; }
  // Get all children entities
  std::vector<Entity *> children;
  auto const op = [&children](Node *node) { EntityExtractor::ExtractStatement(children, node); };
  node->VisitChildren(op);
  // Match the child with the parent entity
  Match(relationships, RsType::kParent, parent, children);
}
/**
 * Extracts all immediate uses relationships from a given AST node.
 *
 * @param relationships vector to populate
 * @param node
 */
void RelationshipExtractor::ExtractUses(std::vector<Relationship *> &relationships, Node *node) {
  if (node->GetNodeType() != NodeType::kStatement) { return; }
  auto *stmt = static_cast<StatementNode *>(node);
  auto stmt_type = stmt->GetStmtType();
  auto *parent = new Entity(stmt_type, std::to_string(stmt->GetStmtNo()));
  switch (stmt_type) {
    case EntityType::kAssignStmt: {
      std::vector<Entity *> children =
          EntityExtractor::ExtractAllVariables(static_cast<AssignNode *>(node)->GetExpression());
      Match(relationships, RsType::kUses, parent, children);
      break;
    }
    case EntityType::kPrintStmt: {
      Node *variable = static_cast<PrintNode *>(node)->GetVariable();
      Entity *child = new VariableEntity(static_cast<VariableNode *>(variable)->GetVariableName());
      relationships.push_back(new Relationship(RsType::kUses, parent, child));
      break;
    }
    case EntityType::kIfStmt: {
      auto *if_node = static_cast<IfNode *>(node);
      std::vector<Entity *> children = EntityExtractor::ExtractAllVariables(if_node->GetConditional());
      Match(relationships, RsType::kUses, parent, children);
      break;
    }
    case EntityType::kWhileStmt: {
      auto *while_node = static_cast<WhileNode *>(node);
      std::vector<Entity *> children = EntityExtractor::ExtractAllVariables(while_node->GetConditional());
      Match(relationships, RsType::kUses, parent, children);
      break;
    }
    default: break;// other statement entity types are ignored.
  }
}
/**
 * Extracts all immediate modifies relationships from a given AST node.
 *
 * @param relationships vector to populate
 * @param node
 */
void RelationshipExtractor::ExtractModifies(std::vector<Relationship *> &relationships, Node *node) {
  if (node->GetNodeType() != NodeType::kStatement) { return; }
  auto *stmt = static_cast<StatementNode *>(node);
  auto stmt_type = stmt->GetStmtType();
  auto *parent = new Entity(stmt_type, std::to_string(stmt->GetStmtNo()));
  switch (stmt_type) {
    case EntityType::kAssignStmt: {
      VariableNode *variable = static_cast<AssignNode *>(node)->GetVariable();
      Entity *child = new VariableEntity(variable->GetVariableName());
      relationships.push_back(new Relationship(RsType::kModifies, parent, child));
      break;
    }
    case EntityType::kReadStmt: {
      VariableNode *variable = static_cast<ReadNode *>(node)->GetVariable();
      Entity *child = new VariableEntity(variable->GetVariableName());
      relationships.push_back(new Relationship(RsType::kModifies, parent, child));
      break;
    }
    default: break;
  }
}
/**
 * Extracts all immediate calls relationships from a given AST node.
 *
 * @param relationships vector to populate
 * @param node
 */
void RelationshipExtractor::ExtractCalls(std::vector<Relationship *> &relationships, Node *node) {
  if (node->GetNodeType() != NodeType::kStatement) { return; }
  auto *stmt = static_cast<StatementNode *>(node);
  if (stmt->GetStmtType() != EntityType::kCallStmt) { return; }
  auto *call_node = static_cast<CallNode *>(stmt);
  Entity *parent = new CallStmtEntity(std::to_string(stmt->GetStmtNo()));
  Entity *child = new ProcedureEntity(call_node->GetProcedureName());
  relationships.push_back(new Relationship(RsType::kCalls, parent, child));
}
/**
 * Extracts all immediate next relationships from a given AST node.
 *
 * @param relationships vector to populate
 * @param node
 */
void RelationshipExtractor::ExtractNext(std::vector<Relationship *> &relationships, Node *node) {
  if (node->GetNodeType() != NodeType::kProcedure) { return; }
  auto *proc = static_cast<ProcedureNode *>(node);
  spdlog::debug("Building control flow graph for procedure {}", proc->GetProcName());
  auto *cfg_builder = new CFGBuilder();
  auto *cfg = cfg_builder->Build(proc);

  auto const op = [&relationships](Node *node) {
    auto *cfg_node = static_cast<CFGNode *>(node);
    if (cfg_node->IsTerminal()) { return; }
    auto *parent = cfg_node->GetStmt();
    std::vector<Entity *> children;
    for (auto *child : node->GetChildren()) {
      auto *child_node = static_cast<CFGNode *>(child);
      if (!child_node->IsTerminal()) { children.push_back(child_node->GetStmt()); }
    }
    Match(relationships, RsType::kNext, parent, children);
  };
  cfg->VisitAll(op);
}
