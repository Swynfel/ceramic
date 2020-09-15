#include "observer.hpp"

std::shared_ptr<Observer>
Observer::ptr() {
    return shared_from_this();
}