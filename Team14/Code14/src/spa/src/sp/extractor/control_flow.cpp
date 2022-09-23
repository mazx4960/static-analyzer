// Copyright 2022 CS3203 Team14. All rights reserved.

#include "control_flow.h"

#include <spdlog/spdlog.h>

#include <unordered_set>
#include <utility>

#include "sp/simple_definition/simple_ast.h"

CFGNode::CFGNode(Entity *stmt) : stmt_(stmt) {}
Entity *CFGNode::GetStmt() { return this->stmt_; }
void CFGNode::AddChild(CFGNode *child) { this->children_.push_back(child); }
void CFGNode::SetChildren(std::vector<CFGNode *> children) { this->children_ = std::move(children); }
bool CFGNode::IsTerminal() { return this->stmt_->GetValue() == "-1"; }
std::vector<CFGNode *> CFGNode::GetChildren() { return this->children_; }
std::string CFGNode::ToString() { return stmt_->ToString(); }
/**
 * DFS to visit all nodes
 * prevents infinite loop by keeping track of visited nodes
 * @param op
 */
void CFGNode::VisitAll(const std::function<void(CFGNode *)> &op) {
  std::unordered_map<CFGNode *, bool> visited;
  std::vector<CFGNode *> stack;
  stack.push_back(this);
  while (!stack.empty()) {
    CFGNode *node = stack.back();
    stack.pop_back();
    if (visited[node]) { continue; }
    visited[node] = true;
    op(node);
    for (CFGNode *child : node->GetChildren()) { stack.push_back(child); }
  }
}

/**
 * Builder class for control flow graph
 */
CFGBuilder::CFGBuilder() { this->start_node_ = new CFGNode(new StmtEntity("-1")); }
/**
 * Build the control flow graph for a given procedure node
 * @param node procedure node
 * @return start node for the control flow graph
 */
CFGNode *CFGBuilder::Build(Node *node) {
  if (node->GetNodeType() != NodeType::kProcedure) {
    spdlog::error("CFGBuilder::Build: node is not a procedure node");
    return nullptr;
  }
  auto *proc_node = static_cast<ProcedureNode *>(node);
  auto *stmt_list_node = proc_node->GetStatementList();
  auto *terminal_node = new CFGNode(new StmtEntity("-1"));
  BuildBlock(stmt_list_node, start_node_, terminal_node);
  spdlog::debug("Control flow graph for procedure {} built", proc_node->GetProcName());
  Clean();
  spdlog::debug("Cleaned up loose ends in control flow graph...");
  return this->start_node_;
}
void CFGBuilder::Clean() {
  auto const op = [](CFGNode *node) {
    if (node->GetChildren().size() != 1) { return; }
    auto *immediate_child = static_cast<CFGNode *>(node->GetChildren()[0]);
    if (!immediate_child->IsTerminal()) { return; }
    if (immediate_child->GetChildren().size() != 1) {
      spdlog::debug("CFGBuilder::Clean: terminal node encountered {}", node->ToString());
      return;
    }
    auto *grand_child = immediate_child->GetChildren()[0];
    spdlog::debug("CFGBuilder::Clean: removing temp terminal node between {} and {}", node->ToString(),
                  grand_child->ToString());
    node->SetChildren({grand_child});
  };
  this->start_node_->VisitAll(op);
}
CFGNode *CFGBuilder::ToCFGNode(Node *node) {
  if (node->GetNodeType() != NodeType::kStatement) { return nullptr; }
  auto *stmt = static_cast<StatementNode *>(node);
  auto stmt_type = stmt->GetStmtType();
  auto stmt_no = stmt->GetStmtNo();
  auto *stmt_ent = new Entity(stmt_type, std::to_string(stmt_no));
  return new CFGNode(stmt_ent);
}
/**
 * Add the child node to the parent node
 * @param parent
 * @param child
 */
void CFGBuilder::ConnectNode(CFGNode *parent, CFGNode *child) { parent->AddChild(child); }
/**
 * Build a block of statements
 * @param node statement list node
 */
CFGNode *CFGBuilder::BuildBlock(Node *node, CFGNode *parent, CFGNode *terminal) {
  if (node->GetNodeType() != NodeType::kStatementList) {
    spdlog::error("CFGBuilder::BuildBlock: node is not a statement list node");
    return nullptr;
  }
  auto const op = [&parent](Node *node) {
    if (node->GetNodeType() != NodeType::kStatement) {
      spdlog::error("CFGBuilder::BuildBlock: node is not a statement node");
      return;
    }
    auto *child = ToCFGNode(node);
    auto *stmt_node = static_cast<StatementNode *>(node);
    ConnectNode(parent, child);
    switch (stmt_node->GetStmtType()) {
      case EntityType::kIfStmt: parent = BuildIf(node, child); break;
      case EntityType::kWhileStmt: parent = BuildWhile(node, child); break;
      default: parent = child; break;
    }
  };
  node->VisitChildren(op);
  ConnectNode(parent, terminal);
  return terminal;
}
CFGNode *CFGBuilder::BuildIf(Node *node, CFGNode *parent) {
  if (node->GetNodeType() != NodeType::kStatement) {
    spdlog::error("CFGBuilder::BuildIf: node is not a statement node");
    return nullptr;
  }
  auto *stmt_node = static_cast<StatementNode *>(node);
  if (stmt_node->GetStmtType() != EntityType::kIfStmt) {
    spdlog::error("CFGBuilder::BuildIf: node is not an if statement node");
    return nullptr;
  }
  auto *if_node = static_cast<IfNode *>(node);
  auto *then_node = if_node->GetThenStatementList();
  auto *else_node = if_node->GetElseStatementList();
  auto *terminal_node = new CFGNode(new StmtEntity("-1"));
  BuildBlock(then_node, parent, terminal_node);
  BuildBlock(else_node, parent, terminal_node);
  return terminal_node;
}
CFGNode *CFGBuilder::BuildWhile(Node *node, CFGNode *parent) {
  if (node->GetNodeType() != NodeType::kStatement) {
    spdlog::error("CFGBuilder::BuildWhile: node is not a statement node");
    return nullptr;
  }
  auto *stmt_node = static_cast<StatementNode *>(node);
  if (stmt_node->GetStmtType() != EntityType::kWhileStmt) {
    spdlog::error("CFGBuilder::BuildWhile: node is not an while statement node");
    return nullptr;
  }
  auto *while_node = static_cast<WhileNode *>(node);
  auto *stmt_list_node = while_node->GetStatementList();
  return BuildBlock(stmt_list_node, parent, parent);
}
