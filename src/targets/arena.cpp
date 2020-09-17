#include <iostream>

#include <unistd.h>

#include "game/arena.hpp"
#include "players/first_legal_player.hpp"
#include "players/random_player.hpp"

int
main(int argc, char* argv[]) {
    Arena arena{};
    std::shared_ptr<Player> first_legal_player = std::make_shared<FirstLegalPlayer>();
    std::shared_ptr<Player> smart_random_player = std::make_shared<RandomPlayer>();
    std::shared_ptr<Player> naive_random_player = std::make_shared<RandomPlayer>(false);
    arena.add_player(first_legal_player);
    arena.add_player(naive_random_player);
    arena.add_player(smart_random_player);
    arena.mode = Arena::MODE::ALL;
    arena.run();
}
