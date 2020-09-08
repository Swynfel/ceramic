#include "pyramid.hpp"

#include <sstream>

Pyramid::Pyramid(ushort size)
  : size(size)
  , tile_types()
  , tile_filled() {
    clear();
}

Pyramid::Pyramid(const Rules& rule)
  : Pyramid(rule.tile_types) {}

Pyramid::Pyramid(const Pyramid& pyramid)
  : size(pyramid.size)
  , tile_types(pyramid.tile_types)
  , tile_filled(pyramid.tile_filled) {}

void
Pyramid::clear() {
    tile_types.assign(size, Tile::NONE);
    tile_filled.assign(size, 0);
}

void
Pyramid::clear_line(ushort line) {
    tile_types[line] = Tile::NONE;
    tile_filled[line] = 0;
}

void
Pyramid::set_line(ushort line, ushort amount, Tile color) {
    Tile previous_color = tile_types[line];
    if (previous_color != Tile::NONE && color != previous_color) {
        throw std::invalid_argument(
            "Placing wrong tile color '" + color.str() + "' on pyramid line already holding tiles of color '" + previous_color.str() + "'");
    }
    tile_types[line] = color;
    tile_filled[line] = amount;
}

bool
Pyramid::is_filled(ushort line) const {
    return amount(line) == line + 1;
}

bool
Pyramid::is_empty(ushort line) const {
    return amount(line) == 0;
}

ushort
Pyramid::amount(ushort line) const {
    return tile_filled[line];
}

ushort
Pyramid::amount_remaining(ushort line) const {
    return line + 1 - tile_filled[line];
}

Tile
Pyramid::color(ushort line) const {
    return tile_types[line];
}

vector<bool>
Pyramid::filled() const {
    vector<bool> result;
    for (ushort i = 0; i < size; i++) {
        result.push_back(is_filled(i));
    }
    return result;
}

// Reading

void
Pyramid::stream_line(ostream& os, const ushort line, bool brackets) const {
    int a = amount(line);
    string c = to_string(int(color(line)));
    if (brackets) {
        os << "[";
    }
    for (int i = 0; i < line; i++) {
        os << (i < a ? c : " ");
    }
    if (brackets) {
        os << "]";
    }
}

ostream&
operator<<(ostream& os, const Pyramid& pyramid) {
    for (int line = 0; line < pyramid.size; line++) {
        os << (line == 0 ? '[' : ' ');
        pyramid.stream_line(os, line, true);
        os << (line == pyramid.size - 1 ? ']' : '\n');
    }
    return os;
}

string
Pyramid::str() const {
    ostringstream os;
    os << *this;
    return os.str();
}

string
Pyramid::repr() const {
    ostringstream os;
    os << "[Pyramid:";
    for (int line = 0; line < size; line++) {
        stream_line(os, line, false);
        os << (line == size - 1 ? ']' : '|');
    }
    return os.str();
}