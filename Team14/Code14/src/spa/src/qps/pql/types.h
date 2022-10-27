// Copyright 2022 CS3203 Team14. All rights reserved.

#pragma once

#include <unordered_set>

#include "commons/entity.h"

enum class SelectType {
  kElem, kBoolean
};

enum class ClauseType {
  kSuchThat, kPattern, kWith
};

enum class ReferenceType {
  kSynonym, kWildcard, kInteger, kIdent, kAttr
};

enum class AttributeType {
  kProcName, kVarName, kValue, kStmtNo, kNone
};

enum class Comparator {
  kEquals,
};

inline std::string SelectTypeToString(SelectType type) {
  switch (type) {
    case SelectType::kElem:
      return "Elem";
    case SelectType::kBoolean:
      return "Boolean";
    default:
      return "Unknown";
  }
}

inline std::string RefTypeToString(ReferenceType type) {
  switch (type) {
    case ReferenceType::kSynonym:
      return "Synonym";
    case ReferenceType::kWildcard:
      return "Wildcard";
    case ReferenceType::kInteger:
      return "Integer";
    case ReferenceType::kIdent:
      return "Ident";
    case ReferenceType::kAttr:
      return "Attr";
    default:
      return "Unknown";
  }
}

inline ReferenceType AttrToRefType(AttributeType type) {
  switch (type) {
    case AttributeType::kProcName:
    case AttributeType::kVarName:
      return ReferenceType::kIdent;
    case AttributeType::kValue:
    case AttributeType::kStmtNo:
      return ReferenceType::kInteger;
    default:
      return ReferenceType::kWildcard;
  }
}

inline std::string AttrToString(AttributeType type) {
  switch (type) {
    case AttributeType::kProcName:
      return "PROCNAME";
    case AttributeType::kVarName:
      return "VARNAME";
    case AttributeType::kValue:
      return "VALUE";
    case AttributeType::kStmtNo:
      return "STMT#";
    case AttributeType::kNone:
      return "NONE";
    default:
      return "Unknown";
  }
}
