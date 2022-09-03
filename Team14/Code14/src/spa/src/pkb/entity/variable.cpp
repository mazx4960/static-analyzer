// Copyright 2022 CS3203 Team14. All rights reserved.

#include "pkb/entity/variable.h"

#include <list>
#include <map>
#include <string>

#include "pkb/entity/simple_entity.h"

std::map<std::string, std::list<int>>*
Variable::populate(std::string variable, std::list<int> lines) {
	variableTable.insert(std::pair<std::string, std::list<int>>(variable, lines));
	return &variableTable;
}
