#pragma once

#include <istream>

#include "pkb/pkb.h"

class SP {
 private:
  IPKBPopulator *pkb_;

 public:
  SP() = default;
  void SetPKB(IPKBPopulator *pkb);
  void LoadSource(std::istream *source_stream);
};
