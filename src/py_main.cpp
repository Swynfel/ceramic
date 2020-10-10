#include <pybind11/pybind11.h>
namespace py = pybind11;
#include "global.hpp"
#include "utils/random.hpp"

void py_bind_arena(py::module& root);
void py_bind_rules(py::module& root);
void py_bind_state(py::module& root);
void py_bind_game(py::module& root);
void py_bind_players(py::module& root);

PYBIND11_MODULE(ceramic, m) {
    m.doc() = "Azul-like Game Environment";

    m.attr("TILE_TYPES") = TILE_TYPES;

    m.def("random_seed", &random_seed);

    py_bind_rules(m);
    py_bind_state(m);
    py_bind_game(m);
    py_bind_players(m);
    py_bind_arena(m);
}