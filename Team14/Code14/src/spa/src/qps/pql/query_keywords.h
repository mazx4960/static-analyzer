// Copyright 2022 CS3203 Team14. All rights reserved.

#pragma once

#include <utility>
#include <vector>
#include <map>

#include "query_call.h"
#include "query_declaration.h"

class QueryKeywords {
 private:
  inline static const std::unordered_set<std::string> kCallKeywords{"Select"};
  inline static const std::unordered_set<std::string> kAssignOperators = {
      "+", "-", "*", "/", "%", "<", ">"};
  inline static const std::unordered_set<std::string> kSuchThatKeywords{
      "such", "that"};

  inline static const std::unordered_map<std::string, EntityType> kDeclarationKeywordsMap{
    {"stmt", EntityType::kStatement},
    {"read", EntityType::kReadStmt},
    {"print", EntityType::kPrintStmt},
    {"call", EntityType::kCallStmt},
    {"while", EntityType::kWhileStmt},
    {"if", EntityType::kIfStmt},
    {"assign", EntityType::kAssignStmt},
    {"variable", EntityType::kVariable},
    {"constant", EntityType::kConstant},
    {"procedure", EntityType::kProcedure}};

  inline static const std::unordered_map<std::string, RsType> kRelationshipKeywordsMap{
    {"Follows", RsType::kFollows},
    {"Parent", RsType::kParent},
    {"Uses", RsType::kUses},
    {"Modifies", RsType::kModifies},
    {"pattern", RsType::kAssignPattern}};

  inline static const std::unordered_map<std::string, RsType> kPatternKeywordsMap{
    {"pattern", RsType::kAssignPattern}};

 public:
  static bool isValidKeyword(const std::string &keyword);
  static bool isValidSuchThatKeyword(const std::string &keyword);
  static bool isValidDeclarationKeyword(const std::string &keyword);
  static bool isValidCallKeyword(const std::string &keyword);
  static bool isValidRelationshipKeyword(const std::string &keyword);
  static bool isValidPatternKeyword(const std::string &keyword);
  static bool isValidAssignOperator(const std::string &keyword);
  static EntityType declarationKeywordToType(const std::string &keyword);
  static RsType relationshipKeywordToType(const std::string &keyword);
  static RsType patternKeywordToType(const std::string &keyword);
};
