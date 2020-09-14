#include "random_player.hpp"

#include "game/game.hpp"

RandomPlayer::RandomPlayer()
  : RandomPlayer(random_seed()) {}

RandomPlayer::RandomPlayer(int seed)
  : randomness(seed)
  , range() {}


Action
RandomPlayer::play(const State& state) {
    vector<Action> legal_actions = Game::all_legal(state);
    if (legal_actions.size() == 0) {
        throw runtime_error("No action was legal");
    }
    ushort index = random_range(randomness, range, 0, legal_actions.size());
    return legal_actions[index];
}


string
RandomPlayer::player_type() const {
    return "random";
}