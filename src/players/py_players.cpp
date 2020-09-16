#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "first_legal_player.hpp"
#include "random_player.hpp"
#include "terminal_player.hpp"

namespace py = pybind11;

void
py_bind_players(py::module& root) {
    py::module m = root.def_submodule("players");

    py::class_<FirstLegalPlayer, std::shared_ptr<FirstLegalPlayer>, Player>(m, "FirstLegalPlayer")
        .def(py::init<>());

    py::class_<RandomPlayer, std::shared_ptr<RandomPlayer>, Player>(m, "RandomPlayer")
        .def(py::init<>())
        .def(py::init<int>());

    py::class_<TerminalPlayer, std::shared_ptr<TerminalPlayer>, Player>(m, "TerminalPlayer")
        .def(py::init<>());
}