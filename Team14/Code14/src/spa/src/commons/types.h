// Copyright 2022 CS3203 Team14. All rights reserved.

#pragma once

#include <string>
#include <unordered_set>

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
  kUnknown,
};

enum class RsType {
  kFollows,
  kFollowsT,
  kParent,
  kParentT,
  kUses,
  kModifies,
  kCalls,
  kCallsT,
  kNext,
  kNextT,
  kAffects,
  kAffectsT,
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
    default: return "Unknown";
  }
}

inline std::string RsTypeToString(RsType type) {
  switch (type) {
    case RsType::kFollows: return "Follows";
    case RsType::kFollowsT: return "Follows*";
    case RsType::kParent: return "Parent";
    case RsType::kParentT: return "Parent*";
    case RsType::kUses: return "Uses";
    case RsType::kModifies: return "Modifies";
    case RsType::kCalls: return "Calls";
    case RsType::kCallsT: return "Calls*";
    case RsType::kNext: return "Next";
    case RsType::kNextT: return "Next*";
    case RsType::kAffects: return "Affects";
    case RsType::kAffectsT:return "Affects*";
    case RsType::kAssignPattern: return "pattern";
    default: return "Unknown";
  }
}

inline std::unordered_set<EntityType> all_stmt_types = {EntityType::kAssignStmt, EntityType::kCallStmt,
                                                        EntityType::kIfStmt, EntityType::kPrintStmt,
                                                        EntityType::kReadStmt, EntityType::kWhileStmt};

inline std::unordered_set<EntityType> GetAllStmtTypes() {
  return all_stmt_types;
}