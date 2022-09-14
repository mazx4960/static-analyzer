// Copyright 2022 CS3203 Team14. All rights reserved.

#pragma once

#include <string>
#include <utility>
#include <iostream>

#include "types.h"
#include "commons/hash_combine.h"

class Entity {
 protected:
  EntityType type_;

  std::string name_;

 public:
  Entity(EntityType type, std::string name);
  [[nodiscard]] EntityType GetType() const;
  [[nodiscard]] std::string GetName() const;
  [[nodiscard]] virtual size_t GetHash() const;
  bool operator==(const Entity &other) const;
  std::string ToString();
};

class ProcedureEntity : public Entity {
 public:
  explicit ProcedureEntity(std::string name)
      : Entity(EntityType::kProcedure, std::move(name)) {}
};

class VariableEntity : public Entity {
 public:
  explicit VariableEntity(std::string name)
      : Entity(EntityType::kVariable, std::move(name)) {}
};

class ConstantEntity : public Entity {
 public:
  explicit ConstantEntity(std::string name)
      : Entity(EntityType::kConstant, std::move(name)) {}
};

class StatementEntity : public Entity {
 private:
  int stmt_no_;

 public:
  explicit StatementEntity(int stmt_no, EntityType entity_type = EntityType::kStatement);
  [[nodiscard]] int GetStmtNo() const;
  bool operator==(const StatementEntity &other) const;
};

class ReadStmtEntity : public StatementEntity {
 public:
  explicit ReadStmtEntity(int stmt_no)
      : StatementEntity(stmt_no, EntityType::kReadStmt) {}
};

class PrintStmtEntity : public StatementEntity {
 public:
  explicit PrintStmtEntity(int stmt_no)
      : StatementEntity(stmt_no, EntityType::kPrintStmt) {}
};

class AssignStmtEntity : public StatementEntity {
 public:
  explicit AssignStmtEntity(int stmt_no)
      : StatementEntity(stmt_no, EntityType::kAssignStmt) {}
};

class CallStmtEntity : public StatementEntity {
 public:
  explicit CallStmtEntity(int stmt_no)
      : StatementEntity(stmt_no, EntityType::kCallStmt) {}
};

class WhileStmtEntity : public StatementEntity {
 public:
  explicit WhileStmtEntity(int stmt_no)
      : StatementEntity(stmt_no, EntityType::kWhileStmt) {}
};

class IfStmtEntity : public StatementEntity {
 public:
  explicit IfStmtEntity(int stmt_no)
      : StatementEntity(stmt_no, EntityType::kIfStmt) {}
};

class EntityHashFunction {
 public:
  size_t operator()(const Entity &entity) const {
    return entity.GetHash();
  }
  size_t operator()(const Entity *entity) const {
    return entity->GetHash();
  }
};

// Usage:
//std::unordered_map<
//    Entity *,
//    std::unordered_set<Entity *, EntityHashFunction>,
//    EntityHashFunction
//>
