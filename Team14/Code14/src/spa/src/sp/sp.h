#pragma once

#include <fstream>

#include "pkb/pkb.h"

class SP {
 private:
  PKB *pkb_;

 public:
  SP() = default;
  void SetPKB(PKB *pkb);
  void LoadSource(std::ifstream &source_stream);
};
