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


Panel&
Panel::operator=(const Panel& other) {
    if (rules != other.rules) {
        throw std::logic_error("Cannot assign panel with different rules");
    }
    score = other.score;
    pyramid = other.pyramid;
    wall = other.wall;
    first_token = other.first_token;
    floor = other.floor;
    return *this;
}

bool
operator==(const Panel& left, const Panel& right) {
    return left.score == right.score &&
           left.pyramid == right.pyramid &&
           left.wall == right.wall &&
           left.first_token == right.first_token &&
           left.floor == right.floor;
}

bool
operator!=(const Panel& left, const Panel& right) {
    return !(left == right);
}

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
    floor = std::min(floor, rules->overflow_count);
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

std::ostream&
operator<<(std::ostream& os, const Panel& panel) {
    os << "Score: " << panel.score << (panel.first_token ? " (+token)" : "") << '\n';
    for (int line = 1; line <= panel.rules->tile_types; line++) {
        panel.wall.stream_line(os, line, true);
        os << ' ';
        panel.pyramid.stream_line(os, line, true);
        os << '\n';
    }
    os << "Floor: " << panel.floor << " (-" << panel.get_penalty() << ")" << '\n';
    return os;
}

std::string
Panel::str() const {
    std::ostringstream os;
    os << *this;
    return os.str();
}

std::string
Panel::repr() const {
    std::ostringstream os;
    os << "<Panel:" << score << (first_token ? "+" : "")
       << wall.repr() << pyramid.repr()
       << "[f:" << floor << "(-" << get_penalty() << ")]"
       << ">";
    return os.str();
}