#pragma once

#include "Genotype.hpp"
#include "PopulationInitializer.hpp"

template <typename T>
class Population {
private:
    std::vector<Genotype<T>> _genotypes;
    Population() = default;

public:
    Population(const PopulationInitializer<T>& initializer);
    Population(const std::vector<Genotype<T>>& genotypes);
    Population(std::vector<Genotype<T>>&& genotypes);
    std::vector<Genotype<T>>& getGenotypes();
};

template <typename T>
Population<T>::Population(const PopulationInitializer<T>& initializer) : _genotypes() {
    initializer.initialize(_genotypes);
}

template <typename T>
Population<T>::Population(const std::vector<Genotype<T>>& genotypes) : _genotypes(genotypes) {
    // do nothing
}

template <typename T>
Population<T>::Population(std::vector<Genotype<T>>&& genotypes) : _genotypes(genotypes) {
    // do nothing
}

template <typename T>
std::vector<Genotype<T>>& Population<T>::getGenotypes() {
    return _genotypes;
}
