#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "center.hpp"
#include "factory.hpp"
#include "panel.hpp"
#include "pyramid.hpp"
#include "state.hpp"
#include "tile.hpp"
#include "tiles.hpp"
#include "wall.hpp"

namespace py = pybind11;

void
py_bind_state(py::module& root) {
    py::module m = root.def_submodule("state", "Classes defining the state of a Ceramic game");

    py::class_<Tile>(m, "Tile")
        .def(py::init<>())
        .def(py::init<ushort>())
        .def(py::init<Tile&>())

        .def("__eq__", [](Tile self, Tile other) { return self == other; })
        .def("__ne__", [](Tile self, Tile other) { return self != other; })
        .def("__int__", &Tile::operator int)
        .def("__bool__", &Tile::operator bool)

        .def_static("value_to_letter", &Tile::value_to_letter)
        .def_static("letter_to_value", &Tile::letter_to_value)
        .def_static("from_letter", &Tile::from_letter)
        .def("letter", &Tile::letter)

        .def("__str__", &Tile::str)
        .def("__repr__", &Tile::repr)

        .def_property_readonly_static("NONE", [](py::object) { return Tile::NONE; });


    py::class_<Tiles>(m, "Tiles")
        .def(py::init<>())
        .def(py::init<Tile>())
        .def(py::init<const vector<ushort>&>())
        .def(py::init<const Tiles&>())

        .def("__getitem__", [](Tiles self, Tile tile) {
            return self[tile];
        })
        .def("__getitem__", [](Tiles self, int tile) {
            return self[tile];
        })
        .def("__setitem__", [](Tiles& self, Tile tile, ushort value) {
            self[tile] = value;
        })
        .def("__setitem__", [](Tiles& self, int tile, ushort value) {
            self[tile] = value;
        })
        .def("total", &Tiles::total)
        .def("__len__", &Tiles::size)

        .def("__eq__", [](const Tiles& self, const Tiles& other) { return self == other; })
        .def("__ne__", [](const Tiles& self, const Tiles& other) { return self != other; })
        .def("__le__", [](const Tiles& self, const Tiles& other) { return self <= other; })
        .def("__ge__", [](const Tiles& self, const Tiles& other) { return self >= other; })

        .def("__add__", [](const Tiles& self, const Tiles& other) { return self + other; })
        .def("__sub__", [](const Tiles& self, const Tiles& other) { return self - other; })
        .def("__radd__", [](Tiles& self, const Tiles& other) { self += other; })
        .def("__rsub__", [](Tiles& self, const Tiles& other) { self -= other; })

        .def("__add__", [](const Tiles& self, Tile other) { return self + other; })
        .def("__sub__", [](const Tiles& self, Tile other) { return self - other; })
        .def("__radd__", [](Tiles& self, Tile other) { self += other; })
        .def("__rsub__", [](Tiles& self, Tile other) { self -= other; })

        .def_property_readonly("quantities", &Tiles::get_quantities)
        .def_property_readonly_static("ZERO", [](py::object) { return Tiles::ZERO; })

        .def("__str__", &Tiles::str)
        .def("__repr__", &Tiles::repr);


    py::class_<Center, Tiles>(m, "Center")
        .def(py::init<>())
        .def_readonly("first_token", &Center::first_token)
        .def_property_readonly("tiles", [](const Center& center) { return (Tiles)center; })

        .def("__str__", &Center::str)
        .def("__repr__", &Center::repr);


    py::class_<Factory, Tiles>(m, "Factory")
        .def(py::init<ushort>())
        .def_readonly("id", &Factory::id)
        .def_property_readonly("tiles", [](const Factory& factory) { return (Tiles)factory; })

        .def("__str__", &Factory::str)
        .def("__repr__", &Factory::repr);


    py::class_<Panel>(m, "Panel")
        .def(py::init<const std::shared_ptr<Rules>>())
        .def(py::init<const Panel&>())

        .def_property_readonly("score", &Panel::get_score)
        .def_property_readonly("pyramid", &Panel::get_pyramid)
        .def_property_readonly("wall", &Panel::get_wall)
        .def_property_readonly("first_token", &Panel::get_first_token)
        .def_property_readonly("floor", &Panel::get_floor)
        .def_property_readonly("penalty", &Panel::get_penalty)

        .def("legal_line", &Panel::legal_line)

        .def("__str__", &Panel::str)
        .def("__repr__", &Panel::repr);


    py::class_<Pyramid>(m, "Pyramid")
        .def(py::init<ushort>())
        .def(py::init<const std::shared_ptr<Rules>>())
        .def(py::init<const Pyramid&>())

        .def("clear", &Pyramid::clear)
        .def("clear_line", &Pyramid::clear_line)
        .def("set_line", &Pyramid::set_line)

        .def("is_filled", &Pyramid::is_filled)
        .def("is_empty", &Pyramid::is_empty)
        .def("amount", &Pyramid::amount)
        .def("amount_remaining", &Pyramid::amount_remaining)
        .def("color", &Pyramid::color)
        .def("accept_color", &Pyramid::accept_color)
        .def("filled", &Pyramid::filled)

        .def("__str__", &Pyramid::str)
        .def("__repr__", &Pyramid::repr);


    py::class_<State>(m, "State")
        .def(py::init<const std::shared_ptr<Rules>>())
        .def(py::init<const State&>())

        .def_property_readonly("rules", &State::get_rules)
        .def_property_readonly("center", &State::get_center_mut)
        .def_property_readonly("factory", &State::get_factory_mut)
        .def_property_readonly("panel", &State::get_panel_mut)
        .def_property_readonly("bag", &State::get_bag_mut)
        .def_property_readonly("bin", &State::get_bin_mut)

        .def_property("current_player", &State::get_current_player, &State::set_current_player)
        .def("next_player", &State::next_player)

        .def("__str__", &State::str)
        .def("__repr__", &State::repr);


    py::class_<Wall>(m, "Wall")
        .def(py::init<const std::shared_ptr<Rules>>())
        .def(py::init<const Wall&>())

        .def("clear", &Wall::clear)
        .def("get_placed_at", &Wall::get_placed_at)
        .def("get_tile_at", &Wall::get_tile_at)
        .def("get_color_at", &Wall::get_color_at)

        .def("get_placed", &Wall::get_placed)
        .def("get_tiles", &Wall::get_tiles)
        .def("get_placed_array", &Wall::get_placed_array)
        .def("get_tiles_array", &Wall::get_tiles_array)

        .def("line_has_color", &Wall::get_tiles_array)
        .def("line_color_y", &Wall::get_tiles_array)

        .def("score_for_placing", &Wall::score_for_placing)
        .def("place_at", &Wall::place_at)
        .def("set_tile_at", &Wall::set_tile_at)
        .def("place_line_color", &Wall::place_line_color)

        .def("__str__", &Wall::str)
        .def("__repr__", &Wall::repr);
}