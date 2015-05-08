#pragma once

#include <cmath>
#include "Population.hpp"
#include "Ranking.hpp"
#include "SelectionStrategy.hpp"

namespace gall {

template <typename T>
class DefaultSelectionStrategy : public SelectionStrategy<T> {
public:
    /*
        Standard implementation of eliminate function eliminates lesser half
        of the population.
     */
    virtual void eliminate(Population<T> &pop);
};

template <typename T>
void DefaultSelectionStrategy<T>::eliminate(Population<T> &pop) {
    std::vector<Genotype<T>> newPopulation;
    long range = static_cast<long>(std::ceil(std::distance(pop.cbegin(), pop.cend()) / 2.0)); /* 2.0 as floating point to prevent cast */
    auto begin = pop.begin();
    auto end = std::next(begin, range);
    pop.erase(begin, end);
}

}
