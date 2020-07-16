# azul
A python module for playing the board game Azul implemented in C++.

It uses [SWIG-4.0](http://www.swig.org) to bind the C++ code to python, so make sure to set it up before. You should have access to the `swig` command.

## Build

Execute the script `build.sh`
```
source build.sh
```

The python module (e.g. `azul.so` for linux) is created in the directory `target`.
