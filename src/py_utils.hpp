#ifndef PY_UTILS_H
#define PY_UTILS_H

#include "global.hpp"

template<class PyClass>
bool
py_eq(const PyClass& self, const PyClass& other) {
    return self == other;
}

template<class PyClass>
bool
py_ne(const PyClass& self, const PyClass& other) {
    return self != other;
}

template<class PyClass>
bool
py_le(const PyClass& self, const PyClass& other) {
    return self <= other;
}

template<class PyClass>
bool
py_lt(const PyClass& self, const PyClass& other) {
    return self < other;
}

template<class PyClass>
bool
py_ge(const PyClass& self, const PyClass& other) {
    return self >= other;
}

template<class PyClass>
bool
py_gt(const PyClass& self, const PyClass& other) {
    return self > other;
}

template<class PyClass>
PyClass
py_add(const PyClass& self, const PyClass& other) {
    return self + other;
}

template<class PyClass, class PyOther>
PyClass
py_add(const PyClass& self, const PyOther& other) {
    return self + other;
}

template<class PyClass>
PyClass
py_sub(const PyClass& self, const PyClass& other) {
    return self - other;
}

template<class PyClass, class PyOther>
PyClass
py_sub(const PyClass& self, const PyOther& other) {
    return self - other;
}

template<class PyClass>
PyClass&
py_radd(PyClass& self, const PyClass& other) {
    return self += other;
}

template<class PyClass, class PyOther>
PyClass&
py_radd(PyClass& self, const PyOther& other) {
    return self += other;
}

template<class PyClass>
PyClass&
py_rsub(PyClass& self, const PyClass& other) {
    return self -= other;
}

template<class PyClass, class PyOther>
PyClass&
py_rsub(PyClass& self, const PyOther& other) {
    return self -= other;
}


#endif //PY_UTILS_H
