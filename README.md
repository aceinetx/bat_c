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
- Port mappings interactions
- While loops
- Else statements
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
References
references are basically variables with the same variable mapping
```
a; b;

main {
	b = ref a;
	increment a;
	increment b;
}
```
