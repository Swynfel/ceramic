#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "center.hpp"
#include "factory.hpp"
#include "panel.hpp"
#include "py_utils.hpp"
#include "pyramid.hpp"
#include "state.hpp"
#include "tile.hpp"
#include "tiles.hpp"
#include "wall.hpp"

namespace py = pybind11;
using namespace py::literals;

void
py_bind_state(py::module& root) {
    py::module m = root.def_submodule("state", "Classes defining the state of a Ceramic game");

    py::class_<Tile>(m, "Tile")
        .def(py::init<ushort>(),
            "value"_a)
        .def(py::init<Tile&>())

        .def("__eq__", &py_eq<Tile>)
        .def("__ne__", &py_ne<Tile>)
        .def("__int__", &Tile::operator int)
        .def("__bool__", &Tile::operator bool)

        .def_static("value_to_letter",
            &Tile::value_to_letter,
            "value"_a)
        .def_static("value_to_lc_letter",
            &Tile::value_to_lc_letter,
            "value"_a)
        .def_static("letter_to_value",
            &Tile::letter_to_value,
            "letter"_a)
        .def_static("from_letter",
            &Tile::from_letter,
            "letter"_a)
        .def("letter", &Tile::letter)
        .def("lc_letter", &Tile::lc_letter)

        .def("__str__", &Tile::str)
        .def("__repr__", &Tile::repr)

        .def_property_readonly_static("NONE", [](py::object) { return Tile::NONE; });


    py::class_<Tiles>(m, "Tiles")
        .def(py::init<>())
        .def(py::init<Tile>(),
            "tile"_a)
        .def(py::init<Tile, int>(),
            "tile"_a,
            "count"_a)
        .def(py::init<const std::vector<ushort>&>(),
            "list"_a)
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
        .def("is_empty", &Tiles::is_empty)
        .def("has_color", &Tiles::has_color)

        .def("__eq__", &py_eq<Tiles>)
        .def("__ne__", &py_ne<Tiles>)
        .def("__le__", &py_le<Tiles>)
        .def("__ge__", &py_ge<Tiles>)

        .def("__add__", &py_add<Tiles>)
        .def("__sub__", &py_sub<Tiles>)
        .def("__radd__", &py_radd<Tiles>)
        .def("__rsub__", &py_rsub<Tiles>)

        .def("__add__", &py_add<Tiles, Tile>)
        .def("__sub__", &py_sub<Tiles, Tile>)
        .def("__radd__", &py_radd<Tiles, Tile>)
        .def("__rsub__", &py_rsub<Tiles, Tile>)

        .def_property_readonly("quantities", &Tiles::get_quantities)
        .def_property_readonly_static("ZERO", [](py::object) { return Tiles::ZERO; })

        .def("__str__", &Tiles::str)
        .def("letter_str", &Tiles::letter_str)
        .def("__repr__", &Tiles::repr);


    py::class_<Center, Tiles>(m, "Center")
        .def(py::init<>())
        .def_readonly("first_token", &Center::first_token)
        .def_property(
            "tiles",
            [](const Center& center) { return (Tiles)center; },
            [](Center& center, Tiles value) { center.set_tiles(value); })

        .def("__str__", &Center::str)
        .def("__repr__", &Center::repr);


    py::class_<Factory, Tiles>(m, "Factory")
        .def(py::init<ushort>(),
            "id"_a)
        .def_readonly("id", &Factory::id)
        .def_property(
            "tiles",
            [](const Factory& factory) { return (Tiles)factory; },
            [](Factory& factory, Tiles value) { factory.set_tiles(value); })

        .def("__str__", &Factory::str)
        .def("__repr__", &Factory::repr);


    py::class_<Panel>(m, "Panel")
        .def(py::init<const std::shared_ptr<const Rules>>(),
            "rules"_a)
        .def(py::init<const Panel&>())

        .def_property_readonly("score", &Panel::get_score)
        .def_property_readonly("pyramid", &Panel::get_pyramid_mut)
        .def_property_readonly("wall", &Panel::get_wall_mut)
        .def_property("first_token", &Panel::get_first_token, &Panel::set_first_token)
        .def_property_readonly("floor", &Panel::get_floor)
        .def_property_readonly("penalty", &Panel::get_penalty)

        .def("add_score",
            &Panel::add_score,
            "value"_a)
        .def("add_floor",
            &Panel::add_floor,
            "value"_a)

        .def("legal_line",
            &Panel::legal_line,
            "line"_a,
            "tile"_a)

        .def("__str__", &Panel::str)
        .def("__repr__", &Panel::repr);


    py::class_<Pyramid>(m, "Pyramid")
        .def(py::init<ushort>(),
            "size"_a)
        .def(py::init<const std::shared_ptr<const Rules>>(),
            "rules"_a)
        .def(py::init<const Pyramid&>())

        .def("clear", &Pyramid::clear)
        .def("get_line",
            &Pyramid::get_line,
            "line"_a)
        .def("set_line",
            &Pyramid::set_line,
            "line"_a,
            "amount"_a,
            "color"_a)
        .def("clear_line",
            &Pyramid::clear_line,
            "line"_a)

        .def("is_filled",
            &Pyramid::is_filled,
            "line"_a)
        .def("is_empty",
            &Pyramid::is_empty,
            "line"_a)
        .def("amount",
            &Pyramid::amount,
            "line"_a)
        .def("amount_remaining",
            &Pyramid::amount_remaining,
            "line"_a)
        .def("color",
            &Pyramid::color,
            "line"_a)
        .def("accept_color",
            &Pyramid::accept_color,
            "line"_a,
            "color"_a)
        .def("filled", &Pyramid::filled)

        .def("__str__", &Pyramid::str)
        .def("__repr__", &Pyramid::repr);


    py::class_<State>(m, "State")
        .def(py::init<const std::shared_ptr<const Rules>>(),
            "rules"_a)
        .def(py::init<const State&>())

        .def("reset", &State::reset)

        .def_property_readonly("rules", &State::get_rules)
        .def_property_readonly("center", &State::get_center_mut)
        .def("factory",
            &State::get_factory_mut,
            "id"_a)
        .def("panel",
            &State::get_panel_mut,
            "id"_a)
        .def_property_readonly("bag", &State::get_bag_mut)
        .def_property_readonly("bin", &State::get_bin_mut)

        .def("get_total_tiles", &State::get_total_tiles)

        .def_property("current_player", &State::get_current_player, &State::set_current_player)
        .def("next_player", &State::next_player)

        .def("is_running_out_of_tiles", &State::is_running_out_of_tiles)
        .def("is_round_finished", &State::is_round_finished)
        .def("is_game_finished", &State::is_game_finished)
        .def("highest_score_players", &State::highest_score_players)
        .def("winning_player", &State::winning_player)

        .def("__str__", &State::str)
        .def("__repr__", &State::repr);


    py::class_<Wall>(m, "Wall")
        .def(py::init<const std::shared_ptr<const Rules>>(),
            "rules"_a)
        .def(py::init<const Wall&>())

        .def("clear", &Wall::clear)
        .def("get_placed_at",
            &Wall::get_placed_at,
            "Returns True if a tile is placed at the given position, and False otherwise"
            "x"_a,
            "y"_a)
        .def("get_tile_at",
            &Wall::get_tile_at,
            "Returns the tile placed at the given position, or Tile.NONE if there is none"
            "x"_a,
            "y"_a)
        .def("color_at",
            &Wall::color_at,
            "Returns which tile color should be placed at the given position"
            "x"_a,
            "y"_a)

        .def("get_placed",
            &Wall::get_placed,
            "Returns a list of boolean representing if a tile is placed or not at each position")
        .def("get_tiles",
            &Wall::get_tiles,
            "Returns the list of tiles placed, filling empty spaces with Tile.NONE")
        .def("get_placed_array",
            &Wall::get_placed_array,
            "Returns an array of boolean representing if a tile is placed or not at each position")
        .def("get_tiles_array",
            &Wall::get_tiles_array,
            "Returns the array of tiles placed, filling empty spaces with Tile.NONE")

        .def("line_has_color",
            &Wall::line_has_color,
            "line"_a,
            "color"_a)
        .def("line_color_x",
            &Wall::line_color_x,
            "The x coordinate of the tile on the given line that is supposed to be of the given color"
            "line"_a,
            "color"_a)

        .def("completed_column_count", &Wall::completed_column_count)
        .def("completed_line_count", &Wall::completed_line_count)
        .def("completed_type_count", &Wall::completed_type_count)
        .def("final_score_bonus", &Wall::final_score_bonus)

        .def("score_for_placing",
            &Wall::score_for_placing,
            "x"_a,
            "y"_a)
        .def("place_at",
            &Wall::place_at,
            "x"_a,
            "y"_a)
        .def("set_tile_at",
            &Wall::set_tile_at,
            "x"_a,
            "y"_a,
            "tile"_a)
        .def("place_line_color",
            &Wall::place_line_color,
            "line"_a,
            "color"_a)

        .def("__str__", &Wall::str)
        .def("__repr__", &Wall::repr);
}