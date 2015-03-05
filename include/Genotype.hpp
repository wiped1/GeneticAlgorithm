#pragma once

#include <vector>
#include "GenotypeInitializer.hpp"

template <typename T>
class Genotype {
private:
    std::vector<T> _genes;

public:
    Genotype(GenotypeInitializer<T>& initializer);
    Genotype(const std::vector<T>& genes);
    Genotype(std::vector<T>&& genes);
    std::vector<T>& getGenes();
};

template <typename T>
Genotype<T>::Genotype(GenotypeInitializer<T>& initializer) : _genes() {
    initializer.initialize(_genes);
}

template <typename T>
Genotype<T>::Genotype(const std::vector<T>& genes) : _genes(genes) {
    // do nothing
}

template <typename T>
Genotype<T>::Genotype(std::vector<T>&& genes) : _genes(genes) {
    // do nothing
}

template <typename T>
std::vector<T>& Genotype<T>::getGenes() {
    return _genes;
}
