#pragma once

#include <functional>
#include "Genotype.hpp"
#include "PopulationInitializer.hpp"

namespace gall {

template <typename T>
class Population {
private:
    using CollectionType = std::vector<Genotype<T>>;
    CollectionType _genotypes;

public:
    Population() = delete;
    Population(const PopulationInitializer<T>& populationInitializer);
    Population(CollectionType genotypes);
    typename CollectionType::iterator begin();
    typename CollectionType::const_iterator cbegin() const;
    typename CollectionType::iterator end();
    typename CollectionType::const_iterator cend() const;
};

template <typename T>
Population<T>::Population(const PopulationInitializer<T>& populationInitializer) : _genotypes() {
    populationInitializer.initialize(_genotypes);
}

template <typename T>
Population<T>::Population(CollectionType genotypes) : _genotypes(std::move(genotypes)) {
    // do nothing
}

template <typename T>
typename Population<T>::CollectionType::iterator Population<T>::begin() {
    return _genotypes.begin();
}

template <typename T>
typename Population<T>::CollectionType::const_iterator Population<T>::cbegin() const {
    return _genotypes.cbegin();
}

template <typename T>
typename Population<T>::CollectionType::iterator Population<T>::end() {
    return _genotypes.end();
}

template <typename T>
typename Population<T>::CollectionType::const_iterator Population<T>::cend() const {
    return _genotypes.cend();
}

}
