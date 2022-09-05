#include <iostream>

#include "spa.h"

int main(int argc, char *argv[]) {
  if (argc != 3) {
    std::cout << "Usage: " << argv[0] << " <source file> <query file> ";
    return 1;
  }

  // Initialize and run SPA
  auto *spa = new SPA(argv[1], argv[2]);
  spa->Run();
}
