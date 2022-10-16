// Copyright 2022 CS3203 Team14. All rights reserved.

#pragma once

#include <unordered_set>

#include "commons/entity.h"

using EntityPointerUnorderedSet = std::unordered_set<Entity *, EntityHashFunction, EntityPointerEquality>;

enum class ReferenceType {
  kSynonym, kWildcard, kInteger, kIdent, kElem
};

enum class AttributeType {
  kProcName, kVarName, kValue, kStmtNo, kNone
};

inline std::string AttrToString(AttributeType type) {
  switch (type) {
    case AttributeType::kProcName: return "PROCNAME";
    case AttributeType::kVarName: return "VARNAME";
    case AttributeType::kValue: return "VALUE";
    case AttributeType::kStmtNo: return "STMT#";
    case AttributeType::kNone: return "NONE";
    default: return "Unknown";
  }
}

enum class Comparator {
  kEquals,
};