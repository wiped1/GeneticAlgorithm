#pragma once

#include "Population.hpp"
#include "Ranking.hpp"
#include "SelectionStrategy.hpp"

template <typename T>
class DefaultSelectionStrategy : public SelectionStrategy<T> {
public:
    /*
        Standard implementation of eliminate function eliminates lesser half
        of the population.
     */
    virtual void eliminate(Population<T> &population, typename Ranking<T>::CollectionType ranking);
};

template <typename T>
void DefaultSelectionStrategy<T>::eliminate(Population<T> &population, typename Ranking<T>::CollectionType ranking) {
    auto& genotypes = population.getGenotypes();
    std::vector<Genotype<T>> afterElimination;

    // iterate through half with higher values
    // and populate new vector
    auto it = ranking.begin();
    for (typename std::vector<Genotype<T>>::size_type i = 0; i < genotypes.size() / 2; i++) {
        afterElimination.push_back(std::move(*(*it).first));
        it++;
    }

    genotypes = std::move(afterElimination);
}
