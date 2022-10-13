#include "query_keywords.h"

bool QueryKeywords::isValidKeyword(const std::string &keyword) {
  return isValidCallKeyword(keyword) ||
  isValidDeclarationKeyword(keyword) ||
  isValidRelationshipKeyword(keyword) ||
  isValidSuchThatKeyword(keyword) ||
  isValidPatternKeyword(keyword) ||
  isValidAndKeyword(keyword);
}
bool QueryKeywords::isValidDeclarationKeyword(const std::string &keyword) {
  return kDeclarationKeywordsMap.find(keyword) != kDeclarationKeywordsMap.end();
}
bool QueryKeywords::isValidCallKeyword(const std::string &keyword) {
  return kCallKeywords.find(keyword) != kCallKeywords.end();
}
bool QueryKeywords::isValidRelationshipKeyword(const std::string &keyword) {
  return kRelationshipKeywordsMap.find(keyword) != kRelationshipKeywordsMap.end();
}
bool QueryKeywords::isValidPatternKeyword(const std::string &keyword) {
  return kPatternKeywordsMap.find(keyword) != kPatternKeywordsMap.end();
}
bool QueryKeywords::isValidSuchThatKeyword(const std::string &keyword) {
  return kSuchThatKeywords.find(keyword) != kSuchThatKeywords.end();
}
bool QueryKeywords::isValidAssignOperator(const std::string &keyword) {
  return kAssignOperators.find(keyword) != kAssignOperators.end();
}
bool QueryKeywords::isValidAndKeyword(const std::string &keyword) {
  return kAndKeywords.find(keyword) != kAndKeywords.end();
}
EntityType QueryKeywords::declarationKeywordToType(const std::string &keyword) {
  return kDeclarationKeywordsMap.at(keyword);
}
RsType QueryKeywords::relationshipKeywordToType(const std::string &keyword) {
  return kRelationshipKeywordsMap.at(keyword);
}
RsType QueryKeywords::patternKeywordToType(const std::string &keyword) {
  return kPatternKeywordsMap.at(keyword);
}
