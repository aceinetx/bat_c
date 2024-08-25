#pragma once
#include "configuration.h"

enum WorkingInstruction {
	WI_NOTHING,
	WI_DEFINE,
	WI_IDENT_UNKNOWN,
	WI_INCREMENT_DECREMENT,
	WI_COMPARE_EXPR,
	WI_PORT
};

struct INSTDefine {
	char* name = (char*)-1;
	register_t value;
};