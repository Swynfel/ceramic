#include "wall.hpp"

#include <algorithm>
#include <numeric>
#include <sstream>

Wall::Wall(const std::shared_ptr<const Rules>& rules)
  : rules(rules)
  , placed(rules->tile_types_2(), Tile::NONE) {}

Wall::Wall(const Wall& wall)
  : rules(wall.rules)
  , placed(wall.placed) {}

Wall&
Wall::operator=(const Wall& other) {
    if (rules != other.rules) {
        throw logic_error("Cannot assign wall with different rules");
    }
    placed = other.placed;
    return *this;
}

// Private

void
Wall::assert_line(ushort line) const {
    if (line == 0) {
        throw std::range_error("No line '0', as it designates the floor.");
    }
    if (line > rules->tile_types) {
        throw std::range_error("No line '" + to_string(line) + "', as there is only '" + to_string(rules->tile_types) + "' lines.");
    }
}

void
Wall::assert_column(ushort column) const {
    if (column == 0 || column > rules->tile_types) {
        throw std::range_error("No column '" + to_string(column) + "', choose 'column' such as 0 < 'column' <= " + to_string(rules->tile_types) + ".");
    }
}

inline Tile
Wall::get_tile_at_unsafe(ushort x, ushort y) const {
    return placed[x - 1 + (y - 1) * rules->tile_types];
}

inline void
Wall::set_tile_at_unsafe(ushort x, ushort y, Tile tile) {
    placed[x - 1 + (y - 1) * rules->tile_types] = tile;
}

// Methods

void
Wall::clear() {
    placed.assign(rules->tile_types_2(), Tile::NONE);
}

bool
Wall::get_placed_at(ushort x, ushort y) const {
    return bool(get_tile_at(x, y));
}

Tile
Wall::get_tile_at(ushort x, ushort y) const {
    assert_line(y);
    assert_column(x);
    return get_tile_at_unsafe(x, y);
}

Tile
Wall::color_at(ushort x, ushort y) const {
    assert_line(y);
    assert_line(x);
    return Tile(x >= y ? x - y : rules->tile_types + x - y);
}


vector<bool>
vec_tile_to_bool(vector<Tile>::const_iterator begin, vector<Tile>::const_iterator end) {
    vector<bool> result;
    while (begin != end) {
        result.push_back(bool(*begin++));
    }
    return result;
}

vector<bool>
Wall::get_placed() const {
    return vec_tile_to_bool(placed.begin(), placed.end());
}

const vector<Tile>&
Wall::get_tiles() const {
    return placed;
}

vector<vector<bool>>
Wall::get_placed_array() const {
    vector<vector<bool>> result;
    vector<Tile>::const_iterator slice_start = placed.begin();
    vector<Tile>::const_iterator slice_end;
    for (int i = 1; i <= rules->tile_types; i++) {
        slice_end = slice_start + rules->tile_types;
        result.push_back(vec_tile_to_bool(slice_start, slice_end));
        slice_start = slice_end;
    }
    return result;
}

vector<vector<Tile>>
Wall::get_tiles_array() const {
    vector<vector<Tile>> result;
    vector<Tile>::const_iterator slice_start = placed.begin();
    vector<Tile>::const_iterator slice_end;
    for (int i = 1; i <= rules->tile_types; i++) {
        slice_end = slice_start + rules->tile_types;
        result.push_back(vector<Tile>(slice_start, slice_end));
        slice_start = slice_end;
    }
    return result;
}


bool
Wall::line_has_color(ushort line, Tile color) const {
    // assert_line(line);
    // vector<Tile>::const_iterator slice_start = placed.begin() + (line - 1) * rules->tile_types;
    // vector<Tile>::const_iterator slice_end = slice_start + rules->tile_types;
    // return std::find(slice_start, slice_end, color) != slice_end;
    return get_tile_at(line_color_x(line, color), line) != Tile::NONE;
}

