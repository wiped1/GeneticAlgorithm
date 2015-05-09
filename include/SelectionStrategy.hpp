#pragma once

#include <algorithm>
#include <iostream>
#include "Population.hpp"
#include "Evaluator.hpp"

namespace gall {

template <typename Genotype>
class SelectionStrategy {
public:
    virtual ~SelectionStrategy() = default;
    virtual void eliminate(Population<Genotype> &population) = 0;
};

}
