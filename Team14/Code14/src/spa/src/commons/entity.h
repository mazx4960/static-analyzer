// Copyright 2022 CS3203 Team14. All rights reserved.

#pragma once

#include <string>
#include <utility>

#include "commons/hash_combine.h"
#include "types.h"

class Entity {
 protected:
  EntityType type_;

  std::string value_;

 public:
  Entity(EntityType type, std::string value);
  [[nodiscard]] EntityType GetType() const;
  [[nodiscard]] std::string GetValue() const;
  [[nodiscard]] virtual size_t GetHash() const;
  bool operator==(const Entity &other) const;
  std::string ToString();
};

class ProcedureEntity : public Entity {
 public:
  explicit ProcedureEntity(std::string name) : Entity(EntityType::kProcedure, std::move(name)) {}
};

class VariableEntity : public Entity {
 public:
  explicit VariableEntity(std::string name) : Entity(EntityType::kVariable, std::move(name)) {}
};

class ConstantEntity : public Entity {
 public:
  explicit ConstantEntity(std::string name) : Entity(EntityType::kConstant, std::move(name)) {}
};

class ReadStmtEntity : public Entity {
 public:
  explicit ReadStmtEntity(std::string stmt_no) : Entity(EntityType::kReadStmt, std::move(stmt_no)) {}
};

class PrintStmtEntity : public Entity {
 public:
  explicit PrintStmtEntity(std::string stmt_no) : Entity(EntityType::kPrintStmt, std::move(stmt_no)) {}
};

class AssignStmtEntity : public Entity {
 public:
  explicit AssignStmtEntity(std::string stmt_no) : Entity(EntityType::kAssignStmt, std::move(stmt_no)) {}
};

class CallStmtEntity : public Entity {
 public:
  explicit CallStmtEntity(std::string stmt_no) : Entity(EntityType::kCallStmt, std::move(stmt_no)) {}
};

class WhileStmtEntity : public Entity {
 public:
  explicit WhileStmtEntity(std::string stmt_no) : Entity(EntityType::kWhileStmt, std::move(stmt_no)) {}
};

class IfStmtEntity : public Entity {
 public:
  explicit IfStmtEntity(std::string stmt_no) : Entity(EntityType::kIfStmt, std::move(stmt_no)) {}
};

/**
 * Hash function for Entity
 *
 * Usage: std::unordered_map<Entity *, std::unordered_set<Entity *, EntityHashFunction>, EntityHashFunction>
 */
class EntityHashFunction {
 public:
  size_t operator()(const Entity &entity) const { return entity.GetHash(); }
  size_t operator()(const Entity *entity) const { return entity->GetHash(); }
};
