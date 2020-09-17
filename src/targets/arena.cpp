#include <iostream>

#include <unistd.h>

#include "game/arena.hpp"
#include "players/random_player.hpp"

int
main(int argc, char* argv[]) {
    Arena arena{};
    std::shared_ptr<Player> player = std::make_shared<RandomPlayer>();
    arena.add_player(player);
    arena.add_player(player);
    arena.add_player(player);
    arena.add_player(player);
    arena.add_player(player);
    arena.mode = Arena::MODE::ALL;
    arena.run();
}
