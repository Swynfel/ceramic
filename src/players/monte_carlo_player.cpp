#include "monte_carlo_player.hpp"

#include "game/game.hpp"

MonteCarloPlayer::MonteCarloPlayer(bool until_round, int rollouts)
  : MonteCarloPlayer(std::make_shared<RandomPlayer>(), until_round, rollouts) {}

MonteCarloPlayer::MonteCarloPlayer(std::shared_ptr<Player> player, bool until_round, int rollouts)
  : sampling_player(std::move(player))
  , rollouts(rollouts)
  , until_round(until_round) {}

MonteCarloPlayer::MonteCarloPlayer(const MonteCarloPlayer& other)
  : sampling_player(other.sampling_player->copy())
  , heuristic(other.heuristic)
  , rollouts(other.rollouts)
  , until_round(other.until_round)
  , smart(other.smart)
  , c(other.c) {}

// Private

float
MonteCarloPlayer::state_score(Game& game, const State& state, int player) {
    game.override_state(state);
    if (until_round) {
        game.roll_round();
        game.end_round();
        game.score_final();
        if (!game.get_state().is_game_finished()) {
            return heuristic.eval(state, player);
        }
    } else {
        game.roll_end_game();
    }
    return (game.get_state().winning_player() == player) ? 1.f : 0.f;
}

// Selects action "a" of index "i" with highest
// Upper Confidence bound applied to Trees defined as:
// UCT_i = X_i + c * sqrt( ln(n) / n_i )
// with:
//    X_i average score after choosing "a"
//    c runtime constant, default is sqrt(2)
//    n total number of samples (n = sum_i(n_i))
//    n_i number of times action "a" was sampled
int
MonteCarloPlayer::select_ucb(int n, const std::vector<float>& score_sums, const std::vector<int>& count) const {
    float ln_n = log(n);
    float highest_uct = -std::numeric_limits<float>::infinity();
    int highest_index = 0;
    for (std::size_t i = 0; i < score_sums.size(); i++) {
        if (count[i] == 0) {
            return i;
        }
        float uct_i = (score_sums[i] / count[i]) + c * sqrt(ln_n / count[i]);
        if (uct_i > highest_uct) {
            highest_uct = uct_i;
            highest_index = i;
        }
    }
    return highest_index;
}

Action
MonteCarloPlayer::best_action(std::vector<Action> actions, std::vector<float> score_sums, std::vector<int> count) const {
    float best_score = -std::numeric_limits<float>::infinity();
    Action best_action;
    int index = 0;
    for (Action action : actions) {
        float score = count[index] > 0 ? score_sums[index] / count[index] : 0.f;
        if (score > best_score) {
            best_score = score;
            best_action = action;
        }
        index++;
    }
    return best_action;
}


std::shared_ptr<Player>
MonteCarloPlayer::copy() const {
    return std::make_shared<MonteCarloPlayer>(*this);
}

Action
MonteCarloPlayer::play(const State& state) {
    int position = state.get_current_player();
    std::vector<Action> legal_actions;
    if (smart) {
        legal_actions = Game::all_smart_legal(state);
    } else {
        legal_actions = Game::all_legal(state);
    }
    std::shuffle(legal_actions.begin(), legal_actions.end(), randomness);
    std::vector<float> score_sums(legal_actions.size(), 0);
    std::vector<int> count(legal_actions.size(), 0);

    Game game = Game(state.get_rules());
    for (int p = 0; p < state.get_rules()->player_count; p++) {
        game.add_player(sampling_player);
    }

    for (int k = 0; k < rollouts; k++) {
        int index = select_ucb(k, score_sums, count);
        State next_state(state);
        Game::apply(legal_actions[index], next_state);
        float score = state_score(game, next_state, position);
        count[index]++;
        score_sums[index] += score;
    }

    return best_action(legal_actions, score_sums, count);
}


std::string
MonteCarloPlayer::player_type() const {
    return "mc-" +
           std::to_string(rollouts) +
           (smart ? "" : "-naive") +
           "-" + (until_round ? heuristic.str() : "full");
}