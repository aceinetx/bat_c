#pragma once

// i wouldn't recommend changing those because as stated in matt's spreadsheet: "Registers: 16 1-byte registers, r0-r15. r0 is a zero register"
#define REGISTER_SIG  unsigned
#define REGISTER_TYPE char
#define RAM_SIG       unsigned
#define RAM_TYPE      char

#define ENTRY_POINT "main"


typedef REGISTER_SIG REGISTER_TYPE register_t;
typedef RAM_SIG      RAM_TYPE      ram_t;

#define SHOW_PSEUDOCODE 0