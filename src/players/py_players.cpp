#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "game/player.hpp"
#include "players/first_legal_player.hpp"

namespace py = pybind11;

void
py_bind_players(py::module& root) {
    py::module m = root.def_submodule("players");

    py::class_<FirstLegalPlayer, Player>(m, "FirstLegalPlayer")
        .def(py::init<>());
}