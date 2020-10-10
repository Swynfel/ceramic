#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "all_arena.hpp"
#include "arena.hpp"
#include "pairs_arena.hpp"

namespace py = pybind11;
using namespace py::literals;

class PublicArena : public Arena {
public:
    using Arena::players;
    using Arena::results;

    using Arena::add_group;

    using Arena::add_results;
    using Arena::column_count;
    using Arena::generate_groups;
    using Arena::print_results;
};

class _PyArena : public Arena {
public:
    using Arena::Arena;

    int column_count() const override {
        PYBIND11_OVERLOAD(
            int,
            Arena,
            column_count);
    }
    void generate_groups(int available_players, int game_players) override {
        PYBIND11_OVERLOAD(
            void,
            Arena,
            generate_groups,
            available_players,
            game_players);
    }

    void print_results(std::vector<std::vector<int>> results) override {
        PYBIND11_OVERLOAD(
            void,
            Arena,
            print_results,
            results);
    }
};

void
py_bind_arena(py::module& root) {
    py::module m = root.def_submodule("arena");

    py::class_<Arena, _PyArena>(m, "Arena")
        .def(py::init())
        .def(py::init<std::shared_ptr<Rules>, std::vector<std::shared_ptr<Player>>>(),
            "rules"_a,
            "players"_a = new std::vector<std::shared_ptr<Player>>())

        .def_readwrite("players", &PublicArena::players)
        .def_readwrite("results", &PublicArena::results)

        .def("add_group", &PublicArena::add_group)
        .def("add_results", &PublicArena::add_results)

        .def("column_count", &PublicArena::column_count)
        .def("generate_groups", &PublicArena::generate_groups)
        .def("print_results", &PublicArena::print_results)

        .def_readwrite("count", &Arena::count)
        .def_readwrite("thread_limit", &Arena::thread_limit)
        .def_readwrite("detailed_player_analysis", &Arena::detailed_player_analysis)
        .def_readwrite("rules", &Arena::rules)

        .def("mode_name", &Arena::mode_name)
        .def("ready", &Arena::ready)
        .def("run", &Arena::run)
        .def("print", &Arena::print)
        .def("run_print", &Arena::run_print);

    py::class_<AllArena, Arena>(m, "AllArena")
        .def(py::init())
        .def(py::init<std::shared_ptr<Rules>, std::vector<std::shared_ptr<Player>>>(),
            "rules"_a,
            "players"_a = new std::vector<std::shared_ptr<Player>>());

    py::class_<PairsArena, Arena>(m, "PairsArena")
        .def(py::init())
        .def(py::init<std::shared_ptr<Rules>, std::vector<std::shared_ptr<Player>>>(),
            "rules"_a,
            "players"_a = new std::vector<std::shared_ptr<Player>>());
}