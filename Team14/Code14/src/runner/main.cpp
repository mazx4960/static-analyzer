#include <cstring>
#include <iostream>

#include "spa.h"

int main() {
  auto *spa = new SPA("C:\\Users\\kelvi\\Desktop\\KelviRepo\\CS3203_repo\\Team14\\Tests14\\system\\stress_1_source.txt" , "C:\\Users\\kelvi\\Desktop\\KelviRepo\\CS3203_repo\\Team14\\Tests14\\system\\stress_1_queries.txt", 1);
  spa->Run();
}
