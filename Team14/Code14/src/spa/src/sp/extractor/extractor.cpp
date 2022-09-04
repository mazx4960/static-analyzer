// Copyright 2022 CS3203 Team14. All rights reserved.

#include "extractor.h"

#include "traverser.h"

std::vector<Entity> EntityExtractor::extract() {
  std::vector<Entity> entities;
  auto op = [entities](SimpleAstNode *node) {
    if (node->GetNodeType() == SimpleNodeType::ASSIGN) {
      entities.push_back(AssignEntity(node->GetLineNumber()));
    } else if (node->GetNodeType() == SimpleNodeType::CALL) {
      entities.push_back(Entity(node->get_name(), EntityType::CALL));
    } else if (node->GetNodeType() == SimpleNodeType::CONSTANT) {
      entities.push_back(Entity(node->get_name(), EntityType::CONSTANT));
    } else if (node->GetNodeType() == SimpleNodeType::IF) {
      entities.push_back(Entity(node->get_name(), EntityType::IF));
    } else if (node->GetNodeType() == SimpleNodeType::PROCEDURE) {
      entities.push_back(Entity(node->get_name(), EntityType::PROCEDURE));
    } else if (node->GetNodeType() == SimpleNodeType::PROGLINE) {
      entities.push_back(Entity(node->get_name(), EntityType::PROGLINE));
    } else if (node->GetNodeType() == SimpleNodeType::STMT) {
      entities.push_back(Entity(node->get_name(), EntityType::STMT));
    } else if (node->GetNodeType() == SimpleNodeType::STMTLST) {
      entities.push_back(Entity(node->get_name(), EntityType::STMTLST));
    } else if (node->GetNodeType() == SimpleNodeType::VARIABLE) {
      entities.push_back(Entity(node->get_name(), EntityType::VARIABLE));
    } else if (node->GetNodeType() == SimpleNodeType::WHILE) {
      entities.push_back(Entity(node->get_name(), EntityType::WHILE));
    }
  };
  Traverser::TraverseProgram(program_node_, op);
  return entities;
}
