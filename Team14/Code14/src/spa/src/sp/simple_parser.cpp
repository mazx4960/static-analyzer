// Copyright 2022 CS3203 Team14. All rights reserved.

#include "simple_parser.h"

Node *SimpleParser::ParseProgram(const std::vector<Token *> &tokens) {
  auto *ast = Parser::Parse(new ProgramGrammarRule(), std::move(tokens));
  if (ast == nullptr) { throw ParseSyntaxError("Failed to parse program"); }
  bool ok = ValidateProgram(ast);
  if (!ok) { throw ParseSemanticError("Cyclic call detected"); }
  return ast;
}
bool SimpleParser::ValidateProgram(Node *node) {
  StringMap call_map;
  auto const op = [&call_map](Node *node) {
    if (node->GetNodeType() != NodeType::kProcedure) { return; }
    auto *procedure = static_cast<ProcedureNode *>(node);
    auto proc_name = procedure->GetProcName();
    auto calls = EntityExtractor::ExtractProcedureCalls(procedure);
    for (auto *call : calls) { call_map[proc_name].push_back(call->GetValue()); }
  };
  node->VisitAll(op);
  return !IsCyclic(call_map);
}
bool SimpleParser::IsCyclic(const StringMap &call_map) {
  StringSet visited;
  StringSet rec_stack;
  for (auto const &[proc, calls] : call_map) {
    if (visited.find(proc) != visited.end()) { continue; }
    if (IsCyclicUtil(proc, call_map, visited, rec_stack)) { return true; }
  }
  return false;
}
bool SimpleParser::IsCyclicUtil(const std::string &proc, StringMap call_map, StringSet &visited, StringSet &rec_stack) {
  visited.insert(proc);
  rec_stack.insert(proc);
  for (auto const &call : call_map[proc]) {
    if (visited.find(call) == visited.end() && IsCyclicUtil(call, call_map, visited, rec_stack)) { return true; }
    if (rec_stack.find(call) != rec_stack.end()) { return true; }
  }
  rec_stack.erase(proc);
  return false;
}
