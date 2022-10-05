// Copyright 2022 CS3203 Team14. All rights reserved.

#pragma once

#include <string>
#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <utility>

#include "commons/types.h"

using EntityTypeSet = std::unordered_set<EntityType>;
using RelationshipSemanticRule = std::vector<EntityTypeSet>;
using SuchThatSemanticRules = std::unordered_map<RsType, RelationshipSemanticRule>;
using PatternSemanticRules = std::vector<EntityTypeSet>;

inline static EntityTypeSet stmt_ref_types = {EntityType::kAssignStmt, EntityType::kCallStmt,
                                              EntityType::kIfStmt, EntityType::kPrintStmt,
                                              EntityType::kReadStmt, EntityType::kWhileStmt,
                                              EntityType::kWildcard, EntityType::kInteger,
                                              EntityType::kStatement};
inline static EntityTypeSet uses_ref_types = {EntityType::kAssignStmt, EntityType::kCallStmt,
                                              EntityType::kIfStmt, EntityType::kPrintStmt,
                                              EntityType::kProcedure, EntityType::kWhileStmt,
                                              EntityType::kIdent, EntityType::kInteger,
                                              EntityType::kStatement};
inline static EntityTypeSet modifies_ref_types = {EntityType::kAssignStmt, EntityType::kCallStmt,
                                                  EntityType::kIfStmt, EntityType::kReadStmt,
                                                  EntityType::kProcedure, EntityType::kWhileStmt,
                                                  EntityType::kIdent, EntityType::kInteger,
                                                  EntityType::kStatement};
inline static EntityTypeSet ent_ref_variable_type = {EntityType::kVariable, EntityType::kIdent,
                                                     EntityType::kWildcard};
inline static EntityTypeSet ent_ref_procedure_type = {EntityType::kProcedure, EntityType::kIdent,
                                                     EntityType::kWildcard};

inline static EntityTypeSet empty_wildcard_types = {};
inline static EntityTypeSet stmt_ref_wildcard_types = {EntityType::kStatement};
inline static EntityTypeSet ent_ref_variable_wildcard_type = {EntityType::kVariable};
inline static EntityTypeSet ent_ref_procedure_wildcard_type = {EntityType::kProcedure};

inline static EntityTypeSet syn_assign_types = {EntityType::kAssignStmt};
inline static EntityTypeSet expression_types = {EntityType::kExpression, EntityType::kWildcardExpression};

inline static RelationshipSemanticRule follows_args_rules = {stmt_ref_types, stmt_ref_types,
                                                             stmt_ref_wildcard_types, stmt_ref_wildcard_types};

inline static RelationshipSemanticRule parent_args_rules = {stmt_ref_types, stmt_ref_types,
                                                            stmt_ref_wildcard_types, stmt_ref_wildcard_types};

inline static RelationshipSemanticRule uses_args_rules = {uses_ref_types, ent_ref_variable_type,
                                                          empty_wildcard_types, ent_ref_variable_wildcard_type};
inline static RelationshipSemanticRule modifies_args_rules = {modifies_ref_types, ent_ref_variable_type,
                                                              empty_wildcard_types, ent_ref_variable_wildcard_type};
inline static RelationshipSemanticRule calls_args_rules = {ent_ref_procedure_type, ent_ref_procedure_type,
                                                           ent_ref_procedure_wildcard_type,
                                                           ent_ref_procedure_wildcard_type};
inline static RelationshipSemanticRule next_args_rules = {stmt_ref_types, stmt_ref_types,
                                                          stmt_ref_wildcard_types, stmt_ref_wildcard_types};

inline static PatternSemanticRules pattern_args_rule = {syn_assign_types, ent_ref_variable_type, expression_types,
                                                        ent_ref_variable_wildcard_type};

inline static SuchThatSemanticRules such_that_semantic_rules = {{RsType::kFollows, follows_args_rules},
                                                                {RsType::kFollowsT, follows_args_rules},
                                                                {RsType::kParent, parent_args_rules},
                                                                {RsType::kParentT, parent_args_rules},
                                                                {RsType::kUses, uses_args_rules},
                                                                {RsType::kModifies, modifies_args_rules},
                                                                {RsType::kCalls, calls_args_rules},
                                                                {RsType::kCallsT, calls_args_rules},
                                                                {RsType::kNext, next_args_rules},
                                                                {RsType::kNextT, next_args_rules}};

