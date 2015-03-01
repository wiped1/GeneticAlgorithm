#pragma once

#include "Genotype.hpp"

template <typename T>
class Population {
private:

    std::vector<Genotype<T>> _genotypes;
    Population() = default;

public:

    Population(const std::vector<Genotype<T>> &genotypes);
    std::vector<Genotype<T>> &getGenotypes();

};

template <typename T>
Population<T>::Population(const std::vector<Genotype<T>> &genotypes) : _genotypes(genotypes) {
    // do nothing
}

template <typename T>
std::vector<Genotype<T>> &getGenotypes() {
    return _genotypes;
}