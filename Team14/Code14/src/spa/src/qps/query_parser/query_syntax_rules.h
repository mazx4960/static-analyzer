// Copyright 2022 CS3203 Team14. All rights reserved.

#pragma once

#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <utility>

#include "commons/types.h"
#include "commons/lexer/token.h"
enum class SyntaxRuleType {
  kStmtRef,
  kEntRef,
  kStmtEntRef
};
using TokenTypeSet = std::unordered_set<TokenType>;
using SyntaxTokenMap = std::unordered_map<SyntaxRuleType, TokenTypeSet>;
using RelationshipSyntaxRule = std::vector<SyntaxRuleType>;
using RelationshipTypeMap = std::unordered_map<RsType, RsType>;
using SuchThatSyntaxRules = std::unordered_map<RsType, RelationshipSyntaxRule>;

inline static SyntaxTokenMap syntax_token_map = {{SyntaxRuleType::kStmtRef,
                                                  {TokenType::kLiteral, TokenType::kSymbol, TokenType::kWildCard}},
                                                 {SyntaxRuleType::kEntRef,
                                                  {TokenType::kQuote, TokenType::kSymbol, TokenType::kWildCard}},
                                                 {SyntaxRuleType::kStmtEntRef,
                                                  {TokenType::kQuote, TokenType::kLiteral,
                                                   TokenType::kSymbol, TokenType::kWildCard}}};


inline static RelationshipTypeMap allow_star_relationship_map = {{RsType::kFollows, RsType::kFollowsT},
                                                                 {RsType::kParent, RsType::kParentT},
                                                                 {RsType::kCalls, RsType::kCallsT},
                                                                 {RsType::kNext, RsType::kNextT},
                                                                 {RsType::kAffects, RsType::kAffectsT}};

inline static RelationshipSyntaxRule follows_ref_rule = {SyntaxRuleType::kStmtRef, SyntaxRuleType::kStmtRef};
inline static RelationshipSyntaxRule parent_ref_rule = {SyntaxRuleType::kStmtRef, SyntaxRuleType::kStmtRef};
inline static RelationshipSyntaxRule uses_ref_rule = {SyntaxRuleType::kStmtEntRef, SyntaxRuleType::kEntRef};
inline static RelationshipSyntaxRule modifies_ref_rule = {SyntaxRuleType::kStmtEntRef, SyntaxRuleType::kEntRef};
inline static RelationshipSyntaxRule calls_ref_rule = {SyntaxRuleType::kEntRef, SyntaxRuleType::kEntRef};
inline static RelationshipSyntaxRule next_ref_rule = {SyntaxRuleType::kStmtRef, SyntaxRuleType::kStmtRef};
inline static RelationshipSyntaxRule affects_ref_rule = {SyntaxRuleType::kStmtRef, SyntaxRuleType::kStmtRef};


inline static SuchThatSyntaxRules such_that_syntax_rules = {{RsType::kFollows, follows_ref_rule},
                                                            {RsType::kFollowsT, follows_ref_rule},
                                                            {RsType::kParent, parent_ref_rule},
                                                            {RsType::kParentT, parent_ref_rule},
                                                            {RsType::kUses, uses_ref_rule},
                                                            {RsType::kModifies, modifies_ref_rule},
                                                            {RsType::kCalls, calls_ref_rule},
                                                            {RsType::kCallsT, calls_ref_rule},
                                                            {RsType::kNext, next_ref_rule},
                                                            {RsType::kNextT, next_ref_rule},
                                                            {RsType::kAffects, affects_ref_rule},
                                                            {RsType::kAffectsT, affects_ref_rule}};

inline static SyntaxRuleType pattern_syntax_rule = SyntaxRuleType::kEntRef;
