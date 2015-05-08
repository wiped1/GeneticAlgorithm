#pragma once

#include <iostream>
#include <functional>
#include "PopulationInitializer.hpp"
#include "Evaluator.hpp"

namespace gall {

template <typename Genotype>
class PopulationInitializer;

template <typename Genotype>
struct GenotypeFitnessPairComparator {
    bool operator()(const std::pair<Genotype, double>& lhs,
            const std::pair<Genotype, double>& rhs) {
        /* two different genotypes with same fitness can exist,
         * that's why we're comparing with standard less than fitness comapartor
         * but also check if when fitness is the same, genotypes are different */
        return lhs.second < rhs.second ||
                (lhs.second == rhs.second && lhs.first < rhs.first);
    }
};

template <typename Genotype>
class Population {
/* aliases */
public:
    using ValueType = std::pair<Genotype, double>;
    /* one could consider chosing std::map instead of std::set
     * but std::map is sorted using <Key, T> and doesn't allow storing Keys with same value.
     * Genotypes have to be sorted by fitness, and fitness can be the same hence set of pairs */
    using CollectionType = std::set<ValueType, GenotypeFitnessPairComparator<Genotype>>;

/* private data members */
private:
    CollectionType genotypes;

/* public members */
public:
    using iterator = int;
    Population() = delete;
    Population(const PopulationInitializer<Genotype>& populationInitializer,
               const Evaluator<Genotype>& evaluator);
    Population(CollectionType genotypes);
    void forEach(const std::function<void(const ValueType&)>& callback);
    void reverseForEach(const std::function<void(const ValueType&)>& callback);
    /* TODO curious recurring template pattern jako interface do wydobycia prywatnych iteratorów */
    typename CollectionType::iterator begin();
    typename CollectionType::const_iterator cbegin() const;
    typename CollectionType::iterator end();
    typename CollectionType::const_iterator cend() const;
    void erase(typename Population<Genotype>::CollectionType::iterator pos);
    void erase(typename Population<Genotype>::CollectionType::iterator begin,
               typename Population<Genotype>::CollectionType::iterator end);
};

template <typename Genotype>
Population<Genotype>::Population(const PopulationInitializer<Genotype>& populationInitializer,
                          const Evaluator<Genotype>& evaluator) {
    populationInitializer.initialize(genotypes, evaluator);
}

template <typename Genotype>
Population<Genotype>::Population(CollectionType genotypes) : genotypes(std::move(genotypes)) {
    // do nothing
}

/*
 * Iterates through contents of genotypes std::set, calling callback function with
 * const parameters. Std::set returns const_iterators because changing underlying
 * values would cause set to invalidate
 *
 * Usage:
 *     pop.forEach([&](auto pair) {
 *         // display fitness
 *         std::cout << pair.second << std::endl;
 *     });
 */
template <typename Genotype>
void Population<Genotype>::forEach(const std::function<void(const ValueType&)>& callback) {
    #if 0
    /* leaving for future reference */
    /*for (auto it = genotypes.begin(); it != genotypes.end(); it++) {
        /* WHY THE FUCK I HAVE TO CONST CAST THIS BAD BOY */
        /* That's why the fuck i have to const cast this bad boy:
         * set returns const references when dereferencing the iterator
         * because std::set does not know anything about the underlying reference
         * meaning that it would invalidate the contract if the value would have changed */
        callback(const_cast<std::pair<Genotype, double>&>(*it));
    }
    #endif
    std::for_each(genotypes.begin(), genotypes.end(), [&](auto pair) {
        callback(pair);
    });
}

template <typename Genotype>
void Population<Genotype>::reverseForEach(const std::function<void(const ValueType&)>& callback) {
    std::for_each(genotypes.end(), genotypes.begin(), [&](auto pair) {
        callback(pair);
    });
}

template <typename Genotype>
typename Population<Genotype>::CollectionType::iterator Population<Genotype>::begin() {
    return genotypes.begin();
}

template <typename Genotype>
typename Population<Genotype>::CollectionType::const_iterator Population<Genotype>::cbegin() const {
    return genotypes.cbegin();
}

template <typename Genotype>
typename Population<Genotype>::CollectionType::iterator Population<Genotype>::end() {
    return genotypes.end();
}

template <typename Genotype>
typename Population<Genotype>::CollectionType::const_iterator Population<Genotype>::cend() const {
    return genotypes.cend();
}

template <typename Genotype>
void Population<Genotype>::erase(typename Population<Genotype>::CollectionType::iterator pos) {
    genotypes.erase(pos);
}

template <typename Genotype>
void Population<Genotype>::erase(typename Population<Genotype>::CollectionType::iterator begin,
                          typename Population<Genotype>::CollectionType::iterator end) {
    genotypes.erase(begin, end);
}

}
