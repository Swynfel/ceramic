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
    const std::shared_ptr<Rules> rules;
    std::vector<Factory> factories;
    std::vector<Panel> panels;

public:
    State(const std::shared_ptr<Rules>& rules);
    //State(const State& state);

    void start();
    void start_round();
};

#endif //STATE_HPP
