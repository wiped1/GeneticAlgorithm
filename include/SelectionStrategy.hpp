#pragma once

#include "Population.hpp"
#include "Evaluator.hpp"
#include <algorithm>
#include <iostream>

template <typename T>
class SelectionStrategy {
public:
    virtual ~SelectionStrategy() = default;
    virtual void eliminate(Population<T> &population, const Evaluator<T> &evaluator) = 0;
};

