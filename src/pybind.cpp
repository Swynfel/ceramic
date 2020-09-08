#include <pybind11/pybind11.h>
namespace py = pybind11;
#include "global.hpp"

void state_pybind(py::module& m);
void action_pybind(py::module& m);

PYBIND11_MODULE(ceramic, m) {
    m.doc() = "Azul-like Game Environment";

    m.attr("TILE_TYPES") = TILE_TYPES;

    state_pybind(m);
    action_pybind(m);
}