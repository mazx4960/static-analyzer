// Copyright 2022 CS3203 Team14. All rights reserved.

#pragma once

#include <string>
#include <utility>

#include "types.h"
#include "commons/hash_combine.h"

class Entity {
 private:
  EntityType type_;

  std::string name_;

 public:
  Entity(EntityType type, std::string name);
  [[nodiscard]] EntityType GetType() const;
  [[nodiscard]] std::string GetName() const;
  [[nodiscard]] virtual size_t GetHash() const;
  bool operator==(const Entity &other) const;
  bool operator==(const Entity *other) const;
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

  EntityType stmt_type_;

 public:
  explicit StatementEntity(int stmt_no);
  [[nodiscard]] int GetStmtNo() const;
  [[nodiscard]] size_t GetHash() const override;  // Overrides Entity's GetHash() function
  bool operator==(const StatementEntity &other) const;
};

class ReadEntity : public StatementEntity {
 public:
  explicit ReadEntity(int stmt_no)
      : StatementEntity(stmt_no) {}
};

class PrintEntity : public StatementEntity {
 public:
  explicit PrintEntity(int stmt_no)
      : StatementEntity(stmt_no) {}
};

class AssignEntity : public StatementEntity {
 public:
  explicit AssignEntity(int stmt_no)
      : StatementEntity(stmt_no) {}
};

class CallEntity : public StatementEntity {
 public:
  explicit CallEntity(int stmt_no)
      : StatementEntity(stmt_no) {}
};

class WhileEntity : public StatementEntity {
 public:
  explicit WhileEntity(int stmt_no)
      : StatementEntity(stmt_no) {}
};

class IfEntity : public StatementEntity {
 public:
  explicit IfEntity(int stmt_no)
      : StatementEntity(stmt_no) {}
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
