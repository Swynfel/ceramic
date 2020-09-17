#include "panel.hpp"

#include <sstream>

Panel::Panel(const std::shared_ptr<const Rules>& rules)
  : rules(rules)
  , score(0)
  , pyramid(rules->tile_types)
  , wall(rules)
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

ushort
Panel::get_score() const {
    return score;
}

void
Panel::add_score(int value) {
    if (score > -value) {
        score += value;
    } else {
        score = 0;
    }
}

const Pyramid&
Panel::get_pyramid() const {
    return pyramid;
}

Pyramid&
Panel::get_pyramid_mut() {
    return pyramid;
}

const Wall&
Panel::get_wall() const {
    return wall;
}

Wall&
Panel::get_wall_mut() {
    return wall;
}

bool
Panel::get_first_token() const {
    return first_token;
}

void
Panel::set_first_token(bool value) {
    first_token = value;
}

ushort
Panel::get_floor() const {
    return floor;
}

void
Panel::add_floor(ushort value) {
    floor += value;
    floor = min(floor, rules->overflow_count);
}

void
Panel::clear_floor() {
    floor = 0;
}

ushort
Panel::get_penalty() const {
    return rules->penalty_for_floor(floor);
}


bool
Panel::legal_line(ushort line, Tile tile) const {
    if (wall.line_has_color(line, tile)) {
        return false;
    }
    return pyramid.accept_color(line, tile);
}


// Reading

ostream&
operator<<(ostream& os, const Panel& panel) {
    os << "Score: " << panel.score << (panel.first_token ? " (+token)" : "") << endl;
    for (int line = 1; line <= panel.rules->tile_types; line++) {
        panel.wall.stream_line(os, line, true);
        os << ' ';
        panel.pyramid.stream_line(os, line, true);
        os << endl;
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