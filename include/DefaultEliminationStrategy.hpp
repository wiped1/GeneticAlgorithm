#pragma once

#include <cmath>
#include "Population.hpp"
#include "EliminationStrategy.hpp"

namespace gall {

template <typename T>
class DefaultEliminationStrategy : public EliminationStrategy<T> {
public:
    /*
        Standard implementation of eliminate function eliminates lesser half
        of the population.
     */
    virtual void eliminate(Population<T> &pop);
};

template <typename T>
void DefaultEliminationStrategy<T>::eliminate(Population<T> &pop) {
    long range = static_cast<long>(std::ceil(pop.asCollection().size() / 2.0)); /* 2.0 as floating point to prevent cast */
    auto begin = pop.asCollection().begin();
    auto end = std::next(begin, range);
    pop.asCollection().erase(begin, end);
}

}
