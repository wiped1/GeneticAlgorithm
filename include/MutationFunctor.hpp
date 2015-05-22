#pragma once

#include <stdexcept>

template <typename T>
struct MutationFunctor {
    virtual void operator()(T&) const = 0;
    virtual ~MutationFunctor() { }
};
