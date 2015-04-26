#pragma once

#include <set>
#include "Genotype.hpp"
#include "Population.hpp"
#include "Evaluator.hpp"

template <typename T>
class Ranking {
private:

public:
    struct FitnessGenotypeComparator {
        bool operator() (const std::pair<Genotype<T>*, double>& lhs,
                const std::pair<Genotype<T>*, double>& rhs) {
            // consider equal values as greater, as it poses no difference in
            // GA execution, but allows SelectionStrategy to properly delete elements
            return lhs.second >= rhs.second;
        }
    };

    typedef std::set<std::pair<Genotype<T>*, double>, FitnessGenotypeComparator> CollectionType;

    /*
     * Returns std::set containing reference to Genotype<T> and it's fitness after evaluation
     */
    CollectionType rank(Population<T>& pop, const Evaluator<T>& evaluator);
};

template <typename T>
typename Ranking<T>::CollectionType Ranking<T>::rank(Population<T> &pop, const Evaluator<T> &evaluator) {
    typename Ranking<T>::CollectionType result;

    for (Genotype<T>& genotype : pop.getGenotypes()) {
        double fitness = evaluator.evaluate(genotype);
        result.emplace(&genotype, fitness);
    }

    return result;
}
