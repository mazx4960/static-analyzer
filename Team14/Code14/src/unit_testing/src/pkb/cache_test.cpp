// Copyright 2022 CS3203 Team14. All rights reserved.

#include "pkb/cache.h"

#include <gtest/gtest.h>

#include "commons/entity.h"
#include "pkb/pkb.h"
#include "pkb_test_helper.h"

using EntityRsInv = std::tuple<Entity *, RsType, bool>;

TEST(CacheTest, Integers) {
  auto *cache = new Cache<int, int>(2);
  cache->Add(1, 1);
  cache->Add(2, 2);
  ASSERT_EQ(cache->Get(1).value, 1);
  ASSERT_EQ(cache->Get(2).value, 2);
  cache->Add(1, 3);
  ASSERT_EQ(cache->Get(1).value, 3);
}

TEST(CacheTest, SingleRelationship) {
  auto *cache = new Cache<EntityRsInv, EntityPointerUnorderedSet ,TripletHash>(2);

  auto *v1 = new VariableEntity("v1");
  auto *v2 = new VariableEntity("v2");
  auto *s1 = new StmtEntity(EntityType::kStatement, "1", "");
  auto *s2 = new StmtEntity(EntityType::kStatement, "2", "");
  auto result1 = EntityPointerUnorderedSet({v1, v2});
  auto result2 = EntityPointerUnorderedSet({v1});

  cache->Add({s1, RsType::kUses, true}, result1);
  cache->Add({s2, RsType::kUses, true}, result2);
  ASSERT_TRUE(PKBTestHelper::set_compare(cache->Get({s1, RsType::kUses, true}).value, EntityPointerUnorderedSet({v1, v2})));
  ASSERT_TRUE(PKBTestHelper::set_compare(cache->Get({s2, RsType::kUses, true}).value, EntityPointerUnorderedSet({v1})));
  cache->Add({s1, RsType::kUses, true}, result2);
  ASSERT_TRUE(PKBTestHelper::set_compare(cache->Get({s1, RsType::kUses, true}).value, EntityPointerUnorderedSet({v1})));
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

TEST(CacheTest, MultipleRelationship){
  auto relationship_manager = RelationshipManager();
  auto *a1 = new AssignStmtEntity("1");
  auto *a2 = new AssignStmtEntity("2");
  auto *a3 = new AssignStmtEntity("3");
  auto *a4 = new AssignStmtEntity("4");
  auto *v1 = new VariableEntity("X");
  auto *v2 = new VariableEntity("Y");
  auto *v3 = new VariableEntity("Z");
  auto *v4 = new VariableEntity("A");
  auto *p1 = new ProcedureEntity("main");

  std::vector<Relationship *> relationships = {
      new FollowsRelationship(a1, a2),
      new FollowsRelationship(a2, a3),
      new ModifiesRelationship(a1, v1),
      new ModifiesRelationship(a2, v2),
      new ModifiesRelationship(a3, v3),
      new ModifiesRelationship(a4, v4),
      new UsesRelationship(a2, v1),
      new UsesRelationship(a2, v3),
      new UsesRelationship(a3, v2),
      new UsesRelationship(a4, v3),
      new ParentRelationship(p1, a1),
      new ParentRelationship(p1, a2),
      new ParentRelationship(p1, a3),
      new ParentRelationship(p1, a4),
      new NextRelationship(a1, a2),
      new NextRelationship(a2, a3),
      new NextRelationship(a3, a4),
  };
  relationship_manager.Populate(relationships);

  // No queries made, empty cache
  ASSERT_EQ(relationship_manager.GetCache()->Size(), 0);

  // Simple query made, not caching
  relationship_manager.Get(RsType::kFollows, a1, false);
  ASSERT_EQ(relationship_manager.GetCache()->Size(), 0);

  // Another simple query, not caching
  relationship_manager.Get(RsType::kNext, a1, true);
  ASSERT_EQ(relationship_manager.GetCache()->Size(), 0);

  // Expensive query made, adding to cache
  relationship_manager.Get(RsType::kModifies, a1, false);
  ASSERT_EQ(relationship_manager.GetCache()->Size(), 1);
  ASSERT_TRUE(PKBTestHelper::set_compare(
      relationship_manager.GetCache()->Get({a1, RsType::kModifies, false}).value, {v1}));

  // Another expensive query
  relationship_manager.Get(RsType::kUses, a4, false);
  ASSERT_EQ(relationship_manager.GetCache()->Size(), 2);
  ASSERT_TRUE(PKBTestHelper::set_compare(
      relationship_manager.GetCache()->Get({a4, RsType::kUses, false}).value, {v3}));

  // Expensive query with multiple caches
  relationship_manager.Get(RsType::kAffectsT, a2, false);
  ASSERT_EQ(relationship_manager.GetCache()->Size(), 6);
  ASSERT_TRUE(PKBTestHelper::set_compare(
      relationship_manager.GetCache()->Get({a2, RsType::kAffectsT, false}).value, {a3, a4}));
  ASSERT_TRUE(PKBTestHelper::set_compare(
      relationship_manager.GetCache()->Get({a2, RsType::kAffects, false}).value, {a3}));
  ASSERT_TRUE(PKBTestHelper::set_compare(
      relationship_manager.GetCache()->Get({a3, RsType::kAffects, false}).value, {a4}));

  // Same expensive query, not caching
  relationship_manager.Get(RsType::kAffectsT, a2, false);
  ASSERT_EQ(relationship_manager.GetCache()->Size(), 6);

  // Another expensive query
  relationship_manager.Get(RsType::kAffectsT, a2, true);
  ASSERT_EQ(relationship_manager.GetCache()->Size(), 9);
  ASSERT_TRUE(PKBTestHelper::set_compare(
      relationship_manager.GetCache()->Get({a2, RsType::kAffectsT, true}).value, {a1}));
  ASSERT_TRUE(PKBTestHelper::set_compare(
      relationship_manager.GetCache()->Get({a2, RsType::kAffects, true}).value, {a1}));
  ASSERT_TRUE(PKBTestHelper::set_compare(
      relationship_manager.GetCache()->Get({a1, RsType::kAffects, true}).value, {}));

  // Clear cache
  relationship_manager.ClearCache();
  ASSERT_EQ(relationship_manager.GetCache()->Size(), 0);
}
