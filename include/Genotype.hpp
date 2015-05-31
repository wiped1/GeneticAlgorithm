#pragma once

#include <vector>
#include <functional>
#include <algorithm>
#include "GenotypeInitializer.hpp"

namespace gall {

template <typename C>
class Genotype {
private:
    C genes;

public:
    /* TODO change alias type to CollectionType */
    using CollectionType = C;
    Genotype(const GenotypeInitializer<Genotype<C>>& genotypeInitializer);
    Genotype(C genes);
    bool operator==(const Genotype<C>& other);
    bool operator<(const Genotype<C>& other) const;
    C& asCollection();
    const C& asCollection() const;
};

template <typename C>
Genotype<C>::Genotype(const GenotypeInitializer<Genotype<C>>& genotypeInitializer) :
        genes() {
    genotypeInitializer.initialize(genes);
}

template <typename C>
Genotype<C>::Genotype(C genes) : genes(std::move(genes)) {
    // do nothing
}

template <typename C>
bool Genotype<C>::operator==(const Genotype<C>& other) {
    return genes == other.genes;
}

template <typename C>
bool Genotype<C>::operator<(const Genotype<C>& other) const {
    return std::lexicographical_compare(genes.cbegin(), genes.cend(),
                                        other.asCollection().cbegin(), other.asCollection().cend());
}

template <typename C>
typename Genotype<C>::CollectionType& Genotype<C>::asCollection() {
    return genes;
}

template <typename C>
const typename Genotype<C>::CollectionType& Genotype<C>::asCollection() const {
    return genes;
}
}
