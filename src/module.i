#if defined(SWIGPYTHON_BUILTIN) /* defined when using -builtin */
%module(moduleimport="from $module import *") azul
#else
%module(moduleimport="import $module") azul
#endif

%include <std_string.i>
%include <std_vector.i>
%include <std_array.i>
%include "utils.swg"

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
  %INT_AUTO
  %STR_AUTO
  %EQ_AUTO(Tile)
}

%extend Tiles {
  %GETITEM
  ushort __getitem__(Tile t) {
    return (*($self))[t];
  }

  %SETITEM
  void __setitem__(Tile t, ushort value) {
    (*($self))[t] = value;
  }

  %EQ_AUTO(Tiles)
  %STR_AUTO
  %LEN_AUTO
}
