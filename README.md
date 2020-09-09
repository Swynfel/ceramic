# Ceramic
A python module for playing variations of the board game Azul, implemented in C++.

## Dependencies

Before building this repository make sure to install the following dependencies.

### Pybind11

[Pybind11](https://github.com/pybind/pybind11) is a library to binds the C++ code to python.

It can be installed with `pip` by executing the following command
```
python -m pip install pybind11
```

### Boost

[Boost](https://www.boost.org/) is a collection of C++ libraries. We need it for quick random number generation.

On Ubuntu, it can be installed with
```
sudo apt-get install libboost-all-dev
```


## Build

Execute the following command, at the root of the project
```
python setup.py build_ext --inplace
```
to generate the module.
For example, on linux, and using python3.7, it will be called `ceramic.cpython-37m-x86_64-linux-gnu.so` file.
It can safely be renamed `ceramic.so`.