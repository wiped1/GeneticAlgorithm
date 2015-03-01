#pragma once

#include "Population.hpp"

template <typename T>
class CrossoverStrategy {
public:
    virtual ~CrossoverStrategy() = default;
    virtual void cross(const Population<T> &population) = 0;
};