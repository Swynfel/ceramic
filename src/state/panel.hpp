#ifndef PANEL_HPP
#define PANEL_HPP

#include <memory>

#include "../global.hpp"
#include "pyramid.hpp"
#include "rules.hpp"
#include "wall.hpp"

class Panel {
private:
    const Rules rules;
    ushort score;
    Pyramid pyramid;
    Wall wall;
    bool first_token;
    ushort floor;

public:
    Panel(const Rules& rules);
    Panel(const ushort size);
    Panel(const Panel& panel);

    void clear();

    const ushort get_score() const;
    const Pyramid get_pyramid() const;
    Pyramid get_pyramid_mut();
    const Wall get_wall() const;
    Wall get_wall_mut();
    const bool get_first_token() const;
    void set_first_token(const bool value);
    const ushort get_floor() const;
    void add_floor(const ushort value);
    void clear_floor();
    const ushort get_penalty() const;

    // Reading
    friend ostream& operator<<(ostream& os, const Panel& panel);
    string str() const;
    string repr() const;
};

#endif //PANEL_HPP
