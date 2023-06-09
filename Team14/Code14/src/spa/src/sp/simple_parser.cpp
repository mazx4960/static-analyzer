// Copyright 2022 CS3203 Team14. All rights reserved.

#include "simple_parser.h"
#include <spdlog/spdlog.h>

Node *SimpleParser::ParseProgram(std::vector<Token *> &tokens) {
  auto *ast = Parser::Parse(new ProgramGrammarRule(), tokens);
  if (ast == nullptr) {
    throw ParseSyntaxError("Failed to Parse program");
  }
  ValidateProgram(ast);
  return ast;
}
void SimpleParser::ValidateProgram(Node *node) {
  if (node->GetNodeType() != NodeType::kProgram) {
    spdlog::error("ValidateProgram: Invalid program node");
    return;
  }
  auto *program = dynamic_cast<ProgramNode *>(node);
  StringSet procedures;
  StringSet calls;
  StringMap call_map;
  auto const op = [&procedures, &calls, &call_map](Node *node) {
    if (node->GetNodeType() != NodeType::kProcedure) {
      spdlog::error("ValidateProgram: Invalid procedure node");
      return;
    }
    auto *procedure = static_cast<ProcedureNode *>(node);
    auto proc_name = procedure->GetProcName();
    auto procedure_calls = EntityExtractor::ExtractProcedureCalls(procedure);

    if (procedures.find(proc_name) != procedures.end()) {
      throw ParseSemanticError("Program contains procedures with the same name");
    }
    procedures.insert(proc_name);
    for (auto *call : procedure_calls) {
      calls.insert(call->GetValue());
      call_map[proc_name].push_back(call->GetValue());
    }
  };
  program->VisitChildren(op);

  if (HasInvalidCalls(procedures, calls)) {
    throw ParseSemanticError("Program contains calls to non-existent procedures");
  }
  if (IsCyclic(call_map)) {
    throw ParseSemanticError("Cyclic call detected");
  }
}
bool SimpleParser::HasInvalidCalls(StringSet &procedures, StringSet &calls) {
  for (const auto &call : calls) {
    if (procedures.find(call) == procedures.end()) {
      return true;
    }
  }
  return false;
}
bool SimpleParser::IsCyclic(StringMap &call_map) {
  StringSet visited;
  StringSet rec_stack;
  for (auto const &[proc, calls] : call_map) {
    if (visited.find(proc) != visited.end()) {
      continue;
    }
    if (IsCyclicUtil(proc, call_map, visited, rec_stack)) {
      return true;
    }
  }
  return false;
}
bool SimpleParser::IsCyclicUtil(const std::string &proc,
                                StringMap &call_map,
                                StringSet &visited,
                                StringSet &rec_stack) {
  visited.insert(proc);
  rec_stack.insert(proc);
  for (auto const &call : call_map[proc]) {
    if (visited.find(call) == visited.end() && IsCyclicUtil(call, call_map, visited, rec_stack)) {
      return true;
    }
    if (rec_stack.find(call) != rec_stack.end()) {
      return true;
    }
  }
  rec_stack.erase(proc);
  return false;
}
