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
#include "state/factory.hpp"
#include "state/panel.hpp"
#include "state/pyramid.hpp"
#include "state/rules.hpp"
#include "state/state.hpp"
#include "state/tiles.hpp"
#include "state/wall.hpp"
%}

%template(UshortVector) vector<ushort>;
%template(UshortArray) array<ushort, TILE_TYPES>;

%include "state/factory.swg"
%include "state/panel.swg"
%include "state/pyramid.swg"
%include "state/rules.swg"
%include "state/state.swg"
%include "state/tiles.swg"
%include "state/wall.swg"
