// Copyright 2022 CS3203 Team14. All rights reserved.

#pragma once

#include <string>
#include <vector>

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
  kFollowsAll,
  kParent,
  kParentAll,
  kUses,
  kModifies,
  // Used by PQL,
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
    case EntityType::kWildcard: return "Wildcard";
    case EntityType::kExpression: return "Expression";
    case EntityType::kWildcardExpression: return "WildcardExpression";
    case EntityType::kString: return "String";
    case EntityType::kInteger: return "Integer";
    default: return "Unknown";
  }
}

inline std::string RsTypeToString(RsType type) {
  switch (type) {
    case RsType::kFollows: return "Follows";
    case RsType::kParent: return "Parent";
    case RsType::kUses: return "Uses";
    case RsType::kModifies: return "Modifies";
    case RsType::kParentAll: return "Parent*";
    case RsType::kFollowsAll: return "Follows*";
    case RsType::kAssignPattern: return "pattern";
    default: return "Unknown";
  }
}

inline std::vector<EntityType> all_stmt_types = {EntityType::kAssignStmt, EntityType::kCallStmt,
                                                 EntityType::kIfStmt,     EntityType::kPrintStmt,
                                                 EntityType::kReadStmt,   EntityType::kWhileStmt};
inline std::vector<EntityType> GetAllStmtTypes() { return all_stmt_types; }
