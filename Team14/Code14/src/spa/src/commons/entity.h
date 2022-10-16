// Copyright 2022 CS3203 Team14. All rights reserved.

#pragma once

#include <algorithm>
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
  explicit ProcedureEntity(std::string name) : Entity(EntityType::kProcedure, std::move(name)) {
  }
};

class VariableEntity : public Entity {
 public:
  explicit VariableEntity(std::string name) : Entity(EntityType::kVariable, std::move(name)) {
  }
};

class ConstantEntity : public Entity {
 public:
  explicit ConstantEntity(std::string name) : Entity(EntityType::kConstant, std::move(name)) {
  }
};

class ReadStmtEntity : public Entity {
 public:
  explicit ReadStmtEntity(std::string stmt_no) : Entity(EntityType::kReadStmt, std::move(stmt_no)) {
  }
};

class StmtEntity : public Entity {
 private:
  std::string stmt_no_;

 protected:
  std::string value_;
 public:
  explicit StmtEntity(EntityType entity_type, std::string stmt_no, std::string value) : Entity(entity_type,
                                                                                               std::move(stmt_no)),
                                                                                        stmt_no_(stmt_no),
                                                                                        value_(std::move(value)) {
  }
};

class PrintStmtEntity : public StmtEntity {
 public:
  explicit PrintStmtEntity(std::string stmt_no) : StmtEntity(EntityType::kPrintStmt, std::move(stmt_no), "") {
  }
};

class AssignStmtEntity : public StmtEntity {
 public:
  explicit AssignStmtEntity(std::string stmt_no) : StmtEntity(EntityType::kAssignStmt, std::move(stmt_no), "") {
  }
};

class CallStmtEntity : public StmtEntity {
 public:
  explicit CallStmtEntity(std::string stmt_no, std::string proc_name) : StmtEntity(EntityType::kCallStmt,
                                                                                   std::move(stmt_no),
                                                                                   std::move(proc_name)) {
  }
  [[nodiscard]] std::string GetProcName() const;
};

class WhileStmtEntity : public StmtEntity {
 public:
  explicit WhileStmtEntity(std::string stmt_no) : StmtEntity(EntityType::kWhileStmt, std::move(stmt_no), "") {
  }
};

class IfStmtEntity : public StmtEntity {
 public:
  explicit IfStmtEntity(std::string stmt_no) : StmtEntity(EntityType::kIfStmt, std::move(stmt_no), "") {
  }
};

/**
 * Hash function for Entity
 *
 * Usage: std::unordered_map<Entity *, std::unordered_set<Entity *, EntityHashFunction, EntityPointerEquality>, EntityHashFunction, EntityPointerEquality>
 */
struct EntityPointerEquality {
  bool operator()(Entity const *lhs, Entity const *rhs) const {
    return lhs->GetType() == rhs->GetType() && lhs->GetValue() == rhs->GetValue();
  }
};

struct EntityHashFunction {
  size_t operator()(const Entity &entity) const {
    return entity.GetHash();
  }
  size_t operator()(const Entity *entity) const {
    return entity->GetHash();
  }
};

struct EntityPointerComparator {
  /**
   * Order two entities by value (less than or equals to)
   * @param lhs
   * @param rhs
   * @return true if lhs <= rhs
   */
  static bool le(const Entity *lhs, const Entity *rhs) {
    std::string left_val = lhs->GetValue();
    std::string right_val = rhs->GetValue();
    if (isNumber(left_val) && isNumber(right_val)) {
      return std::stoi(left_val) <= std::stoi(right_val);
    }
    return left_val < right_val;
  }

  /**
   * Order two entities by value (strictly less than)
   * @param lhs
   * @param rhs
   * @return true if lhs < rhs
   */
  static bool lt(const Entity *lhs, const Entity *rhs) {
    std::string left_val = lhs->GetValue();
    std::string right_val = rhs->GetValue();
    if (isNumber(left_val) && isNumber(right_val)) {
      return std::stoi(left_val) < std::stoi(right_val);
    }
    return left_val < right_val;
  }

  /**
   * Order two entities by value (strictly greater than)
   * @param lhs
   * @param rhs
   * @return true if lhs > rhs
   */
  static bool gt(const Entity *lhs, const Entity *rhs) {
    std::string left_val = lhs->GetValue();
    std::string right_val = rhs->GetValue();
    if (isNumber(left_val) && isNumber(right_val)) {
      return std::stoi(left_val) > std::stoi(right_val);
    }
    return left_val > right_val;
  }

 private:
  static bool isNumber(const std::string &s) {
    return std::all_of(s.begin(), s.end(), ::isdigit);
  }
};
