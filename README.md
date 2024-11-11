# bfc

A compiler toolchain for Brainfuck, which includes a transpiler to C, an interpreter and a tool for generating Brainfuck code that writes some text.

## How to use

`bfc` is highly configurable. Here's the table of them:

The first argument is _mandatory_. The user must choose from these:

Name|Description
---|---
`compile`|Compiles the provided Brainfuck code into C.
`interpret`|Interprets the provided Brainfuck code.
`write`|Generates Brainfuck code that writes the provided text.

---

These are _optional_. If omitted, its default value is used. <br />
The _shorthand_ syntax can be used in place of the _full_ syntax.

Shorthand|Full|Default|Description
---|---|---|---
`-m`|`--memory-size`|`10000`|Sets the size of the memory the program will use.
`-s`|`--signed`|`false`|Defines whether the memory bytes will be signed or not.
`-u`|`--unsigned`|`true`|Defines whether the memory bytes will be unsigned or not. This option is there for convenience: the memory is _unsigned_ by default.
`-i`|`--input`|`*stdin`|Defines the input stream the program will read from. It has a special value, `*stdin`, which refers to the _standard input_. It can be a file also.
`-o`|`--output`|If mode is `compile`: `out.c`. <br /> If mode is `write`: `out.bf`. <br /> If mode is `interpret`: `*stdout`. |Defines the output stream the program will output to. It has a special value, `*stdout`, which refers to the _standard output_.

## Testing

There are some testing programs inside the `tests/` directory. Feel free to use them while using the program!

### Test Programs' Sources

[github.com/4ffy/brainfuck-programs](https://github.com/4ffy/brainfuck-programs/) <br />
[brainfuck.org](https://www.brainfuck.org/).
