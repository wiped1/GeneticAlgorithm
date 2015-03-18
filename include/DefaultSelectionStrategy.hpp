#pragma once

#include "SelectionStrategy.hpp"

template <typename T>
class DefaultSelectionStrategy : public SelectionStrategy<T> {
public:
    /*
        Standard implementation of eliminate function eliminates lesser half
        of the population.
     */
    virtual void eliminate(Population<T> &population, const Evaluator<T> &evaluator);
};

template <typename T>
void DefaultSelectionStrategy<T>::eliminate(Population<T> &population, const Evaluator<T>& evaluator) {
    auto& genotypes = population.getGenotypes();

    std::sort(genotypes.begin(), genotypes.end(), [&](Genotype<T> first, Genotype<T> second) {
        double firstScore = evaluator.evaluate(first);
        double secondScore = evaluator.evaluate(second);

        return firstScore > secondScore;
    });

    // in case of even number of elements it's result is rounded down
    genotypes.erase(genotypes.begin() + genotypes.size() / 2, genotypes.end());
}
