// Copyright 2022 CS3203 Team14. All rights reserved.

#include "relationship_extractor.h"

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
 * @param node
 * @return vector of follows relationships
 */
void RelationshipExtractor::ExtractFollows(std::vector<Relationship *> &relationships, Node *node) {
  if (node->GetNodeType() != NodeType::kStatementList) { return; }
  Entity *prev_entity = nullptr;
  for (auto *stmt : static_cast<StatementListNode *>(node)->GetStatements()) {
    auto stmt_type = stmt->GetStmtType();
    auto stmt_no = stmt->GetStmtNo();
    Entity *cur_entity = new StatementEntity(stmt_type, stmt_no);
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
  if (node->GetNodeType() == NodeType::kProcedure) {
    auto *proc = static_cast<ProcedureNode *>(node);
    auto *stmt_list = proc->GetStatementList();
    auto proc_name = proc->GetProcName();
    Entity *parent = new ProcedureEntity(proc_name);
    ExtractParentHelper(relationships, parent, stmt_list);
  }
  if (node->GetNodeType() == NodeType::kStatement) {
    auto *stmt = static_cast<StatementNode *>(node);
    auto stmt_type = stmt->GetStmtType();
    if (stmt_type == StmtType::kWhile) {
      auto *while_stmt = static_cast<WhileNode *>(stmt);
      auto *stmt_list = while_stmt->GetStatementList();
      Entity *parent = new WhileEntity(while_stmt->GetStmtNo());
      ExtractParentHelper(relationships, parent, stmt_list);
    }
    if (stmt_type == StmtType::kIf) {
      auto *if_stmt = static_cast<IfNode *>(stmt);
      Entity *parent = new WhileEntity(if_stmt->GetStmtNo());
      auto *then_stmt_list = if_stmt->GetThenStatementList();
      auto *else_stmt_list = if_stmt->GetElseStatementList();
      ExtractParentHelper(relationships, parent, then_stmt_list);
      ExtractParentHelper(relationships, parent, else_stmt_list);
    }
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
  if (node->GetNodeType() == NodeType::kProcedure) {
    auto *proc = static_cast<ProcedureNode *>(node);
    Entity *parent = new ProcedureEntity(proc->GetProcName());
    auto *stmt_list = proc->GetStatementList();
    ExtractUsesRecursive(relationships, parent, stmt_list);
  }
  if (node->GetNodeType() == NodeType::kStatement) {
    auto *stmt = static_cast<StatementNode *>(node);
    auto stmt_type = stmt->GetStmtType();
    if (stmt_type == StmtType::kAssign) {
      std::vector<Entity *> children =
          EntityExtractor::ExtractVariables(static_cast<AssignNode *>(node)->GetExpression());
      Entity *parent = new AssignEntity(stmt->GetStmtNo());
      Match(relationships, RsType::kUses, parent, children);
    } else if (stmt_type == StmtType::kPrint) {
      Entity *parent = new PrintEntity(stmt->GetStmtNo());
      Node *variable = static_cast<PrintNode *>(node)->GetVariable();
      Entity *child = new VariableEntity(static_cast<VariableNode *>(variable)->GetVariableName());
      relationships.push_back(new Relationship(RsType::kUses, parent, child));
    } else if (stmt_type == StmtType::kIf) {
      auto *if_node = static_cast<IfNode *>(node);
      Entity *parent = new IfEntity(stmt->GetStmtNo());
      std::vector<Entity *> children = EntityExtractor::ExtractVariables(if_node->GetConditional());
      Match(relationships, RsType::kUses, parent, children);
      ExtractUsesRecursive(relationships, parent, if_node->GetThenStatementList());
      ExtractUsesRecursive(relationships, parent, if_node->GetElseStatementList());
    } else if (stmt_type == StmtType::kWhile) {
      auto *while_node = static_cast<WhileNode *>(node);
      Entity *parent = new WhileEntity(stmt->GetStmtNo());
      std::vector<Entity *> children = EntityExtractor::ExtractVariables(while_node->GetConditional());
      Match(relationships, RsType::kUses, parent, children);
      ExtractUsesRecursive(relationships, parent, while_node->GetStatementList());
    } else if (stmt_type == StmtType::kCall) {
      // TODO: implement recursive uses for statements in call
    }
  }
}
void RelationshipExtractor::ExtractUsesRecursive(std::vector<Relationship *> &relationships, Entity *parent,
                                                 Node *node) {
  if (node->GetNodeType() == NodeType::kStatement) {
    auto *stmt = static_cast<StatementNode *>(node);
    auto stmt_type = stmt->GetStmtType();
    if (stmt_type == StmtType::kAssign) {
      std::vector<Entity *> children =
          EntityExtractor::ExtractVariables(static_cast<AssignNode *>(node)->GetExpression());
      Match(relationships, RsType::kUses, parent, children);
    } else if (stmt_type == StmtType::kPrint) {
      Node *variable = static_cast<PrintNode *>(node)->GetVariable();
      Entity *child = new VariableEntity(static_cast<VariableNode *>(variable)->GetVariableName());
      relationships.push_back(new Relationship(RsType::kUses, parent, child));
    } else if (stmt_type == StmtType::kIf) {
      auto *if_node = static_cast<IfNode *>(node);
      std::vector<Entity *> children = EntityExtractor::ExtractVariables(if_node->GetConditional());
      Match(relationships, RsType::kUses, parent, children);
      ExtractUsesRecursive(relationships, parent, if_node->GetThenStatementList());
      ExtractUsesRecursive(relationships, parent, if_node->GetElseStatementList());
    } else if (stmt_type == StmtType::kWhile) {
      auto *while_node = static_cast<WhileNode *>(node);
      std::vector<Entity *> children = EntityExtractor::ExtractVariables(while_node->GetConditional());
      Match(relationships, RsType::kUses, parent, children);
      ExtractUsesRecursive(relationships, parent, while_node->GetStatementList());
    } else if (stmt_type == StmtType::kCall) {
      // TODO: implement recursive uses for statements in call
    }
  }
}
void RelationshipExtractor::ExtractModifies(std::vector<Relationship *> &relationships, Node *node) {
  if (node->GetNodeType() == NodeType::kProcedure) {
    auto *proc = static_cast<ProcedureNode *>(node);
    Entity *parent = new ProcedureEntity(proc->GetProcName());
    auto *stmt_list = proc->GetStatementList();
    ExtractModifiesRecursive(relationships, parent, stmt_list);
  }
  if (node->GetNodeType() == NodeType::kStatement) {
    auto *stmt = static_cast<StatementNode *>(node);
    auto stmt_type = stmt->GetStmtType();
    if (stmt_type == StmtType::kAssign) {
      Entity *parent = new AssignEntity(stmt->GetStmtNo());
      VariableNode *variable = static_cast<AssignNode *>(node)->GetVariable();
      Entity *child = new VariableEntity(variable->GetVariableName());
      relationships.push_back(new Relationship(RsType::kModifies, parent, child));
    } else if (stmt_type == StmtType::kRead) {
      Entity *parent = new ReadEntity(stmt->GetStmtNo());
      VariableNode *variable = static_cast<ReadNode *>(node)->GetVariable();
      Entity *child = new VariableEntity(variable->GetVariableName());
      relationships.push_back(new Relationship(RsType::kModifies, parent, child));
    } else if (stmt_type == StmtType::kIf) {
      auto *if_node = static_cast<IfNode *>(node);
      Entity *parent = new IfEntity(stmt->GetStmtNo());
      ExtractModifiesRecursive(relationships, parent, if_node->GetThenStatementList());
      ExtractModifiesRecursive(relationships, parent, if_node->GetElseStatementList());
    } else if (stmt_type == StmtType::kWhile) {
      auto *while_node = static_cast<WhileNode *>(node);
      Entity *parent = new WhileEntity(stmt->GetStmtNo());
      ExtractModifiesRecursive(relationships, parent, while_node->GetStatementList());
    } else if (stmt_type == StmtType::kCall) {
      // TODO: implement recursive uses for statements in call
    }
  }
}
void RelationshipExtractor::ExtractModifiesRecursive(std::vector<Relationship *> &relationships, Entity *parent,
                                                     Node *node) {
  if (node->GetNodeType() == NodeType::kProcedure) {
    auto *proc = static_cast<ProcedureNode *>(node);
    auto *stmt_list = proc->GetStatementList();
    ExtractModifiesRecursive(relationships, parent, stmt_list);
  }
  if (node->GetNodeType() == NodeType::kStatement) {
    auto *stmt = static_cast<StatementNode *>(node);
    auto stmt_type = stmt->GetStmtType();
    if (stmt_type == StmtType::kAssign) {
      VariableNode *variable = static_cast<AssignNode *>(node)->GetVariable();
      Entity *child = new VariableEntity(variable->GetVariableName());
      relationships.push_back(new Relationship(RsType::kModifies, parent, child));
    } else if (stmt_type == StmtType::kRead) {
      VariableNode *variable = static_cast<ReadNode *>(node)->GetVariable();
      Entity *child = new VariableEntity(variable->GetVariableName());
      relationships.push_back(new Relationship(RsType::kModifies, parent, child));
    } else if (stmt_type == StmtType::kIf) {
      auto *if_node = static_cast<IfNode *>(node);
      ExtractModifiesRecursive(relationships, parent, if_node->GetThenStatementList());
      ExtractModifiesRecursive(relationships, parent, if_node->GetElseStatementList());
    } else if (stmt_type == StmtType::kWhile) {
      auto *while_node = static_cast<WhileNode *>(node);
      ExtractModifiesRecursive(relationships, parent, while_node->GetStatementList());
    } else if (stmt_type == StmtType::kCall) {
      // TODO: implement recursive uses for statements in call
    }
  }
}
