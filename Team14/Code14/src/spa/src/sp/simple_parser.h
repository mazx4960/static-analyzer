// Copyright 2022 CS3203 Team14. All rights reserved.

#pragma once

#include <utility>

#include "commons/lexer/token.h"
#include "commons/parser/parser.h"
#include "commons/parser/parser_exceptions.h"
#include "sp/extractor/entity_extractor.h"
#include "sp/simple_definition/simple_grammar.h"

using StringMap = std::unordered_map<std::string, std::vector<std::string>>;
using StringSet = std::unordered_set<std::string>;

class SimpleParser : public Parser {
 private:
  static bool HasDuplicates(std::vector<std::string> *procedures);
  static bool HasInvalidCalls(StringSet *procedures, StringSet *calls);
  static bool IsCyclic(StringMap *call_map);
  static bool IsCyclicUtil(const std::string &proc, StringMap *call_map, StringSet *visited, StringSet *rec_stack);

 public:
  static Node *ParseProgram(const std::vector<Token *> &tokens);
  static void ValidateProgram(Node *node);
};
