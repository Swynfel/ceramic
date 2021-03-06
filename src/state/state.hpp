#ifndef STATE_HPP
#define STATE_HPP

#include <memory>
#include <string>
#include <vector>

#include "center.hpp"
#include "factory.hpp"
#include "panel.hpp"
#include "rules/rules.hpp"
#include "tiles.hpp"

class State {
    friend class Game;

private:
    const std::shared_ptr<const Rules> rules;
    Center center;
    std::vector<Factory> factories;
    std::vector<Panel> panels;
    Tiles bag;
    Tiles bin;
    ushort player;

    void assert_player_id(ushort id) const;
    void assert_factory_id(ushort id) const;

public:
    State(std::shared_ptr<const Rules> rules);
    State(const State& state);

    State& operator=(const State& other);
    friend bool operator==(const State& left, const State& right);
    friend bool operator!=(const State& left, const State& right);

    void reset();

    const std::shared_ptr<const Rules>& get_rules() const;
    ushort get_current_player() const;

    const Center& get_center() const;
    Center& get_center_mut();
    const Factory& get_factory(ushort id) const;
    Factory& get_factory_mut(ushort id);
    const Panel& get_panel(ushort id) const;
    Panel& get_panel_mut(ushort id);

    Tiles get_bag() const;
    Tiles& get_bag_mut();
    Tiles get_bin() const;
    Tiles& get_bin_mut();

    Tiles get_total_tiles() const;

    void set_current_player(ushort id);
    void next_player();

    bool is_running_out_of_tiles() const;
    bool is_round_finished() const;
    bool is_game_finished() const;
    std::vector<ushort> highest_score_players() const;
    ushort winning_player() const;

    // Reading
    friend std::ostream& operator<<(std::ostream& os, const State& state);
    std::string str() const;
    std::string repr() const;
};

#endif //STATE_HPP
