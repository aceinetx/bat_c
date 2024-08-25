#include <vector>
#include <string>
#include <assert.h>
#include "registers.h"
#include "configuration.h"

#include "var_map.h"

std::vector<VarMapEntry> var_map;

void VarMapAdd(std::string name, std::string scope) {
	var_map.push_back(VarMapEntry{ (ram_t)var_map.size(), name, scope });
}

ram_t VarMapGet(std::string name, std::string scope) {
	for (int i = 0; i < var_map.size(); i++) {
		VarMapEntry var = var_map.at(i);
		if (var.name == name && var.scope_name == scope || (var.name == name && var.scope_name == "")) return var.offset;
	}
	assert("Failed to locate variable in current scope" == 0);
}