#ifndef STATE_HPP
#define STATE_HPP

#include <memory>
#include <vector>

#include "factory.hpp"
#include "panel.hpp"
#include "rules.hpp"
#include "tiles.hpp"

class State {
private:
    const Rules rules;
    std::vector<Factory> factories;
    std::vector<Panel> panels;
    Tiles bag;
    Tiles bin;
    ushort player;

public:
    State(const Rules& rules);
    //State(const State& state);

    void start();
    void start_round();

    const Rules& get_rules() const;
    const ushort get_current_player() const;
    void set_current_player(const ushort id);
    void next_player() const;
};

#endif //STATE_HPP
