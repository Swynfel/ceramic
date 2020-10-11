#include "random_player.hpp"

#include "game/game.hpp"

RandomPlayer::RandomPlayer(bool smart)
  : RandomPlayer(random_seed(), smart) {}

RandomPlayer::RandomPlayer(int seed, bool smart)
  : randomness(seed)
  , range()
  , smart(smart) {}


std::shared_ptr<Player>
RandomPlayer::copy() const {
    return std::make_shared<RandomPlayer>(*this);
}

Action
RandomPlayer::play(const State& state) {
    std::vector<Action> legal_actions;
    if (smart) {
        legal_actions = Game::all_smart_legal(state);
    } else {
        legal_actions = Game::all_legal(state);
    }
    if (legal_actions.size() == 1) {
        return legal_actions[0];
    }
    ushort index = random_range(randomness, range, 0, legal_actions.size());
    return legal_actions[index];
}


std::string
RandomPlayer::player_type() const {
    return std::string("random") + (smart ? "" : "-naive");
}