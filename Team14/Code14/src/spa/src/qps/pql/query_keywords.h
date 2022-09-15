// Copyright 2022 CS3203 Team14. All rights reserved.

#pragma once

#include <utility>
#include <vector>

#include "query_call.h"
#include "query_declaration.h"

class QueryKeywords {
 private:
  inline static const std::unordered_set<std::string> kCallKeywords {"Select"};
  inline static const std::unordered_set<std::string> kDeclarationKeywords {
      "stmt", "read", "print", "call", "while", "if", "assign", "variable", "constant", "procedure"};
  inline static const std::unordered_set<std::string> kSuchThatKeywords {
      "such", "that"};
  inline static const std::unordered_set<std::string> kClauseKeywords {
      "Follows", "Parent", "Uses", "Modifies", "pattern"};
  inline static const std::unordered_set<std::string> kAssignOperators = {
      "+", "-", "*", "/", "%", "<", ">"};
 public:
  static bool isValidKeyword(const std::string& keyword);
  static bool isValidSuchThatKeyword(const std::string& keyword);
  static bool isValidDeclarationKeyword(const std::string& keyword);
  static bool isValidCallKeyword(const std::string& keyword);
  static bool isValidClauseKeyword(const std::string& keyword);
  static bool isValidAssignOperator(const std::string& keyword);

};
