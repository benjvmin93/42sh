# 42sh

### Presentation

42sh is probably the most famous project proposed by EPITA school. The goal was to implement a POSIX compliant shell in C that will implement every features of a classical shell. The purpose was mostly to deeply learn C language in all its facets.

### Build

`meson setup {dirName}`

`ninja -C {dirName}`

### Tests

After building the project, go inside the {dirName} directory and type:

`ninja check`

This will launch the testsuite.

### Issues ?

If you have this error when you are trying to build the project :

`meson.build:42:0: ERROR: C library 'readline' not found`

If you have Ubuntu, write down on your terminal:

`sudo apt install libreadline-dev` and try again.
