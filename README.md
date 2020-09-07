# Ceramic
A python module for playing variations of the board game Azul, implemented in C++.

## Pybind11

Before building this repository make sure to install [pybind11](https://github.com/pybind/pybind11), to binds the C++ code to python.

It can be installed with `pip` by executing the following command
```
python -m pip install pybind11
```

## Build

Execute the following command, at the root of the project
```
python setup.py build_ext --inplace
```
to generate the module.
For example, on linux, and using python3.7, it will be called `ceramic.cpython-37m-x86_64-linux-gnu.so` file.
It can safely be renamed `ceramic.so`.