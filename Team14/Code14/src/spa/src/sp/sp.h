#pragma once

#include <fstream>

#include "pkb/pkb.h"

class SP {
 private:
  IPKBSaver *pkb_;

 public:
  SP() = default;
  void SetPKB(IPKBSaver *pkb);
  void LoadSource(std::ifstream &source_stream);
};
