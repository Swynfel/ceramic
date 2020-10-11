#ifndef ROUND_HEURISTIC_HPP
#define ROUND_HEURISTIC_HPP

#include "global.hpp"
#include "state/pyramid.hpp"
#include "state/state.hpp"
#include "state/wall.hpp"

#include <sstream>
#include <string>

struct RoundHeuristic {
    float bonus_factor = 0.2f;
    float leading_factor = 0.2f;
    float penalty_factor = 0.f;

    std::string str() const {
        std::stringstream ss;
        ss << "h(" << bonus_factor << "," << leading_factor << "," << penalty_factor << ")";
        return ss.str();
    }

    float eval_winrate(
        float total_score,
        float player_score,
        float highest_score,
        int player_count) const {
        return (highest_score == 0) ? 1.f / player_count : leading_factor * (player_score / highest_score) + (1.f - leading_factor) * (player_score / total_score);
    }

    float eval_score(
        int score,
        int incomplete_pyramid_lines,
        int squared_wall_columns,
        int squared_wall_lines,
        int squared_wall_colors,
        const Rules& rules) const {
        int n = rules.tile_types;
        float result = score;
        result -= penalty_factor * incomplete_pyramid_lines;
        result += bonus_factor * (rules.column_bonus * squared_wall_columns + rules.line_bonus * squared_wall_lines + rules.type_bonus * squared_wall_colors) / (n * n);
        result = std::max(result, 0.f);
        return result;
    }

    float eval(const State& state, int player) const {
        const Rules& rules = *state.get_rules();
        int player_count = state.get_rules()->player_count;
        int n = state.get_rules()->tile_types;
        //
        float total_score = 0;
        float player_score = 0;
        float highest_score = 0;
        for (int p = 0; p < player_count; p++) {
            const Panel& panel = state.get_panel(p);
            const Pyramid& pyramid = panel.get_pyramid();
            const Wall& wall = panel.get_wall();
            int raw_score = panel.get_score();
            int incomplete_pyramid_lines = 0;
            for (int line = 1; line <= n; line++) {
                if (!pyramid.is_empty(line)) {
                    incomplete_pyramid_lines++;
                }
            }
            int squared_wall_columns = 0;
            int squared_wall_lines = 0;
            int squared_wall_colors = 0;
            for (int i = 1; i <= n; i++) {
                int value;
                value = wall.column_tile_count(i);
                if (value < n) {
                    squared_wall_columns += value * value;
                }
                value = wall.line_tile_count(i);
                if (value < n) {
                    squared_wall_lines += value * value;
                }
                value = wall.type_tile_count(i - 1);
                if (value < n) {
                    squared_wall_colors += value * value;
                }
            }
            float score = eval_score(raw_score, incomplete_pyramid_lines, squared_wall_columns, squared_wall_lines, squared_wall_colors, rules);
            total_score += score;
            if (p == player) {
                player_score = score;
            }
            if (score > highest_score) {
                highest_score = score;
            }
        }
        return eval_winrate(total_score, player_score, highest_score, player_count);
    }
};

#endif //ROUND_HEURISTIC_HPP
