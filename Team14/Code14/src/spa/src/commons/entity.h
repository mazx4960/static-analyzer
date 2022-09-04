// Copyright 2022 CS3203 Team14. All rights reserved.

#pragma once

#include <string>
#include <utility>

enum class EntityType {
  kProcedure,
  kStatement,
  kVariable,
  kConstant,
};

class Entity {
 private:
  EntityType type_;
  std::string name_;

 public:
  Entity(EntityType type, std::string name);
  EntityType GetType();
  std::string GetName();
  bool operator==(const Entity &other) const;
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

enum class StmtType {
  kAssign,
  kCall,
  kIf,
  kWhile,
  kPrint,
  kRead,
};

class StatementEntity : public Entity {
 private:
  int stmt_no_;
  StmtType stmt_type_;

 public:
  explicit StatementEntity(StmtType stmt_type, int stmt_no);
  StmtType GetStmtType();
  [[nodiscard]] int GetStmtNo() const;
  bool operator==(const StatementEntity &other) const;
};

class ReadEntity : public StatementEntity {
 public:
  explicit ReadEntity(int stmt_no)
      : StatementEntity(StmtType::kRead, stmt_no) {}
};

class PrintEntity : public StatementEntity {
 public:
  explicit PrintEntity(int stmt_no)
      : StatementEntity(StmtType::kPrint, stmt_no) {}
};

class AssignEntity : public StatementEntity {
 public:
  explicit AssignEntity(int stmt_no)
      : StatementEntity(StmtType::kAssign, stmt_no) {}
};

class CallEntity : public StatementEntity {
 public:
  explicit CallEntity(int stmt_no)
      : StatementEntity(StmtType::kCall, stmt_no) {}
};

class WhileEntity : public StatementEntity {
 public:
  explicit WhileEntity(int stmt_no)
      : StatementEntity(StmtType::kWhile, stmt_no) {}
};

class IfEntity : public StatementEntity {
 public:
  explicit IfEntity(int stmt_no)
      : StatementEntity(StmtType::kIf, stmt_no) {}
};
