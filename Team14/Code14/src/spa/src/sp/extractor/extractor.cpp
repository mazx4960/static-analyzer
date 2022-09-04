// Copyright 2022 CS3203 Team14. All rights reserved.

#include "extractor.h"

#include "traverser.h"

std::vector<Entity *> EntityExtractor::extract() {
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
  Traverser::TraverseProgram(program_node_, op);
  return entities;
}
