#include "panel.hpp"

#include <sstream>

Panel::Panel(const Rules& rules)
  : rules(rules)
  , score(0)
  , pyramid(rules.tile_types)
  , wall(rules.tile_types)
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

void
Panel::clear() {
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

Pyramid
Panel::get_pyramid_mut() {
    return pyramid;
}

const Wall
Panel::get_wall() const {
    return wall;
}

Wall
Panel::get_wall_mut() {
    return wall;
}

const bool
Panel::get_first_token() const {
    return first_token;
}

void
Panel::set_first_token(bool value) {
    first_token = value;
}

const ushort
Panel::get_floor() const {
    return floor;
}

void
Panel::add_floor(const ushort value) {
    floor += value;
    floor = max(floor, rules.overflow_count);
}

void
Panel::clear_floor() {
    floor = 0;
}

const ushort
Panel::get_penalty() const {
    return rules.penalty_for_floor(get_floor());
}

// Reading

ostream&
operator<<(ostream& os, const Panel& panel) {
    os << "Score: " << panel.score << (panel.first_token ? " (+token)" : "") << endl;
    for (int line = 1; line <= panel.rules.tile_types; line++) {
        os << (line == 1 ? '[' : ' ');
        panel.wall.stream_line(os, line, true);
        os << ' ';
        panel.pyramid.stream_line(os, line, true);
        os << (line == panel.rules.tile_types ? ']' : '\n');
    }
    os << "Floor: " << panel.floor << " (-" << panel.get_penalty() << ")" << endl;
    return os;
}

string
Panel::str() const {
    ostringstream os;
    os << *this;
    return os.str();
}

string
Panel::repr() const {
    ostringstream os;
    os << "<Panel:" << score << (first_token ? "+" : "")
       << wall.repr() << pyramid.repr()
       << "[f:" << floor << "(-" << get_penalty() << ")]"
       << ">";
    return os.str();
}