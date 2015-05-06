#pragma once

#include <vector>
#include "Genotype.hpp"
#include "GenotypeInitializer.hpp"
#include "Population.hpp"

namespace gall {

template <typename T>
class Population;

template <typename T>
class PopulationInitializer {
private:
    unsigned int _size;
    const GenotypeInitializer<T>& initializer;
public:
    PopulationInitializer(const GenotypeInitializer<T>& initializer, unsigned int size);
    void initialize(typename Population<T>::CollectionType& genotypes) const;
};

template <typename T>
PopulationInitializer<T>::PopulationInitializer(const GenotypeInitializer<T>& initializer,
        unsigned int size) : initializer(initializer), _size(size) {
    // do nothing
}

template <typename T>
void PopulationInitializer<T>::initialize(typename Population<T>::CollectionType& genotypes) const {
    for (unsigned int i = 0; i < _size; i++) {
        genotypes.emplace(initializer, 0);
    }
}

}
