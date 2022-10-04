// Copyright 2022 CS3203 Team14. All rights reserved.

#pragma once

#include <string>
#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <utility>

#include "commons/types.h"

using EntityTypeSet = std::unordered_set<EntityType>;
using RelationshipRules = std::vector<EntityTypeSet>;
using SuchThatRules = std::unordered_map<RsType, RelationshipRules>;
using PatternRules = std::vector<EntityTypeSet>;

inline static EntityTypeSet stmt_ref_types = {EntityType::kAssignStmt, EntityType::kCallStmt,
                                              EntityType::kIfStmt, EntityType::kPrintStmt,
                                              EntityType::kReadStmt, EntityType::kWhileStmt,
                                              EntityType::kWildcardStmt, EntityType::kStatement,
                                              EntityType::kInteger};

inline static EntityTypeSet uses_ref_types = {EntityType::kAssignStmt, EntityType::kPrintStmt,
                                              EntityType::kIfStmt, EntityType::kWhileStmt,
                                              EntityType::kProcedure, EntityType::kCallStmt,
                                              EntityType::kString, EntityType::kInteger};

inline static EntityTypeSet modifies_ref_types = {EntityType::kAssignStmt, EntityType::kReadStmt,
                                                  EntityType::kIfStmt, EntityType::kWhileStmt,
                                                  EntityType::kProcedure, EntityType::kCallStmt,
                                                  EntityType::kString, EntityType::kInteger};

inline static EntityTypeSet syn_assign_types = {EntityType::kAssignStmt};
inline static EntityTypeSet ent_ref_variable_type = {EntityType::kVariable, EntityType::kString,
                                                     EntityType::kWildcardEnt};
inline static EntityTypeSet expression_types = {EntityType::kExpression, EntityType::kWildcardExpression};

inline static RelationshipRules follows_args_rule = {stmt_ref_types, stmt_ref_types};
inline static RelationshipRules follows_all_args_rule = {stmt_ref_types, stmt_ref_types};
inline static RelationshipRules parent_args_rule = {stmt_ref_types, stmt_ref_types};
inline static RelationshipRules parent_all_args_rule = {stmt_ref_types, stmt_ref_types};
inline static RelationshipRules uses_args_rule = {uses_ref_types, ent_ref_variable_type};
inline static RelationshipRules modifies_args_rule = {modifies_ref_types, ent_ref_variable_type};

inline static PatternRules pattern_args_rule = {syn_assign_types, ent_ref_variable_type, expression_types};

inline static SuchThatRules such_that_rules = {{RsType::kFollows, follows_args_rule},
                                               {RsType::kFollowsT, follows_all_args_rule},
                                               {RsType::kParent, parent_args_rule},
                                               {RsType::kParentT, parent_all_args_rule},
                                               {RsType::kUses, uses_args_rule},
                                               {RsType::kModifies, modifies_args_rule}};

inline SuchThatRules getSuchThatRules() {
  return such_that_rules;
}

inline PatternRules getPatternRules() {
  return pattern_args_rule;
}