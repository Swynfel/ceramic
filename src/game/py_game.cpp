#include <pybind11/pybind11.h>

#include "action.hpp"
#include "game.hpp"

namespace py = pybind11;

void
py_bind_game(py::module& root) {
    py::module m = root.def_submodule("game");
}