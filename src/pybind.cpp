#include <pybind11/pybind11.h>
namespace py = pybind11;
#include "global.hpp"

void py_bind_state(py::module& root);
void py_bind_game(py::module& root);
void py_bind_rules(py::module& root);

PYBIND11_MODULE(ceramic, m) {
    m.doc() = "Azul-like Game Environment";

    m.attr("TILE_TYPES") = TILE_TYPES;

    py_bind_rules(m);
    py_bind_state(m);
    py_bind_game(m);
}