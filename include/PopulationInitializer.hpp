#pragma once

#include <vector>
#include "Genotype.hpp"
#include "GenotypeInitializer.hpp"

template <typename T>
class PopulationInitializer {
private:
    unsigned int _size;
    const GenotypeInitializer<T> &_initializer;
public:
    PopulationInitializer(const GenotypeInitializer<T> &initializer, unsigned int size);
    void initialize(std::vector<Genotype<T>> &genotypes) const;
};

template <typename T>
PopulationInitializer<T>::PopulationInitializer(const GenotypeInitializer<T> &initializer,
        unsigned int size) : _initializer(initializer), _size(size) {
    // do nothing
}

template <typename T>
void PopulationInitializer<T>::initialize(std::vector<Genotype<T>> &genotypes) const {
    for (unsigned int i = 0; i < _size; i++) {
        genotypes.emplace_back(_initializer);
    }
}
