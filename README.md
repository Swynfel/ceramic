# Ceramic
A python module for playing variations of the board game Azul, implemented in C++.

## Dependencies

Before building this repository, make sure to set up the following:

- SWIG-4.0
- Boost

[SWIG-4.0](http://www.swig.org) binds the C++ code to python. You should have access to the `swig` command.

[Boost](https://www.boost.org) is a collection of C++ libraries. We need it for the random number generation. On Ubuntu, it can be installed with `sudo apt-get install libboost-all-dev`.

## Build (linux)

Execute the script `build.sh`
```
source build.sh
```

The python module `ceramic.so` is created in the directory `target`.
