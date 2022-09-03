// Copyright 2022 CS3203 Team14. All rights reserved.

#pragma once

#include <string>

enum class EntityType {
  Procedure,
  Statement,
  Variable,
  Constant,
};

class Entity {
 private:
  EntityType type;
  std::string name;

 public:
  Entity(EntityType type, std::string name);
  EntityType GetType();
  std::string GetName();
  bool operator==(const Entity &other) const;
};

class ProcedureEntity : public Entity {
 public:
  explicit ProcedureEntity(std::string name)
      : Entity(EntityType::Procedure, std::move(name)) {}
};

class VariableEntity : public Entity {
 public:
  explicit VariableEntity(std::string name)
      : Entity(EntityType::Variable, std::move(name)) {}
};

class ConstantEntity : public Entity {
 public:
  explicit ConstantEntity(std::string name)
      : Entity(EntityType::Constant, std::move(name)) {}
};

enum class StmtType {
  Assign,
  Call,
  If,
  While,
  Print,
  Read,
};

class StatementEntity : public Entity {
 private:
  int stmt_no;
  StmtType stmt_type;

 public:
  explicit StatementEntity(StmtType stmt_type, int stmt_no);
  StmtType GetStmtType();
  [[nodiscard]] int GetStmtNo() const;
  bool operator==(const StatementEntity &other) const;
};

class ReadEntity : public StatementEntity {
 public:
  explicit ReadEntity(int stmt_no)
      : StatementEntity(StmtType::Read, stmt_no) {}
};

class PrintEntity : public StatementEntity {
 public:
  explicit PrintEntity(int stmt_no)
      : StatementEntity(StmtType::Print, stmt_no) {}
};

class AssignEntity : public StatementEntity {
 public:
  explicit AssignEntity(int stmt_no)
      : StatementEntity(StmtType::Assign, stmt_no) {}
};

class CallEntity : public StatementEntity {
 public:
  explicit CallEntity(int stmt_no)
      : StatementEntity(StmtType::Call, stmt_no) {}
};

class WhileEntity : public StatementEntity {
 public:
  explicit WhileEntity(int stmt_no)
      : StatementEntity(StmtType::While, stmt_no) {}
};

class IfEntity : public StatementEntity {
 public:
  explicit IfEntity(int stmt_no)
      : StatementEntity(StmtType::If, stmt_no) {}
};
