#include <gtest/gtest.h>
#include <iostream>

/*
 * A test to verify behaviour of References in C++.
 * For debugging purposes.
 */

class Obj {
 public:
  int x_ = 0; // some random variable
  Obj() = default;
};

Obj &function_ref(Obj &obj) {
  return obj;
};

Obj function_copy(Obj obj) {
  return obj;
};

TEST(CUSTOM, REFERENCE) {
  Obj obj = Obj();
  Obj &obj_ref = function_ref(obj);         // return type and declaration are both references
  Obj invalid_obj_ref = function_ref(obj);  // return type is reference, but declaration is not reference
  Obj obj_copy = function_copy(obj);           // return type and declaration are both copies

  ASSERT_EQ(&obj, &obj_ref);            // both references the same memory space
  ASSERT_NE(&obj, &invalid_obj_ref);    // invalid_obj_ref points to a different memory space
  ASSERT_NE(&obj, &obj_copy);           // obj_copy points to a different memory space
}