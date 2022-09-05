#include <iostream>

#include "spa.h"

int main(int argc, char *argv[]) {
  if (argc != 2) {
    std::cout << "Usage: {} <source file> <query file>" << argv[0];
    return 1;
  }

  // Initialize and run SPA
  auto *spa = new SPA(argv[1], argv[2]);
  spa->Run();
}
