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
    virtual void eliminate(Population<T> &population, typename Ranking<T>::Type ranking);
};

template <typename T>
void DefaultSelectionStrategy<T>::eliminate(Population<T> &population, typename Ranking<T>::Type ranking) {
    auto& genotypes = population.getGenotypes();
    // TODO rename rankingIt?
    auto rankingIt = ranking.begin();
    // ranking is sorted high to low, std::advance moves iterator to the middle
    // so the half with lesser fitness is removed
    std::advance(rankingIt, ranking.size() / 2);
    // TODO move to separate private function
    for (rankingIt; rankingIt != ranking.end(); rankingIt++) {
        // iterate through vector and delete elements with the same address
        auto genotypesIt = genotypes.begin();
        // TODO rename genotypesIt?
        for (genotypesIt; genotypesIt < genotypes.end(); genotypesIt++) {
            // compare addresses of elements pointed to by iterators
            if (&(*genotypesIt) == (*rankingIt).first) {
                genotypes.erase(genotypesIt);
                break;
            }
        }
    }
}
