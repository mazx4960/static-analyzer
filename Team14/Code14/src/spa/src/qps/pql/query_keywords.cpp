#include "query_keywords.h"

bool QueryKeywords::isValidKeyword(const std::string &keyword) {
  return isValidCallKeyword(keyword) || isValidDeclarationKeyword(keyword) ||
      isValidClauseKeyword(keyword) || isValidSuchThatKeyword(keyword);
}
bool QueryKeywords::isValidDeclarationKeyword(const std::string &keyword) {
  return kDeclarationKeywords.find(keyword) != kDeclarationKeywords.end();
}
bool QueryKeywords::isValidCallKeyword(const std::string &keyword) {
  return kCallKeywords.find(keyword) != kCallKeywords.end();
}
bool QueryKeywords::isValidClauseKeyword(const std::string &keyword) {
  return kClauseKeywords.find(keyword) != kClauseKeywords.end();
}
bool QueryKeywords::isValidSuchThatKeyword(const std::string &keyword) {
  return kSuchThatKeywords.find(keyword) != kSuchThatKeywords.end();
}
bool QueryKeywords::isValidAssignOperator(const std::string &keyword) {
  return kAssignOperators.find(keyword) != kAssignOperators.end();
}
