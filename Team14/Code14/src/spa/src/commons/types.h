// Copyright 2022 CS3203 Team14. All rights reserved.

#pragma once

enum class EntityType {
  kProcedure,
  kStatement,
  kVariable,
  kConstant,
};

enum class StmtType {
  kAssign,
  kCall,
  kIf,
  kWhile,
  kPrint,
  kRead,
};

enum class RsType {
  kFollows,
  kParent,
  kUses,
  kModifies,
};

class TypeError : public std::runtime_error {
 protected:
  explicit TypeError(const std::string &message) : std::runtime_error(message) {}
};

class EntityTypeError : public TypeError {
 public:
  explicit EntityTypeError(const std::string &message) : TypeError(message) {}
};

class StmtTypeError : public TypeError {
 public:
  explicit StmtTypeError(const std::string &message) : TypeError(message) {}
};

class RsTypeError : public TypeError {
 public:
  explicit RsTypeError(const std::string &message) : TypeError(message) {}
};