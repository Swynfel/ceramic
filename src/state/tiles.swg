%include "tiles.hpp"

%extend Tile {
  %INT_AUTO
  %STR_AUTO
  %EQ_AUTO(Tile)
  %COPY_AUTO(Tile)
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
  %COPY_AUTO(Tiles)
  %STR_AUTO
  %LEN_AUTO
}
