// Copyright 2022 CS3203 Team14. All rights reserved.

#pragma once

#include <unordered_set>
#include "commons/entity.h"

using EntitySet = std::unordered_set<Entity *, EntityHashFunction, EntityPointerEquality>;
