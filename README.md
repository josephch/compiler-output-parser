# Compiler Output Parser

Parses compiler output and populates error, warning and messages in a structured manner.

## Description

Compiler Output Parser is a header only library, which provides an API to parse compiler output a single line at a time. 

## Getting Started

### Dependencies

* Compiler Output Parser uses [compile-time-regular-expressions library](https://github.com/hanickadot/compile-time-regular-expressions)

### Getting the source
```
git clone https://github.com/josephch/compiler-output-parser.git
```
Compiler Output Parser depends on compile-time-regular-expressions libraries. Please make sure that it is installed. It is  present also as a submodule, can be fetched via
```
git submodule init
git submodule update
```

### Using the library

* Sample code
```
#include "compiler_output_parser.hpp"
void fn()
{
   std::string testLine = "/home/test/file/path/test.cpp:3:10: fatal error: test.h: No such file or directory";
   CompilerOutputLineInfo info = GetCompilerOutputLineInfo(testLine);
}
```

## Authors

Contributors names and contact info

Christo Joseph
[twitter](https://x.com/christojoseph)

## License

This project is licensed under the GPL V3 License - see the LICENSE.md file for details

## Acknowledgments

Inspiration, code snippets, etc.
* [Code::Blocks](https://www.codeblocks.org/)
* [compile-time-regular-expressions](https://github.com/hanickadot/compile-time-regular-expressions)
