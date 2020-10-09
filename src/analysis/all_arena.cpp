#include "all_arena.hpp"

#include "groups_utils.hpp"

void
AllArena::generate_groups(int available_players, int game_players) {
    GroupUtils::all(groups, available_players, game_players, true);
}


std::string
AllArena::mode_name() const {
    return "All";
}

bool
AllArena::ready() const {
    return players.size() >= 2;
}