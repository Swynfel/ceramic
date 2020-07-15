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

%extend Tiles {
  ushort __getitem__(Tile t) {
    return (*($self))[t];
  }
}
