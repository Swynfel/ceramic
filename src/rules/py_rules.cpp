#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "rules.hpp"

#include <memory>

namespace py = pybind11;
using namespace py::literals;


std::shared_ptr<Rules>
py_create_rule(
    ushort player_count,
    ushort tile_count,
    ushort tile_types,
    ushort factory_tiles,
    ushort line_bonus,
    ushort column_bonus,
    ushort type_bonus,
    ushort overflow_count,
    ushort overflow_penalty) {
    return std::make_shared<Rules>(Rules{
        .player_count = player_count,
        .tile_count = tile_count,
        .tile_types = tile_types,
        .factory_tiles = factory_tiles,
        .line_bonus = line_bonus,
        .column_bonus = column_bonus,
        .type_bonus = type_bonus,
        .overflow_count = overflow_count,
        .overflow_penalty = overflow_penalty,
    });
}

void
py_bind_rules(py::module& root) {
    py::module m = root.def_submodule("rules", "Rules for Ceramic games");

    py::class_<Rules, std::shared_ptr<Rules>>(m, "Rules")
        .def(py::init(&py_create_rule),
            "player_count"_a = 4,
            "tile_count"_a = 20,
            "tile_types"_a = 5,
            "factory_tiles"_a = 4,
            "line_bonus"_a = 7,
            "column_bonus"_a = 3,
            "type_bonus"_a = 15,
            "overflow_count"_a = 7,
            "overflow_penalty"_a = 3)
        .def(py::init<const Rules&>())

        .def_readwrite("player_count", &Rules::player_count)
        .def_readwrite("tile_count", &Rules::tile_count)
        .def_readwrite("tile_types", &Rules::tile_types)
        .def_readwrite("factory_tiles", &Rules::factory_tiles)
        .def_readwrite("line_bonus", &Rules::line_bonus)
        .def_readwrite("column_bonus", &Rules::column_bonus)
        .def_readwrite("type_bonus", &Rules::type_bonus)
        .def_readwrite("overflow_count", &Rules::overflow_count)
        .def_readwrite("overflow_penalty", &Rules::overflow_penalty)

        .def("__eq__", [](const Rules self, const Rules other) { return self == other; })
        .def("__ne__", [](const Rules self, const Rules other) { return self != other; })

        .def("penalty_at",
            &Rules::penalty_at,
            "The penalty at a given position on the floor"
            "position"_a)
        .def("penalty_for_floor",
            &Rules::penalty_for_floor,
            "The penalty at for a given tiles on floor"
            "floor"_a)
        .def_property_readonly("factory_count", &Rules::factory_count)
        .def_property_readonly("tile_types_2", &Rules::tile_types_2)

        .def_property_readonly_static("BASE", [](py::object) { return Rules::BASE; })
        .def_property_readonly_static("MINI", [](py::object) { return Rules::MINI; })

        .def("__str__", &Rules::str)
        .def("__repr__", &Rules::repr);
}