#ifndef PANEL_HPP
#define PANEL_HPP

#include <memory>

#include "global.hpp"
#include "pyramid.hpp"
#include "rules/rules.hpp"
#include "wall.hpp"

class Panel {
private:
    const std::shared_ptr<const Rules> rules;
    ushort score;
    Pyramid pyramid;
    Wall wall;
    bool first_token;
    ushort floor;

public:
    Panel(const std::shared_ptr<const Rules>& rules);
    Panel(const Panel& panel);

    Panel& operator=(const Panel& other);
    friend bool operator==(const Panel& left, const Panel& right);
    friend bool operator!=(const Panel& left, const Panel& right);

    void clear();

    ushort get_score() const;
    void add_score(int value);
    const Pyramid& get_pyramid() const;
    Pyramid& get_pyramid_mut();
    const Wall& get_wall() const;
    Wall& get_wall_mut();
    bool get_first_token() const;
    void set_first_token(bool value);
    ushort get_floor() const;
    void add_floor(ushort value);
    void clear_floor();
    ushort get_penalty() const;

    bool legal_line(ushort line, Tile tile) const;

    // Reading
    friend std::ostream& operator<<(std::ostream& os, const Panel& panel);
    std::string str() const;
    std::string repr() const;
};

#endif //PANEL_HPP
