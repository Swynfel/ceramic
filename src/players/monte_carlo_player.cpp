#include "monte_carlo_player.hpp"

#include "game/game.hpp"

MonteCarloPlayer::MonteCarloPlayer(bool until_round, int rollouts)
  : MonteCarloPlayer(std::make_shared<RandomPlayer>(), until_round, rollouts) {}

MonteCarloPlayer::MonteCarloPlayer(std::shared_ptr<Player> player, bool until_round, int rollouts)
  : sampling_player(std::move(player))
  , rollouts(rollouts)
  , until_round(until_round) {}


float
MonteCarloPlayer::game_score(Game& game, const State& state, int player) {
    float score = 0;
    for (int k = 0; k < rollouts; k++) {
        game.override_state(state);
        game.roll_game();
        if (game.get_state().winning_player() == player) {
            score++;
        }
    }
    return score;
}


float
MonteCarloPlayer::round_score(Game& game, const State& state, int player) {
    float score = 0;
    for (int k = 0; k < rollouts; k++) {
        game.override_state(state);
        game.roll_round();
        game.end_round();
        float s = 0;
        for (int p = 0; p < state.get_rules()->player_count; p++) {
            s += game.get_state().get_panel(p).get_score();
        }
        s = game.get_state().get_panel(player).get_score() - (s / state.get_rules()->player_count);
        score += s;
    }
    return score;
}

Action
MonteCarloPlayer::play(const State& state) {
    int position = state.get_current_player();
    vector<Action> legal_actions;
    if (true) {
        legal_actions = Game::all_smart_legal(state);
        if (legal_actions.size() == 0) {
            legal_actions = Game::all_penalty_legal(state);
        }
    } else {
        legal_actions = Game::all_legal(state);
    }
    float best_score = -std::numeric_limits<float>::infinity();
    Action best_action;

    Game game = Game(state.get_rules());
    for (int p = 0; p < state.get_rules()->player_count; p++) {
        game.add_player(sampling_player);
    }

    for (Action action : legal_actions) {
        State next_state(state);
        Game::apply(action, next_state);
        float score = until_round ? round_score(game, state, position) : game_score(game, state, position);
        if (score > best_score) {
            best_score = score;
            best_action = action;
        }
    }
    return best_action;
}


string
MonteCarloPlayer::player_type() const {
    return "monte-carlo";
}