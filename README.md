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
