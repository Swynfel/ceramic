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
    const std::shared_ptr<Rules> rules;
    Center center;
    std::vector<Factory> factories;
    std::vector<Panel> panels;
    Tiles bag;
    Tiles bin;
    ushort player;

    void assert_player_id(ushort id) const;
    void assert_factory_id(ushort id) const;

public:
    State(const std::shared_ptr<Rules>& rules);
    State(const State& state);

    void reset();

    const std::shared_ptr<Rules>& get_rules() const;
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

    void set_current_player(ushort id);
    void next_player();

    bool is_round_finished() const;
    bool is_game_finished() const;

    // Reading
    friend ostream& operator<<(ostream& os, const State& state);
    string str() const;
    string repr() const;
};

#endif //STATE_HPP
