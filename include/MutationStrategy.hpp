#pragma once

#include "Population.hpp"

class MutationStrategy {
public:
    virtual ~MutationStrategy() = default;
    template <typename T>
    virtual void mutate(const Population<T> &population) = 0;
};