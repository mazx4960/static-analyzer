// Copyright 2022 CS3203 Team14. All rights reserved.
#include "pattern_extractor.h"

#include <iostream>

#include "sp/simple_definition/simple_ast.h"

std::vector<Pattern *> PatternExtractor::ExtractAll(Node *node) {
  std::vector<Pattern *> patterns;
  auto const op = [&patterns](Node *node) {
    if (node->GetNodeType() == NodeType::kStatement
        && static_cast<StatementNode*>(node)->GetStmtType() == StmtType::kAssign) {
      auto *assign = static_cast<AssignNode*>(node);
      patterns.push_back(new Pattern(assign->GetVariable()->GetVariableName(), assign->GetExpression()->ToString()));
    }
  };
  node->VisitAll(op);
  return patterns;
}
