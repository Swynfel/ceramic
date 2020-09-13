#include "pyramid.hpp"

#include <sstream>

Pyramid::Pyramid(ushort size)
  : size(size)
  , tile_types()
  , tile_filled() {
    clear();
}

Pyramid::Pyramid(const std::shared_ptr<Rules>& rule)
  : Pyramid(rule->tile_types) {}

Pyramid::Pyramid(const Pyramid& pyramid)
  : size(pyramid.size)
  , tile_types(pyramid.tile_types)
  , tile_filled(pyramid.tile_filled) {}

// Place before every direct acces to arrays tile_types and tile_filled
void
Pyramid::assert_line(ushort line) const {
    if (line == 0) {
        throw std::range_error("No line '0', as it designates the floor.");
    }
    if (line > size) {
        throw std::range_error("No line '" + to_string(line) + "', as there is only '" + to_string(size) + "' lines.");
    }
}

// Methods

void
Pyramid::clear() {
    tile_types.assign(size, Tile::NONE);
    tile_filled.assign(size, 0);
}


Tiles
Pyramid::get_line(ushort line) const {
    assert_line(line);
    if (tile_filled[line - 1] == 0) {
        return Tiles::ZERO;
    }
    return Tiles(tile_types[line - 1], tile_filled[line - 1]);
}

void
Pyramid::clear_line(ushort line) {
    assert_line(line);
    tile_types[line - 1] = Tile::NONE;
    tile_filled[line - 1] = 0;
}

void
Pyramid::set_line(ushort line, ushort amount, Tile color) {
    assert_line(line);
    if (amount > line) {
        throw std::invalid_argument("Can't set amount over " + to_string(line) + " for line with the same id.");
    }
    tile_types[line - 1] = color;
    tile_filled[line - 1] = amount;
}

bool
Pyramid::is_filled(ushort line) const {
    return amount(line) == line;
}

bool
Pyramid::is_empty(ushort line) const {
    return amount(line) == 0;
}

ushort
Pyramid::amount(ushort line) const {
    assert_line(line);
    return tile_filled[line - 1];
}

ushort
Pyramid::amount_remaining(ushort line) const {
    return line - amount(line);
}

Tile
Pyramid::color(ushort line) const {
    assert_line(line);
    return tile_types[line - 1];
}

bool
Pyramid::accept_color(ushort line, Tile tile) const {
    Tile current = color(line);
    return current == Tile::NONE || (current == tile && !is_filled(line));
}


vector<bool>
Pyramid::filled() const {
    vector<bool> result;
    for (ushort i = 1; i <= size; i++) {
        result.push_back(is_filled(i));
    }
    return result;
}

// Reading

void
Pyramid::stream_line(ostream& os, ushort line, bool brackets) const {
    int a = amount(line);
    string c = to_string(int(color(line)));
    if (brackets) {
        os << "[";
    }
    for (int i = 1; i <= line; i++) {
        os << (i <= a ? c : " ");
    }
    if (brackets) {
        os << "]";
    }
}

ostream&
operator<<(ostream& os, const Pyramid& pyramid) {
    for (int line = 1; line <= pyramid.size; line++) {
        os << (line == 1 ? '[' : ' ');
        pyramid.stream_line(os, line, true);
        os << (line == pyramid.size ? ']' : '\n');
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
    for (int line = 1; line <= size; line++) {
        stream_line(os, line, false);
        os << (line == size ? ']' : '|');
    }
    return os.str();
}