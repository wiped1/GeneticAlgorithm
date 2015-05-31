#pragma once

#include <iostream>
#include <utility>
#include <functional>
#include <set>
#include "PopulationInitializer.hpp"
#include "Evaluator.hpp"

namespace gall {

//template <typename Genotype>
//class PopulationInitializer;

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
    using CollectionType = std::multiset<ValueType, GenotypeFitnessPairComparator<Genotype>>;

/* private data members */
private:
    CollectionType genotypes;
    const Evaluator<Genotype>* evaluator = nullptr;

/* public members */
public:
    using iterator = int;
    Population() = delete;
    Population(const PopulationInitializer<Genotype>& populationInitializer,
               const Evaluator<Genotype>& evaluator);
    Population(CollectionType genotypes);
    CollectionType& asCollection();
    const CollectionType& asCollection() const;
    void insert(const Genotype& genitype);
    template <typename InputIt>
    void insert(InputIt first, InputIt last);
private:
    ValueType makePair(Genotype& genotype);
};

template <typename Genotype>
Population<Genotype>::Population(const PopulationInitializer<Genotype>& populationInitializer,
                          const Evaluator<Genotype>& evaluator) : evaluator(&evaluator) {
    populationInitializer.initialize(genotypes, evaluator);
}

template <typename Genotype>
Population<Genotype>::Population(CollectionType genotypes) : genotypes(std::move(genotypes)) {
    // do nothing
}

template <typename Genotype>
typename Population<Genotype>::CollectionType& Population<Genotype>::asCollection() {
    return genotypes;
}

template <typename Genotype>
const typename Population<Genotype>::CollectionType& Population<Genotype>::asCollection() const {
    return genotypes;
}

template <typename Genotype>
typename Population<Genotype>::ValueType
Population<Genotype>::makePair(Genotype& genotype) {
    if (evaluator == nullptr) {
        throw std::runtime_error("Population: Evaluator is not initialized.");
    }
    return ValueType{genotype, evaluator->evaluate(genotype)};
};

template <typename Genotype>
void Population<Genotype>::insert(const Genotype& genotype) {
    genotypes.insert(std::move(makePair(genotype)));
};

template <typename Genotype>
template <typename InputIt>
void Population<Genotype>::insert(InputIt first, InputIt last) {
    std::for_each(first, last, [&](auto genotype) {
        /* makePair invoked using this, because it's in lambda expression */
        genotypes.insert(std::move(this->makePair(genotype)));
    });
};
}
