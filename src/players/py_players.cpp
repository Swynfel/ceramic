#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "first_legal_player.hpp"
#include "monte_carlo_player.hpp"
#include "random_player.hpp"
#include "round_heuristic.hpp"
#include "terminal_player.hpp"

namespace py = pybind11;
using namespace py::literals;

void
py_bind_players(py::module& root) {
    py::module m = root.def_submodule("players");

    py::class_<FirstLegalPlayer, std::shared_ptr<FirstLegalPlayer>, Player>(m, "FirstLegalPlayer")
        .def(py::init<>());

    py::class_<RandomPlayer, std::shared_ptr<RandomPlayer>, Player>(m, "RandomPlayer")
        .def(py::init<bool>(),
            "smart"_a = true)
        .def(py::init<int, bool>(),
            "seed"_a,
            "smart"_a = true)
        .def_readonly("smart", &RandomPlayer::smart);

    py::class_<TerminalPlayer> terminal_player =
        py::class_<TerminalPlayer, std::shared_ptr<TerminalPlayer>, Player>(m, "TerminalPlayer");
    terminal_player.def(py::init<>());

    py::enum_<TerminalPlayer::ColoredType>(terminal_player, "ColoredType")
        .value("NONE", TerminalPlayer::ColoredType::NONE)
        .value("GREY", TerminalPlayer::ColoredType::GREY)
        .value("FULL", TerminalPlayer::ColoredType::FULL);

    py::class_<MonteCarloPlayer, std::shared_ptr<MonteCarloPlayer>, Player>(m, "MonteCarloPlayer")
        .def(py::init<bool, int>(),
            "until_round"_a = true,
            "rollouts"_a = MonteCarloPlayer::DEFAULT_ROLLOUTS)
        .def(py::init<std::shared_ptr<Player>, bool, int>(),
            "player"_a,
            "until_round"_a = true,
            "rollouts"_a = MonteCarloPlayer::DEFAULT_ROLLOUTS)
        .def_readwrite("heuristic", &MonteCarloPlayer::heuristic)
        .def_readwrite("rollouts", &MonteCarloPlayer::rollouts)
        .def_readwrite("until_round", &MonteCarloPlayer::until_round)
        .def_readwrite("smart", &MonteCarloPlayer::smart)
        .def_readwrite("c", &MonteCarloPlayer::c)
        .def_property_readonly_static("DEFAULT_ROLLOUTS", []() { return MonteCarloPlayer::DEFAULT_ROLLOUTS; })
        .def_property_readonly_static("DEFAULT_C", []() { return MonteCarloPlayer::DEFAULT_C; });

    py::class_<RoundHeuristic>(m, "RoundHeuristic")
        .def(py::init<>())
        .def("eval_winrate",
            &RoundHeuristic::eval_winrate,
            "total_score"_a,
            "player_score"_a,
            "highest_score"_a,
            "player_count"_a)
        .def("eval_score",
            &RoundHeuristic::eval_score,
            "score"_a,
            "incomplete_pyramid_lines"_a,
            "squared_wall_columns"_a,
            "squared_wall_lines"_a,
            "squared_wall_colors"_a,
            "rules"_a)
        .def("eval",
            &RoundHeuristic::eval,
            "state"_a,
            "player"_a)
        .def_readwrite("penalty_factor", &RoundHeuristic::penalty_factor)
        .def_readwrite("bonus_factor", &RoundHeuristic::bonus_factor)
        .def_readwrite("leading_factor", &RoundHeuristic::leading_factor);
}