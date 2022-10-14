// Copyright 2022 CS3203 Team14. All rights reserved.

#include "pkb/cache.h"

#include <gtest/gtest.h>

#include "commons/entity.h"
#include "pkb_test_helper.h"

TEST(CacheTest, Integers) {
  auto *cache = new Cache<int, int>(2);
  cache->Add(1, 1);
  cache->Add(2, 2);
  ASSERT_EQ(cache->Get(1).value, 1);
  ASSERT_EQ(cache->Get(2).value, 2);
  cache->Add(1, 3);
  ASSERT_EQ(cache->Get(1).value, 3);
}

TEST(CacheTest, Relationships) {
  using entity_rs_inv = std::tuple<Entity *, RsType, bool>;
  using EntitySet = std::unordered_set<Entity *, EntityHashFunction, EntityPointerEquality>;
  auto *cache = new Cache<entity_rs_inv, EntitySet, TripletHash>(2);

  auto *v1 = new VariableEntity("v1");
  auto *v2 = new VariableEntity("v2");
  auto *s1 = new StmtEntity(EntityType::kStatement, "1", "");
  auto *s2 = new StmtEntity(EntityType::kStatement, "2", "");
  auto result1 = EntitySet({v1, v2});
  auto result2 = EntitySet({v1});

  cache->Add({s1, RsType::kUses, true}, result1);
  cache->Add({s2, RsType::kUses, true}, result2);
  ASSERT_TRUE(PKBTestHelper::set_compare(cache->Get({s1, RsType::kUses, true}).value, EntitySet({v1, v2})));
  ASSERT_TRUE(PKBTestHelper::set_compare(cache->Get({s2, RsType::kUses, true}).value, EntitySet({v1})));
  cache->Add({s1, RsType::kUses, true}, result2);
  ASSERT_TRUE(PKBTestHelper::set_compare(cache->Get({s1, RsType::kUses, true}).value, EntitySet({v1})));
  ASSERT_FALSE(cache->Get({s1, RsType::kUses, false}).found);
}

TEST(CacheTest, Eviction) {
  auto *cache = new Cache<int, int>(2);
  cache->Add(1, 1);
  cache->Add(2, 2);
  cache->Add(3, 3);
  // 1 should be evicted
  ASSERT_FALSE(cache->Get(1).found);
  ASSERT_TRUE(cache->Get(2).found);
  cache->Add(1, 3);
  // 3 should be evicted since 2 was recently accessed
  ASSERT_FALSE(cache->Get(3).found);
  ASSERT_TRUE(cache->Get(1).found);
  ASSERT_TRUE(cache->Get(2).found);
}