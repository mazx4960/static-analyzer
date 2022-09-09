#pragma once

#include <fstream>

#include "pkb/pkb.h"

class SP {
 private:
  IPKBPopulator *pkb_;

 public:
  SP() = default;
  void SetPKB(IPKBPopulator *pkb);
  void LoadSource(std::ifstream &source_stream);
};