ushort
Wall::line_color_x(ushort line, Tile color) const {
    assert_line(line);
    return (line - 1 + ushort(color)) % rules->tile_types + 1;
}

ushort
Wall::completed_column_count() const {
    int count = 0;
    for (int column = 1; column <= rules->tile_types; column++) {
        int y;
        for (y = 1; y <= rules->tile_types; y++) {
            if (!get_tile_at_unsafe(column, y)) {
                break;
            }
        }
        if (y > rules->tile_types) {
            count++;
        }
    }
    return count;
}

ushort
Wall::completed_line_count() const {
    int count = 0;
    for (int line = 1; line <= rules->tile_types; line++) {
        int x;
        for (x = 1; x <= rules->tile_types; x++) {
            if (!get_tile_at_unsafe(x, line)) {
                break;
            }
        }
        if (x > rules->tile_types) {
            count++;
        }
    }
    return count;
}

ushort
Wall::completed_type_count() const {
    int count = 0;
    for (int type = 0; type < rules->tile_types; type++) {
        Tile tile = Tile(type);
        int line;
        for (line = 1; line <= rules->tile_types; line++) {
            if (!line_has_color(line, tile)) {
                break;
            }
        }
        if (line > rules->tile_types) {
            count++;
        }
    }
    return count;
}

ushort
Wall::final_score_bonus() const {
    return rules->column_bonus * completed_column_count() +
           rules->line_bonus * completed_line_count() +
           rules->type_bonus * completed_type_count();
}


// Scoring

ushort
Wall::score_for_placing(ushort x, ushort y) {
    ushort width = 1;
    ushort height = 1;
    // left
    for (int left = x - 1; left > 0; left--) {
        if (get_tile_at_unsafe(left, y)) {
            width++;
        }
    }
    // right
    for (int right = x + 1; right <= rules->tile_types; right++) {
        if (get_tile_at_unsafe(right, y)) {
            width++;
        }
    }
    // up
    for (int up = y - 1; up > 0; up--) {
        if (get_tile_at_unsafe(x, up)) {
            height++;
        }
    }
    // down
    for (int down = y + 1; down <= rules->tile_types; down++) {
        if (get_tile_at_unsafe(x, down)) {
            height++;
        }
    }
    if (width != 1 && height != 1) {
        return width + height;
    }
    return width + height - 1;
}

ushort
Wall::place_at(ushort x, ushort y) {
    return set_tile_at(x, y, color_at(x, y));
}

ushort
Wall::set_tile_at(ushort x, ushort y, Tile tile) {
    assert_line(y);
    assert_column(x);
    set_tile_at_unsafe(x, y, tile);
    return score_for_placing(x, y);
}

ushort
Wall::place_line_color(ushort line, Tile color) {
    // Can call unsafe because of 'line_color_y'
    ushort x = line_color_x(line, color);
    set_tile_at_unsafe(x, line, color);
    return score_for_placing(x, line);
}


// Reading

void
Wall::stream_line(ostream& os, const ushort line, bool brackets) const {
    if (brackets) {
        os << "[";
    }
    for (int i = 1; i <= rules->tile_types; i++) {
        Tile color = color_at(i, line);
        if (get_placed_at(i, line)) {
            os << color.letter();
        } else {
            os << color.lc_letter();
        }
    }
    if (brackets) {
        os << "]";
    }
}

ostream&
operator<<(ostream& os, const Wall& wall) {
    for (int line = 1; line <= wall.rules->tile_types; line++) {
        os << (line == 1 ? '[' : ' ');
        wall.stream_line(os, line, true);
        os << (line == wall.rules->tile_types ? ']' : '\n');
    }
    return os;
}

string
Wall::str() const {
    ostringstream os;
    os << *this;
    return os.str();
}

string
Wall::repr() const {
    ostringstream os;
    os << "[Wall:";
    for (int line = 1; line <= rules->tile_types; line++) {
        stream_line(os, line, false);
        os << (line == rules->tile_types ? ']' : '|');
    }
    return os.str();
}