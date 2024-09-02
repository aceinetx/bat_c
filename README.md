# bat_c
A High-level language for [matt's redstone computer](https://github.com/mattbatwings/BatPU-2) with C-Like syntax
## Features implemented
- Variables
- Define
- Functions
- Compare expressions
- If statements
- Scopes
## WIP Features
- While loops
- Else statements
## Get started
Your first option is to download the .exe from latest release and run it (under linux use wine), second option is to use bat c [online](https://aceinetserv.000webhostapp.com/batc/)
## Known issues
- You can call functions like this: ```func(``` and not like this: ```func();```
- If you try to compile this under a linux machine, when ran you'll get segmentation fault (but I think you can run it under wine)
- Expressions can only be 3-4 operators long (i'm lazy to implement more)
## Example programs
Math operations (Division is not currently supported)
```
main {
  x; y;

  x = 20;
  y = 5 * x;
}
```
While loop
```
x;
loop {
  increment x;
  loop();
}
main {
  loop();
}
```
If statements
```
main {
  x;
  increment x;

  cmpexpr x == 1;
  if;
    x = 69;
  endif;
}
```
References<br>
references are basically variables with the same variable mapping
```
a; b;

main {
	b = ref a;
	increment a;
	increment b;
}
```
Fill screen
```
x; y;

x_loop {
	increment x;
	cmpexpr x < 32;
	if;
		port pixel_x x;
		port pixel_y y;

		port draw_pixel;

		port buffer_screen;

		x_loop();
	endif;
}

loop {
	x = 255;
	increment y;
	cmpexpr y < 32;
	if;
		x_loop();
		loop();
	endif;
}

main {
	port clear_screen_buffer;

	decrement x;
	decrement y;

	loop();
}
```
Inline assembly
```
main {
	asm
ldi r5 42
ldi r6 69;
}
```
