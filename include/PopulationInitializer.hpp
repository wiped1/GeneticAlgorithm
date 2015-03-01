#pragma once

#include <vector>
#include "Genotype.hpp"
#include "GenotypeInitializer.hpp"

template <typename T>
class PopulationInitializer {

unsigned int _size;

public:

    PopulationInitializer(unsigned int size);

    void initialize(const GenotypeInitializer<T> &initializer, const std::vector<Genotype<T>> &genotypes) const;


};

PopulationInitializer::PopulationInitializer(unsigned int size) : _size(size) {

}

template <typename T>
void PopulationInitializer::initialize(const GenotypeInitializer<T> &initializer, const std::vector<Genotype<T>> &genotypes) {
    for (unsigned int i = 0; i < _size; i++) {
        genotypes.emplace_back(initializer);
    }
}