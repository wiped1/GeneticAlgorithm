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
    virtual void eliminate(Population<T> &pop, typename Ranking<T>::CollectionType ranking);
};

template <typename T>
void DefaultSelectionStrategy<T>::eliminate(Population<T> &pop, typename Ranking<T>::CollectionType ranking) {
    std::vector<Genotype<T>> newPopulation;
    long range = std::distance(pop.cbegin(), pop.cend()) / 2;
    auto begin = ranking.begin();
    auto end = std::next(begin, range);
    /* iterate through ranking first (highest) half,
       moving genotypes to auxiliary container */
    std::for_each(begin, end, [&](auto& pair) {
        newPopulation.emplace_back(std::move(*pair.first));
    });
    pop = Population<T>(newPopulation);
}
