# Command Line User Interface Rendering Library

## What cluir-lib is?
  
  + C++ mess for any UNIX Operating System
  + simple tui library
  + educational project
  + portable and not dependable on third party libs
  
## What cluir-lib isnt?

  + a Windows friend (~~you probably *can* install all dependencies in Windows, but most likely its just a big pain~~)
  + low level + well optimized 
  + ncurses successor
  + a well designed solution on C

## Quick start

get the library

```
  $ git clone https://github.com/amuerta/cluirlib
  $ cd cluir_lib && cp cluirlib.* <your-project-directory>

```

use it

```cpp

  #include <cluirlib.cpp>
  // do not use header file

```

## Progress

### Done 
    - basic screen functionality
    - basic input handling
    - basic objects
    - color
### ToDo
    - HighLevel API -> more Blocks
    - many reworks
    - Input Server
    - Render Server
    - Parameter system
    - Glyph(ascii art) font handling
    - file parser

## Preview:
![preview-1](https://github.com/amuerta/cluirlib/imgs/demo1.png)
