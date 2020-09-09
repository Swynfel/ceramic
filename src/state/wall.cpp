#include "wall.hpp"

#include <sstream>

Wall::Wall(const ushort size)
  : size(size)
  , placed() {
    clear();
}

Wall::Wall(const std::shared_ptr<Rules>& rule)
  : Wall(rule->tile_types) {}

Wall::Wall(const Wall& wall)
  : size(wall.size)
  , placed(wall.placed) {}

void
Wall::assert_line(const ushort line) const {
    if (line == 0) {
        throw std::range_error("No line '0', as it designates the floor.");
    }
    if (line > size) {
        throw std::range_error("No line '" + to_string(line) + "', as there is only '" + to_string(size) + "' lines.");
    }
}

void
Wall::assert_column(const ushort column) const {
    if (column == 0 || column > size) {
        throw std::range_error("No column '" + to_string(column) + "', choose 'column' such as 0 < 'column' <= " + to_string(size) + ".");
    }
}

// Methods

void
Wall::clear() {
    placed.assign(size * size, false);
}

bool
Wall::is_placed_at(ushort x, ushort y) const {
    assert_line(y);
    assert_column(x);
    return placed[x - 1 + (y - 1) * size];
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
    for (int i = 1; i <= size; i++) {
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
    for (int i = 1; i <= size; i++) {
        os << (is_placed_at(i, line) ? 'x' : ' ');
    }
    if (brackets) {
        os << "]";
    }
}

ostream&
operator<<(ostream& os, const Wall& wall) {
    for (int line = 1; line <= wall.size; line++) {
        os << (line == 1 ? '[' : ' ');
        wall.stream_line(os, line, true);
        os << (line == wall.size ? ']' : '\n');
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
    for (int line = 1; line <= size; line++) {
        stream_line(os, line, false);
        os << (line == size ? ']' : '|');
    }
    return os.str();
}