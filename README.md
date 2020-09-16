# Ceramic
A python module for playing variations of the board game Azul, implemented in C++.


## Build for Python

### Dependencies

Before building this repository make sure to install the following dependencies:
- C++14 compiler
- [Pybind11](https://github.com/pybind/pybind11)

#### Pybind11

[Pybind11](https://github.com/pybind/pybind11) is a library to binds the C++ code to python.

It can be installed with `pip` by executing the following command
```
python -m pip install pybind11
```

### Build

Execute the following command, at the root of the project
```
python setup.py build_ext --inplace
```
to generate the module.
For example, on linux, and using python3.7, it will be called `ceramic.cpython-37m-x86_64-linux-gnu.so` file.
It can safely be renamed `ceramic.so`.


## Build for C++ only

This part is to build c++ only (no python) libraries and executables.

### Dependencies

Make sure you have the following tools:
- CMake
- Make
- C++14 compiler

### Build

Execute the following
```
mkdir build
cmake ..
make
```

Executable and libraries will be placed in the `build` directory.

#### ceramic-test
Runs a game with 4 random players.

#### ceramic-terminal-player
Play a game against 3 random players using the terminal.