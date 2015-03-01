#pragma once

#include "Population.hpp"
#include "Evaluator.hpp"
#include <algorithm>
#include <iostream>

template <typename T>
class SelectionStrategy {
private:
    const Evaluator<T> &_evaluator;
public:
    SelectionStrategy(const Evaluator<T> &evaluator);
    virtual ~SelectionStrategy() = default;
    /*
        Standard implementation of eliminate function eliminates lesser half
        of the population.
     */
    virtual void eliminate(Population<T> &population);
};

template <typename T>
SelectionStrategy<T>::SelectionStrategy(const Evaluator<T> &evaluator) : _evaluator(evaluator) {
    // do nothing
}

template <typename T>
void SelectionStrategy<T>::eliminate(Population<T> &population) {
    auto& genotypes = population.getGenotypes();

    // TODO move to another class?
    std::sort(genotypes.begin(), genotypes.end(), [this](Genotype<T> first, Genotype<T> second) {
        double firstScore = _evaluator.evaluate(first);
        double secondScore = _evaluator.evaluate(second);

        return firstScore > secondScore;
    });

    genotypes.erase(genotypes.begin() + genotypes.size() / 2, genotypes.end());
}
