#pragma once

#include <algorithm>
#include <iostream>
#include "Population.hpp"
#include "Evaluator.hpp"
#include "Ranking.hpp"

template <typename T>
class SelectionStrategy {
public:
    virtual ~SelectionStrategy() = default;
    virtual void eliminate(Population<T> &population, typename Ranking<T>::CollectionType ranking) = 0;
};

