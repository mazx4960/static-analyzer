// Copyright 2022 CS3203 Team14. All rights reserved.

#include "entity_table.h"

void EntityTable::populate(Entity &entity) { this->table_.insert(&entity); }

std::unordered_set<Entity *, EntityHashFunction, EntityPointerEquality> EntityTable::get() { return this->table_; }

int EntityTable::getCount() { return this->table_.size(); }