// Copyright 2022 CS3203 Team14. All rights reserved.

#pragma once

#include <string>

enum class EntityType {
  kProcedure,
  kStatement,
  kVariable,
  kConstant,
  // Statement types
  kAssignStmt,
  kCallStmt,
  kIfStmt,
  kWhileStmt,
  kPrintStmt,
  kReadStmt,
  // Used by PQL
  kWildcard,
  kExpression,
  kWildcardExpression,
  kString,
  kInteger,
};

enum class RsType {
  kFollows,
  kParent,
  kUses,
  kModifies,
  // Used by PQL
  kParentAll,
  kFollowsAll,
  kAssignPattern,
};

inline std::string EntityTypeToString(EntityType type) {
  switch (type) {
    case EntityType::kProcedure: return "Procedure";
    case EntityType::kStatement: return "Statement";
    case EntityType::kVariable: return "Variable";
    case EntityType::kConstant: return "Constant";
    case EntityType::kAssignStmt: return "AssignStmt";
    case EntityType::kCallStmt: return "CallStmt";
    case EntityType::kIfStmt: return "IfStmt";
    case EntityType::kWhileStmt: return "WhileStmt";
    case EntityType::kPrintStmt: return "PrintStmt";
    case EntityType::kReadStmt: return "ReadStmt";
    default: return "Unknown";
  }
}

inline std::string RsTypeToString(RsType type) {
  switch (type) {
    case RsType::kFollows: return "Follows";
    case RsType::kParent: return "Parent";
    case RsType::kUses: return "Uses";
    case RsType::kModifies: return "Modifies";
    default: return "Unknown";
  }
}