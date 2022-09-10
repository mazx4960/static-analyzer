#include <gtest/gtest.h>
#include <iostream>

/*
 * A test to verify behaviour of References in C++.
 */

class Obj {
 private:
  int x = 0;

 public:
  Obj() = default;
};

Obj &function_ref(Obj &obj) {
  std::cout << "In ref function : " << &obj << std::endl;
  return obj;
};

Obj function_copy(Obj obj) {
  std::cout << "In copy function: " << &obj << std::endl;
  return obj;
};

TEST(CUSTOM, REFERENCE) {
  Obj obj = Obj();
  std::cout << "Original        : " << &obj << std::endl;

  Obj &obj_ref = function_ref(obj);
  std::cout << "Returned ref    : " << &obj_ref << std::endl;

  Obj obj_copy = function_copy(obj);
  std::cout << "Returned copy   : " << &obj_copy << std::endl;

}