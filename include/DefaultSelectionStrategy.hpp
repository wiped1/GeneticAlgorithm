#pragma once

#include "SelectionStrategy.hpp"
#include "Population.hpp"
#include "Ranking.hpp"

template <typename T>
class DefaultSelectionStrategy : public SelectionStrategy<T> {
public:
    /*
        Standard implementation of eliminate function eliminates lesser half
        of the population.
     */
    virtual void eliminate(Population<T> &population, typename Ranking<T>::Type ranking);
};

template <typename T>
void DefaultSelectionStrategy<T>::eliminate(Population<T> &population, typename Ranking<T>::Type ranking) {
    auto& genotypes = population.getGenotypes();
    auto it = ranking.begin();
    // ranking is sorted high to low, std::advance moves iterator to the middle
    // so the half with lesser fitness is removed
    std::advance(it, ranking.size() / 2);
    for (it; it != ranking.end(); it++) {
        genotypes.erase(std::remove(genotypes.begin(), genotypes.end(), (*it).first), genotypes.end());
    }
}
