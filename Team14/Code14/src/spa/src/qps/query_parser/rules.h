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

class Rules {
 private:
  RefTypes stmt_ref_ = {ReferenceType::kSynonym, ReferenceType::kWildcard, ReferenceType::kInteger};
  RefTypes ent_ref_ = {ReferenceType::kSynonym, ReferenceType::kWildcard, ReferenceType::kIdent};
  SyntaxRules such_rules_ =
      {{RsType::kFollows, {{T::kStmtRef, T::kStmtRef}}}, {RsType::kFollowsT, {{T::kStmtRef, T::kStmtRef}}},
       {RsType::kParent, {{T::kStmtRef, T::kStmtRef}}}, {RsType::kParentT, {{T::kStmtRef, T::kStmtRef}}},
       {RsType::kModifies, {{T::kStmtRef, T::kEntRef}, {T::kEntRef, T::kEntRef}}},
       {RsType::kUses, {{T::kStmtRef, T::kEntRef}, {T::kEntRef, T::kEntRef}}},
       {RsType::kCalls, {{T::kEntRef, T::kEntRef}}}, {RsType::kCallsT, {{T::kEntRef, T::kEntRef}}},
       {RsType::kNext, {{T::kStmtRef, T::kStmtRef}}}, {RsType::kNextT, {{T::kStmtRef, T::kStmtRef}}},
       {RsType::kAffects, {{T::kStmtRef, T::kStmtRef}}}, {RsType::kAffectsT, {{T::kStmtRef, T::kStmtRef}}},};

  bool IsStmtRef(ReferenceType ref_type) const {
    return std::find(stmt_ref_.begin(), stmt_ref_.end(), ref_type) != stmt_ref_.end();
  };
  bool IsEntRef(ReferenceType ref_type) const {
    return std::find(ent_ref_.begin(), ent_ref_.end(), ref_type) != ent_ref_.end();
  };
  bool IsValid(T target, ReferenceType ref_type) const {
    if (target == T::kStmtRef) {
      return IsStmtRef(ref_type);
    }
    if (target == T::kEntRef) {
      return IsEntRef(ref_type);
    }
    return false;
  };

 public:
  bool CheckSuchThat(RsType type, ReferenceType first, ReferenceType second) {
    if (such_rules_.find(type) == such_rules_.end()) {
      return false;
    }
    auto rule = such_rules_[type];
    return std::any_of(rule.begin(), rule.end(), [first, second, this](auto pair) {
      return IsValid(pair.first, first) && IsValid(pair.second, second);
    });
  };
};