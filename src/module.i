%module azul
%include <std_string.i>
%include <std_vector.i>
%include <std_array.i>

using namespace std;
typedef unsigned short ushort;

%{
#define SWIG_FILE_WITH_INIT
#include "global.hpp"
#include "tiles.hpp"
%}

%template(UshortVector) vector<ushort>;
%template(UshortArray) array<ushort, TILE_TYPES>;

%include "tiles.hpp"

%extend Tile {
  int __int__() {
    return (int)(*($self));
  }

  string __str__() {
    return (*($self)).str();
  }
}

%extend Tiles {
  ushort __getitem__(Tile t) {
    return (*($self))[t];
  }

  void __setitem__(Tile t, ushort value) {
    (*($self))[t] = value;
  }

  bool __eq__(const Tiles& other) {
    return (*($self)) == other;
  }

  string __str__() {
    return (*($self)).str();
  }
}
