#pragma once

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
    long range = std::distance(pop.cbegin(), pop.cend()) / 2;
    auto begin = std::next(pop.begin(), range);
    auto end = pop.end();
    pop.erase(begin, end);
}

}
