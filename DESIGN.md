# Soya Design Document

## Design principles

- only one way to do things
- each header must be modular, i.e. work without soyalib.h or dependence on other files

## Conventions

| Description                                                   | Example                                         |
|---------------------------------------------------------------|-------------------------------------------------|
| Object-like macros should be capitalized                      | `#define SY_BLACK (syRgba){0., 0., 0., 1.}`     |
| Function-like macros should be camelCase                      | `#define syColorOpaque(c) syWithAlpha((c), 1.)` |
| Functions should be camelCase                                 | `void syClear()`                                |
| Functions, Macros and Structs should have the `sy` prefix[^1] |                                                 |
| Type functions | syArray syArrayPush |


[^1]: Except for the user defined functions `configure`, `setup` and `loop`



