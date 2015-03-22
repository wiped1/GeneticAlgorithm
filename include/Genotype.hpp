#pragma once

#include <vector>
#include "GenotypeInitializer.hpp"

template <typename T>
class Genotype {
private:
    std::vector<T> _genes;

public:
    Genotype(const GenotypeInitializer<T>& genotypeInitializer);
    Genotype(std::vector<T> genes);
    bool operator==(const Genotype<T>& other);
    std::vector<T>& getGenes();
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
std::vector<T>& Genotype<T>::getGenes() {
    return _genes;
}

