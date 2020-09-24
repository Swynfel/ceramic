#include "monte_carlo_player.hpp"

#include "game/game.hpp"

MonteCarloPlayer::MonteCarloPlayer(bool until_round, int rollouts)
  : MonteCarloPlayer(std::make_shared<RandomPlayer>(), until_round, rollouts) {}

MonteCarloPlayer::MonteCarloPlayer(std::shared_ptr<Player> player, bool until_round, int rollouts)
  : sampling_player(std::move(player))
  , rollouts(rollouts)
  , until_round(until_round) {}

// Private

float
MonteCarloPlayer::end_round_heuristic(int total_score, int player_score, int highest_score, int player_count) {
    float first_factor = 0.2f;
    float scale_factor = 0.6f;
    float score_factor = 1.f - first_factor - scale_factor;
    return total_score == 0 ? 1.f / player_count
                            : ((player_score >= highest_score) ? first_factor : 0.f) +
                                  scale_factor * float(player_score) / highest_score +
                                  score_factor * float(player_score) / total_score;
}

float
MonteCarloPlayer::state_score(Game& game, const State& state, int player) {
    game.override_state(state);
    if (until_round) {
        game.roll_round();
        game.end_round();
        if (game.get_state().is_game_finished()) {
            game.score_final();
            return (game.get_state().winning_player() == player) ? 1 : 0;
        }
        int player_score = 0;
        int total_score = 0;
        int highest_score = 0;
        int player_count = state.get_rules()->player_count;
        for (int p = 0; p < player_count; p++) {
            int score = game.get_state().get_panel(p).get_score();
            total_score += score;
            if (p == player) {
                player_score = score;
            }
            if (score > highest_score) {
                highest_score = score;
            }
        }
        return end_round_heuristic(total_score, player_score, highest_score, player_count);
    } else {
        game.roll_game();
        return (game.get_state().winning_player() == player) ? 1 : 0;
    }
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
MonteCarloPlayer::select_ucb(int n, const vector<float>& score_sums, const vector<int>& count) const {
    float ln_n = log(n);
    float highest_uct = -std::numeric_limits<float>::infinity();
    int highest_index = 0;
    for (int i = 0; i < score_sums.size(); i++) {
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
MonteCarloPlayer::play(const State& state) {
    int position = state.get_current_player();
    vector<Action> legal_actions;
    if (smart) {
        legal_actions = Game::all_smart_legal(state);
        if (legal_actions.size() == 0) {
            legal_actions = Game::all_penalty_legal(state);
        }
    } else {
        legal_actions = Game::all_legal(state);
    }
    vector<float> score_sums(legal_actions.size(), 0);
    vector<int> count(legal_actions.size(), 0);

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

    float best_score = -std::numeric_limits<float>::infinity();
    Action best_action;
    int index = 0;
    for (Action action : legal_actions) {
        float score = count[index] > 0 ? score_sums[index] / count[index] : 0.f;
        if (score > best_score) {
            best_score = score;
            best_action = action;
        }
        index++;
    }
    // index = 0;
    // std::cout << ":: ";
    // for (Action action : legal_actions) {
    //     float score = count[index] > 0 ? score_sums[index] / count[index] : 0.f;
    //     std::cout << action;
    //     printf("(%i|%.3f) ", count[index], score);
    //     index++;
    // }
    // std::cout << std::endl;
    return best_action;
}


string
MonteCarloPlayer::player_type() const {
    return "monte-carlo";
}