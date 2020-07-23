%include "exception.i"

// For python:slot, look at https://docs.python.org/3/c-api/typeobj.html

%{
#include <cassert>
#include <stdexcept>
%}

%define %RANGE_ERROR {
  try {
    $action
  } catch(std::out_of_range) {
    SWIG_exception(SWIG_IndexError, "Index out of bounds");
  }
}
%enddef

%define %INVALID_ARGUMENT_ERROR {
  try {
    $action
  } catch(const std::invalid_argument& e) {
    SWIG_exception(SWIG_ValueError, e.what());
  }
}
%enddef


%define %INT
%feature("python:slot", "nb_int", functype="unaryfunc") __int__;
%enddef

%define %INT_AUTO
%INT
int __int__() {
  return (int)(*($self));
}
%enddef

%define %LEN
%feature("python:slot", "sq_length", functype="lenfunc") __len__;
%enddef

%define %LEN_AUTO
%LEN
long __len__() const {
  return $self->size();
}
%enddef

%define %GETITEM
%feature("python:slot", "mp_subscript", functype="binaryfunc") __getitem__;
%enddef

%define %SETITEM
%feature("python:slot", "mp_ass_subscript", functype="objobjargproc") __setitem__;
%enddef

%define %STR
%feature("python:slot", "tp_str", functype="reprfunc") __str__;
%enddef

%define %STR_AUTO
%STR
string __str__() {
  return (*($self)).str();
}
%enddef

%define %EQ_AUTO(TYPE)
bool __eq__(const TYPE& other) {
  return (*($self)) == other;
}
%enddef

%define %COPY_AUTO(TYPE)
TYPE copy() const {
  return TYPE(*($self));
}
%enddef
