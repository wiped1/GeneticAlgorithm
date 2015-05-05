#pragma once

#include <set>
#include "Genotype.hpp"
#include "Population.hpp"
#include "Evaluator.hpp"

namespace gall {

template <typename T>
class Ranking {
private:

public:
    struct FitnessGenotypeComparator {
        bool operator() (const std::pair<Genotype<T>*, double>& lhs,
                const std::pair<Genotype<T>*, double>& rhs) {
            return lhs.second > rhs.second;
        }
    };

    using CollectionType = std::multiset<std::pair<Genotype<T>*, double>, FitnessGenotypeComparator>;

    /*
     * Returns std::set containing reference to Genotype<T> and it's fitness after evaluation
     */
    CollectionType rank(Population<T>& pop, const Evaluator<T>& evaluator);
};

template <typename T>
typename Ranking<T>::CollectionType Ranking<T>::rank(Population<T> &pop, const Evaluator<T> &evaluator) {
    typename Ranking<T>::CollectionType result;
    std::for_each(pop.begin(), pop.end(), [&](Genotype<T>& genotype) {
        double fitness = evaluator.evaluate(genotype);
        result.emplace(&genotype, fitness);
    });
    return result;
}

}
