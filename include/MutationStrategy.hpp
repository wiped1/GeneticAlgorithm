#pragma once

#include "Population.hpp"

template <typename T>
class MutationStrategy {
public:
    virtual ~MutationStrategy() = default;
    virtual void mutate(Population<T>& population) = 0;
};
