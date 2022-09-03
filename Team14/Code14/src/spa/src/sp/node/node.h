

// Copyright 2022 CS3203 Team14. All rights reserved.

#ifndef SPA_TEAM14_CODE14_SRC_SPA_SRC_SP_NODE_NODE_H_
#define SPA_TEAM14_CODE14_SRC_SPA_SRC_SP_NODE_NODE_H_

#endif//SPA_TEAM14_CODE14_SRC_SPA_SRC_SP_NODE_NODE_H_

enum class NodeType {
  kProgram,
  kProcedure,
  kStmt,
  kAssign,
  kCall,
  kIf,
  kWhile,
  kPrint,
  kRead,
};

class Node {
 private:
  NodeType type_;

 public:
  Node();
  ~Node();
  NodeType GetType();
};