#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "action.hpp"
#include "game.hpp"
#include "player.hpp"
#include "py_utils.hpp"

namespace py = pybind11;

Action
py_create_action(
    ushort pick,
    Tile color,
    ushort place) {
    return Action{
        .pick = pick,
        .color = color,
        .place = place,
    };
}

void
py_bind_game(py::module& root) {
    py::module m = root.def_submodule("game");

    py::class_<Action>(m, "Action")
        .def(py::init(&py_create_action),
            py::arg("pick"),
            py::arg("color"),
            py::arg("place"))

        .def_readwrite("pick", &Action::pick)
        .def_readwrite("color", &Action::color)
        .def_readwrite("place", &Action::place)

        .def("__eq__", &py_eq<Action>)
        .def("__ne__", &py_ne<Action>)

        .def("__str__", &Action::str)
        .def("__repr__", &Action::repr);

    py::class_<Game>(m, "Game")
        .def(py::init<>())
        .def(py::init<std::shared_ptr<Rules>&>())
        .def(py::init<std::shared_ptr<Rules>&, vector<Player>>())

        .def_property_readonly("state", &Game::get_state)

        .def("players_missing", &Game::players_missing)
        .def("has_enough_players", &Game::has_enough_players)
        .def("add_player", &Game::add_player)
        .def("add_players", &Game::add_players)

        .def("reset", &Game::reset)
        .def("start_round", &Game::start_round)
        .def("end_round", &Game::end_round)
        .def("score_final", &Game::score_final)

        .def("roll_round", &Game::roll_round)
        .def("roll_game", &Game::roll_game)
        .def("next_player", &Game::next_player)

        .def("setup_factories", &Game::setup_factories)
        .def("score_panels", [](Game& game) { game.score_panels(); })
        .def("apply_first_token", [](Game& game) { game.apply_first_token(); })

        .def("legal", [](const Game& game, Action action) { return game.legal(action); })
        .def("apply", [](Game& game, Action action) { game.apply(action); });


    py::class_<Player>(m, "Player")
        .def(py::init<>())

        .def_property_readonly("id", &Player::get_id)
        .def_property_readonly("position", &Player::get_position)
        .def_property_readonly("joined_game", &Player::has_joined_game)

        .def("play", &Player::play);
}