#pragma once

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
    using CollectionType = std::set<ValueType, GenotypeFitnessPairComparator<Genotype>>;

/* private data members */
private:
    CollectionType genotypes;

/* public members */
public:
    Population() = delete;
    Population(const PopulationInitializer<Genotype>& populationInitializer,
               const Evaluator<Genotype>& evaluator);
    Population(CollectionType genotypes);
    void erase(typename Population<Genotype>::CollectionType::iterator pos);
    void erase(typename Population<Genotype>::CollectionType::iterator begin,
               typename Population<Genotype>::CollectionType::iterator end);
    /* TODO zamienić na applyInDescending, applyInAscendingOrder czy coś w tym stylu */
    typename CollectionType::iterator begin();
    typename CollectionType::const_iterator cbegin() const;
    typename CollectionType::iterator end();
    typename CollectionType::const_iterator cend() const;
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
