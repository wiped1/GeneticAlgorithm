#pragma once

#include "Population.hpp"
#include "Evaluator.hpp"
#include "Ranking.hpp"
#include <algorithm>
#include <iostream>

template <typename T>
class SelectionStrategy {
public:
    virtual ~SelectionStrategy() = default;
    virtual void eliminate(Population<T> &population, typename Ranking<T>::Type ranking) = 0;
};

