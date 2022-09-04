// Copyright 2022 CS3203 Team14. All rights reserved.

#include "extractor.h"

#include "traverser.h"

/**
 * Extracts all entities from a given AST of a program.
 */
std::vector<Entity *> EntityExtractor::ExtractEntity(ProgramNode *program_node) {
  std::vector<Entity *> entities;
  auto const op = [&entities](SimpleAstNode *node) {
    if (node->GetNodeType() == SimpleNodeType::kProcedure) {
      auto proc_name = static_cast<ProcedureNode *>(node)->GetProcName();
      Entity *entity = new ProcedureEntity(proc_name);
      entities.push_back(entity);
    } else if (node->GetNodeType() == SimpleNodeType::kStatement) {
      auto *stmt = static_cast<StatementNode *>(node);
      auto stmt_type = stmt->GetStmtType();
      auto stmt_no = stmt->GetStmtNo();
      Entity *entity = new StatementEntity(stmt_type, stmt_no);
      entities.push_back(entity);
    } else if (node->GetNodeType() == SimpleNodeType::kVariable) {
      auto var_name = static_cast<VariableNode *>(node)->GetVariableName();
      Entity *entity = new VariableEntity(var_name);
      entities.push_back(entity);
    } else if (node->GetNodeType() == SimpleNodeType::kConstant) {
      auto const_name = static_cast<ConstantNode *>(node)->GetValue();
      Entity *entity = new ConstantEntity(std::to_string(const_name));
      entities.push_back(entity);
    }
  };
  Traverser::TraverseProgram(program_node, op);
  return entities;
}

/**
 * Extracts all relationships from a given AST of a program.
 * Currently only support follows and parent relationships
 */
std::vector<Relationship *> RelationshipExtractor::ExtractRelationship(ProgramNode *program_node) {
  std::vector<Relationship *> relationships;
  auto const op = [&relationships](SimpleAstNode *node) {
    // Extract follows relationships
    if (node->GetNodeType() == SimpleNodeType::kStatementList) {
      auto *stmt_list = static_cast<StatementListNode *>(node);
      std::vector<Relationship *> follows = ExtractFollows(stmt_list);
      relationships.insert(relationships.end(), follows.begin(), follows.end());
    }
    // Extract parent relationships
    std::vector<Relationship *> parent = ExtractParent(node);
    relationships.insert(relationships.end(), parent.begin(), parent.end());
  };
  Traverser::TraverseProgram(program_node, op);
  return relationships;
}
/**
 * Extracts all follows relationships from a given statement list.
 *
 * @param node
 * @return vector of follows relationships
 */
std::vector<Relationship *> RelationshipExtractor::ExtractFollows(StatementListNode *node) {
  std::vector<Relationship *> relationships;
  Entity *prev_entity = nullptr;
  for (auto *stmt : node->GetStatements()) {
    auto stmt_type = stmt->GetStmtType();
    auto stmt_no = stmt->GetStmtNo();
    Entity *cur_entity = new StatementEntity(stmt_type, stmt_no);
    if (prev_entity != nullptr) {
      Relationship *follows = new FollowsRelationship(prev_entity, cur_entity);
      relationships.push_back(follows);
    }
    prev_entity = cur_entity;
  }
  return relationships;
}
/**
 * Extracts all immediate parent relationships from a given AST node.
 *
 * @param node
 * @return vector of parent relationships
 */
std::vector<Relationship *> RelationshipExtractor::ExtractParent(SimpleAstNode *node) {
  if (node->GetNodeType() == SimpleNodeType::kProcedure) {
    auto *proc = static_cast<ProcedureNode *>(node);
    auto *stmt_list = proc->GetStatementList();
    auto proc_name = proc->GetProcName();
    Entity *parent = new ProcedureEntity(proc_name);
    return ExtractParentHelper(parent, stmt_list);
  }
  if (node->GetNodeType() == SimpleNodeType::kStatement) {
    auto *stmt = static_cast<StatementNode *>(node);
    auto stmt_type = stmt->GetStmtType();
    if (stmt_type == StmtType::kWhile) {
      auto *while_stmt = static_cast<WhileNode *>(stmt);
      auto *stmt_list = while_stmt->GetStatementList();
      Entity *parent = new WhileEntity(while_stmt->GetStmtNo());
      return ExtractParentHelper(parent, stmt_list);
    }
    if (stmt_type == StmtType::kIf) {
      auto *if_stmt = static_cast<IfNode *>(stmt);
      Entity *parent = new WhileEntity(if_stmt->GetStmtNo());
      auto *if_stmt_list = if_stmt->GetIfStatementList();
      auto *then_stmt_list = if_stmt->GetThenStatementList();
      auto relationships = ExtractParentHelper(parent, if_stmt_list);
      auto then_relationships = ExtractParentHelper(parent, then_stmt_list);
      relationships.insert(relationships.end(), then_relationships.begin(), then_relationships.end());
      return relationships;
    }
  }
  return std::vector<Relationship *>();
}
/**
 * Extracts all parent relationships from a given parent entity and statement list.
 * Helper function for ExtractParent
 *
 * @param parent
 * @param node
 * @return vector of parent relationships
 */
std::vector<Relationship *> RelationshipExtractor::ExtractParentHelper(Entity *parent, StatementListNode *node) {
  std::vector<Entity *> children = ExtractChildren(node);
  std::vector<Relationship *> relationships;
  for (auto *child : children) {
    Relationship *parent_relationship = new ParentRelationship(parent, child);
    relationships.push_back(parent_relationship);
  }
  return relationships;
}
/**
 * Extracts all children entities from a given statement list.
 *
 * @param node
 * @return vector of all children entities
 */
std::vector<Entity *> RelationshipExtractor::ExtractChildren(StatementListNode *node) {
  std::vector<Entity *> children;
  for (auto *stmt : node->GetStatements()) {
    auto stmt_type = stmt->GetStmtType();
    auto stmt_no = stmt->GetStmtNo();
    Entity *entity = new StatementEntity(stmt_type, stmt_no);
    children.push_back(entity);
  }
  return children;
}
