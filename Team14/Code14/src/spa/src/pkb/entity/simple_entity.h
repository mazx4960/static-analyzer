// Copyright 2022 CS3203 Team14. All rights reserved.

#pragma once

#include <list>
#include <map>
#include <string>

class SIMPLEEntity {
public:
	virtual std::map<std::string, std::list<int>>*
		populate(std::string entity, std::list<int> lines) = 0;
}
