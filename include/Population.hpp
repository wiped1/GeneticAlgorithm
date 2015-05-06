#pragma once

#include <functional>
#include "Genotype.hpp"
#include "PopulationInitializer.hpp"

namespace gall {

template <typename T>
class PopulationInitializer;

template <typename T>
struct GenotypeFitnessPairComparator {
    bool operator()(const std::pair<Genotype<T>, double>& lhs,
            const std::pair<Genotype<T>, double>& rhs) {
        /* TODO zamienić negację na komparator który będzie negował komparację zadanego komparatora */
//        return !(lhs.second < rhs.second) ||
//                (lhs.second == rhs.second && lhs.first < rhs.first);
        return lhs.second > rhs.second;
    }
};

template <typename T>
class Population {
private:

    /* private alias in order to declare `genotypes` member */
    using _GenotypeType = std::pair<Genotype<T>, double>;
    using _CollectionType = std::set<_GenotypeType, GenotypeFitnessPairComparator<T>>;
    _CollectionType genotypes;

public:
    /* public alias for other classes to use */
    using GenotypeType = _GenotypeType;
    using CollectionType = _CollectionType;
    Population() = delete;
    Population(const PopulationInitializer<T>& populationInitializer);
    Population(CollectionType genotypes);
    void erase(typename Population<T>::CollectionType::iterator pos);
    void erase(typename Population<T>::CollectionType::iterator begin,
               typename Population<T>::CollectionType::iterator end);
    typename CollectionType::iterator begin();
    typename CollectionType::const_iterator cbegin() const;
    typename CollectionType::iterator end();
    typename CollectionType::const_iterator cend() const;
};

template <typename T>
Population<T>::Population(const PopulationInitializer<T>& populationInitializer) : genotypes() {
    populationInitializer.initialize(genotypes);
}

template <typename T>
Population<T>::Population(CollectionType genotypes) : genotypes(std::move(genotypes)) {
    // do nothing
}

template <typename T>
typename Population<T>::CollectionType::iterator Population<T>::begin() {
    return genotypes.begin();
}

template <typename T>
typename Population<T>::CollectionType::const_iterator Population<T>::cbegin() const {
    return genotypes.cbegin();
}

template <typename T>
typename Population<T>::CollectionType::iterator Population<T>::end() {
    return genotypes.end();
}

template <typename T>
typename Population<T>::CollectionType::const_iterator Population<T>::cend() const {
    return genotypes.cend();
}

template <typename T>
void Population<T>::erase(typename Population<T>::CollectionType::iterator pos) {
    genotypes.erase(pos);
}

template <typename T>
void Population<T>::erase(typename Population<T>::CollectionType::iterator begin,
                          typename Population<T>::CollectionType::iterator end) {
    genotypes.erase(begin, end);
}

}
