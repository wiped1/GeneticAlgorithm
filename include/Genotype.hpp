#pragma once

#include <vector>
#include "GenotypeInitializer.hpp"

template <typename T>
class Genotype {
private:
    using CollectionType = std::vector<T>;
    CollectionType _genes;

public:
    Genotype(const GenotypeInitializer<T>& genotypeInitializer);
    Genotype(CollectionType genes);
    bool operator==(const Genotype<T>& other);
    typename CollectionType::iterator begin();
    typename CollectionType::const_iterator cbegin();
    typename CollectionType::iterator end();
    typename CollectionType::const_iterator cend();
};

template <typename T>
Genotype<T>::Genotype(const GenotypeInitializer<T>& genotypeInitializer) : _genes() {
    genotypeInitializer.initialize(_genes);
}

template <typename T>
Genotype<T>::Genotype(std::vector<T> genes) : _genes(std::move(genes)) {
    // do nothing
}

template <typename T>
bool Genotype<T>::operator==(const Genotype<T>& other) {
    return _genes == other._genes;
}

template <typename T>
typename Genotype<T>::CollectionType::iterator Genotype<T>::begin() {
    return _genes.begin();
}

template <typename T>
typename Genotype<T>::CollectionType::const_iterator Genotype<T>::cbegin() {
    return _genes.cbegin();
}

template <typename T>
typename Genotype<T>::CollectionType::iterator Genotype<T>::end() {
    return _genes.end();
}

template <typename T>
typename Genotype<T>::CollectionType::const_iterator Genotype<T>::cend() {
    return _genes.cend();
}
