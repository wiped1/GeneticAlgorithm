#pragma once

#include "Population.hpp"

namespace gall {

template <typename T>
class MutationStrategy {
public:
    virtual ~MutationStrategy() = default;
    virtual void mutate(Population<T>& population) const = 0;
};

}
