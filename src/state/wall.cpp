#include "wall.hpp"

#include <sstream>

Wall::Wall(const ushort size)
  : size(size)
  , placed() {
    clear();
}

Wall::Wall(const Rules& rule)
  : Wall(rule.tile_types) {}

Wall::Wall(const Wall& wall)
  : size(wall.size)
  , placed(wall.placed) {}

void
Wall::clear() {
    placed.assign(size * size, false);
}

bool
Wall::is_placed_at(ushort x, ushort y) const {
    if (x >= size || y >= size) {
        throw std::range_error("x or y out of range");
    }
    return placed[x + y * size];
}

const vector<bool>
Wall::get_placed() const {
    return placed;
}

vector<vector<bool>>
Wall::get_placed_array() const {
    vector<vector<bool>> result;
    vector<bool>::const_iterator slice_start = placed.begin();
    vector<bool>::const_iterator slice_end;
    for (int i = 0; i < size; i++) {
        slice_end = slice_start + size;
        result.push_back(
            vector<bool>(slice_start, slice_end));
        slice_start = slice_end;
    }
    return result;
}

// Reading

void
Wall::stream_line(ostream& os, const ushort line, bool brackets) const {
    if (brackets) {
        os << "[";
    }
    for (int i = 0; i < size; i++) {
        os << is_placed_at(i, line) ? 'x' : ' ';
    }
    if (brackets) {
        os << "]";
    }
}

ostream&
operator<<(ostream& os, const Wall& wall) {
    int x = 0;
    for (int line = 0; line < wall.size; line++) {
        os << (line == 0 ? '[' : ' ');
        wall.stream_line(os, line, true);
        os << (line == wall.size - 1 ? ']' : '\n');
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
    for (int line = 0; line < size; line++) {
        stream_line(os, line, false);
        os << (line == size - 1 ? ']' : '|');
    }
    return os.str();
}