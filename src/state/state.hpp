#ifndef STATE_HPP
#define STATE_HPP

#include <memory>
#include <string>
#include <vector>

#include "center.hpp"
#include "factory.hpp"
#include "panel.hpp"
#include "rules.hpp"
#include "tiles.hpp"

class State {
    friend class Game;

private:
    const Rules rules;
    Center center;
    std::vector<Factory> factories;
    std::vector<Panel> panels;
    Tiles bag;
    Tiles bin;
    ushort player;

    void assert_player_id(const ushort id) const;
    void assert_factory_id(const ushort id) const;

public:
    State(const Rules& rules);
    State(const State& state);

    void reset();

    const Rules& get_rules() const;
    const ushort get_current_player() const;

    const Center& get_center() const;
    Center& get_center_mut();
    const Factory& get_factory(const ushort id) const;
    Factory& get_factory_mut(const ushort id);
    const Panel& get_panel(const ushort id) const;
    Panel& get_panel_mut(const ushort id);

    Tiles get_bag() const;
    Tiles& get_bag_mut();
    Tiles get_bin() const;
    Tiles& get_bin_mut();

    void set_current_player(const ushort id);
    void next_player();

    // Reading
    friend ostream& operator<<(ostream& os, const State& state);
    string str() const;
    string repr() const;
};

#endif //STATE_HPP
