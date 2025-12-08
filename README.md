# structypes

> [!WARNING]
> Use at your own risk! This library is updated often and may not be stable.

This repository contains header files with implementations for various structs
and functions that serve as a standard library for C.

All header files follow the [`stb library style`](https://github.com/nothings/stb/blob/master/docs/stb_howto.txt).

Rules:
- No external dependencies.
- Must compile down to C99.
- Everything must be implemented from scratch.
- Depend as less as possible on C's standard headers (e.g.: string.h).

You can find examples on how to use every function on the `tests` folder.

## Implemented libraries

### Data structures

- [`hashmap.h`](./includes/hashmap.h): a hashmap that uses closed addressing.
- [`node.h`](./includes/node.h): a simple node to make different kinds of trees.
- [`vec.h`](./includes/vec.h): a dynamic array that contains generic pointers.

### Utility functions

- [`str.h`](./includes/str.h): string management using `C's char*`, not a
  string builder implementation.
