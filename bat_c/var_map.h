#pragma once
#include "registers.h"
#include <string>

struct VarMapEntry {
	ram_t offset;
	std::string name;
	std::string scope_name;
};

extern std::vector<VarMapEntry> var_map;
extern int references_count;

void VarMapAdd(std::string name, std::string scope);

ram_t VarMapGet(std::string name, std::string scope);