#ifndef PANEL_HPP
#define PANEL_HPP

#include <memory>

#include "../global.hpp"
#include "pyramid.hpp"
#include "rules.hpp"
#include "wall.hpp"

class Panel {
private:
    const std::weak_ptr<Rules> rules;
    ushort score;
    Pyramid pyramid;
    Wall wall;
    bool first_token;
    ushort floor;

public:
    Panel(std::shared_ptr<Rules> rules);
    Panel(ushort size);
    Panel(const Panel& panel);

    const ushort get_score() const;
    const Pyramid get_pyramid() const;
    const Wall get_wall() const;
    const bool get_first_token() const;
    const ushort get_floor() const;
    const ushort get_penalty() const;
};

#endif //PANEL_HPP
