#pragma once

#include "Population.hpp"

class CrossoverStrategy {
public:
    virtual ~CrossoverStrategy() = default;
    template <typename T>
    virtual void cross(const Population<T> &population) = 0;
};