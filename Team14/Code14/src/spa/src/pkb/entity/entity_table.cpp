// Copyright 2022 CS3203 Team14. All rights reserved.

#include "entity_table.h"

void EntityTable::populate(Entity &entity) {
  this->table_.insert(&entity);
}

EntityPointerUnorderedSet EntityTable::get() {
  return this->table_;
}
