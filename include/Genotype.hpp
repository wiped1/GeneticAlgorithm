#pragma once

#include <vector>
#include "GenotypeInitializer.hpp"

template <typename T>
class Genotype {
private:
    std::vector<T> _genes;

public:
    Genotype(const GenotypeInitializer<T> &initializer);
    Genotype(const std::vector<T>& _genes);
    const std::vector<T>& getGenes() const;
};

template <typename T>
Genotype<T>::Genotype(const GenotypeInitializer<T> &initializer) : _genes() {
    initializer.initialize(_genes);
}

template <typename T>
Genotype<T>::Genotype(const std::vector<T>& genes) : _genes(genes) {
    // do nothing
}

template <typename T>
const std::vector<T>& Genotype<T>::getGenes() const {
    return _genes;
}
