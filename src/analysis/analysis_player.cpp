#include "analysis_player.hpp"

#include <chrono>
#include <iostream>

AnalysisPlayer::AnalysisPlayer(std::shared_ptr<Player> player)
  : Player()
  , analysed_player(player) {}


Action
AnalysisPlayer::play(const State& state) {
    auto begin = std::chrono::high_resolution_clock::now();
    Action choice = analysed_player->play(state);
    auto end = std::chrono::high_resolution_clock::now();
    move_counter.fetch_add(1);
    time.fetch_add(std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count());
    return choice;
}

void
AnalysisPlayer::error(std::string error) {
    analysed_player->error(error);
}


std::string
AnalysisPlayer::player_type() const {
    return "analysis-" + analysed_player->player_type();
}


std::shared_ptr<Observer>
AnalysisPlayer::observer() {
    std::shared_ptr<Observer> obs = analysed_player->observer();
    if (obs != nullptr) {
        std::cout << "Warning: Analysis Player doesn't keep track of observer";
    }
    return obs;
}