// Copyright 2022 CS3203 Team14. All rights reserved.

#pragma once

#include <vector>
#include <unordered_map>
#include "qps/pql/types.h"

enum class T {
  kEntRef, kStmtRef
};

using RefTypes = std::vector<ReferenceType>;
using SyntaxRules = std::unordered_map<RsType, std::vector<std::pair<T, T>>>;

inline RefTypes stmt_references = {ReferenceType::kSynonym, ReferenceType::kWildcard, ReferenceType::kInteger};
inline RefTypes ent_references = {ReferenceType::kSynonym, ReferenceType::kWildcard, ReferenceType::kIdent};
inline RefTypes attr_references = {ReferenceType::kAttr, ReferenceType::kIdent, ReferenceType::kInteger};
inline SyntaxRules such_that_templates =
    {{RsType::kFollows, {{T::kStmtRef, T::kStmtRef}}}, {RsType::kFollowsT, {{T::kStmtRef, T::kStmtRef}}},
     {RsType::kParent, {{T::kStmtRef, T::kStmtRef}}}, {RsType::kParentT, {{T::kStmtRef, T::kStmtRef}}},
     {RsType::kModifies, {{T::kStmtRef, T::kEntRef}, {T::kEntRef, T::kEntRef}}},
     {RsType::kUses, {{T::kStmtRef, T::kEntRef}, {T::kEntRef, T::kEntRef}}},
     {RsType::kCalls, {{T::kEntRef, T::kEntRef}}}, {RsType::kCallsT, {{T::kEntRef, T::kEntRef}}},
     {RsType::kNext, {{T::kStmtRef, T::kStmtRef}}}, {RsType::kNextT, {{T::kStmtRef, T::kStmtRef}}},
     {RsType::kAffects, {{T::kStmtRef, T::kStmtRef}}}, {RsType::kAffectsT, {{T::kStmtRef, T::kStmtRef}}},};

inline bool IsStmtRef(ReferenceType ref_type) {
  return std::find(stmt_references.begin(), stmt_references.end(), ref_type) != stmt_references.end();
};
inline bool IsEntRef(ReferenceType ref_type) {
  return std::find(ent_references.begin(), ent_references.end(), ref_type) != ent_references.end();
};
inline bool IsAttrRef(ReferenceType ref_type) {
  return std::find(attr_references.begin(), attr_references.end(), ref_type) != attr_references.end();
};
inline bool IsValid(T target, ReferenceType ref_type) {
  if (target == T::kStmtRef) {
    return IsStmtRef(ref_type);
  }
  if (target == T::kEntRef) {
    return IsEntRef(ref_type);
  }
  return false;
};

inline bool CheckSuchThat(RsType type, ReferenceType first, ReferenceType second) {
  if (such_that_templates.find(type) == such_that_templates.end()) {
    return false;
  }
  auto rule = such_that_templates[type];
  return std::any_of(rule.begin(), rule.end(), [first, second](auto pair) {
    return IsValid(pair.first, first) && IsValid(pair.second, second);
  });
};
inline bool CheckPattern(ReferenceType var) {
  return IsEntRef(var);
};
inline bool CheckWith(ReferenceType first, ReferenceType second) {
  return IsAttrRef(first) && IsAttrRef(second);
};