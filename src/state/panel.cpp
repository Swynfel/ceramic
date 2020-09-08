#include "panel.hpp"

Panel::Panel(std::shared_ptr<Rules> rules)
  : rules(rules)
  , score(0)
  , pyramid(rules->tile_types)
  , wall(rules->tile_types)
  , first_token(false)
  , floor(0) {}

Panel::Panel(const ushort size)
  : rules()
  , score(0)
  , pyramid(size)
  , wall(size)
  , first_token(false)
  , floor(0) {}

Panel::Panel(const Panel& panel)
  : rules(panel.rules)
  , score(panel.score)
  , pyramid(panel.pyramid)
  , wall(panel.wall)
  , first_token(panel.first_token)
  , floor(panel.floor) {}


void Panel::clear() {
    score = 0;
    pyramid.clear();
    wall.clear();
    first_token = false;
    floor = 0;
}


const ushort
Panel::get_score() const {
    return score;
}

const Pyramid
Panel::get_pyramid() const {
    return pyramid;
}

const Wall
Panel::get_wall() const {
    return wall;
}

const bool
Panel::get_first_token() const {
    return first_token;
}

const ushort
Panel::get_floor() const {
    return floor;
}

const ushort
Panel::get_penalty() const {
    if (auto strong_rules = rules.lock()) {
        return strong_rules->penalty_for_floor(get_floor());
    }
    throw runtime_error("Unable compute penalty with expired rules");
}
