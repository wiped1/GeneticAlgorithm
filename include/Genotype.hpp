#pragma once

#include <vector>
#include <functional>
#include <algorithm>
#include "GenotypeInitializer.hpp"

namespace gall {

template <typename CollectionType>
class Genotype {
private:
    /* TODO rename CollectionType template parameter to something in lines of C */
    CollectionType genes;

public:
    /* TODO change alias type to CollectionType */
    using Collection = CollectionType;
    Genotype(const GenotypeInitializer<Genotype<CollectionType>>& genotypeInitializer);
    Genotype(CollectionType genes);
    bool operator==(const Genotype<CollectionType>& other);
    bool operator<(const Genotype<CollectionType>& other) const;
    typename CollectionType::iterator insert(typename CollectionType::iterator pos, const typename CollectionType::value_type& value);
    void forEach(const std::function<void(typename CollectionType::value_type&)>&);
    void reverseForEach(const std::function<void(typename CollectionType::value_type&)>&);
    typename CollectionType::iterator begin();
    typename CollectionType::iterator rbegin();
    typename CollectionType::const_iterator cbegin() const;
    typename CollectionType::iterator end();
    typename CollectionType::iterator rend();
    typename CollectionType::const_iterator cend() const;
};

template <typename CollectionType>
Genotype<CollectionType>::Genotype(const GenotypeInitializer<Genotype<CollectionType>>& genotypeInitializer) :
        genes() {
    genotypeInitializer.initialize(genes);
}

template <typename CollectionType>
Genotype<CollectionType>::Genotype(CollectionType genes) : genes(std::move(genes)) {
    // do nothing
}

template <typename CollectionType>
bool Genotype<CollectionType>::operator==(const Genotype<CollectionType>& other) {
    return genes == other.genes;
}

template <typename CollectionType>
bool Genotype<CollectionType>::operator<(const Genotype<CollectionType>& other) const {
    return std::lexicographical_compare(genes.cbegin(), genes.cend(),
                                        other.cbegin(), other.cend());
}


template <typename CollectionType>
typename CollectionType::iterator Genotype<CollectionType>::insert(typename Collection::iterator pos, const typename CollectionType::value_type& value) {
    return genes.insert(pos, value);
}

template <typename CollectionType>
void Genotype<CollectionType>::forEach(const std::function<void(typename CollectionType::value_type&)>& callback) {
    std::for_each(genes.begin(), genes.end(), [&](typename CollectionType::value_type& gene) {
        callback(gene);
    });
}

template <typename CollectionType>
void Genotype<CollectionType>::reverseForEach(const std::function<void(typename CollectionType::value_type&)>& callback) {
    std::for_each(genes.end(), genes.begin(), [&](typename CollectionType::value_type& gene) {
        callback(gene);
    });
}

template <typename CollectionType>
typename CollectionType::iterator Genotype<CollectionType>::begin() {
    return genes.begin();
}

template <typename CollectionType>
typename CollectionType::iterator Genotype<CollectionType>::rbegin() {
    return genes.rbegin();
}

template <typename CollectionType>
typename CollectionType::const_iterator Genotype<CollectionType>::cbegin() const {
    return genes.cbegin();
}

template <typename CollectionType>
typename CollectionType::iterator Genotype<CollectionType>::end() {
    return genes.end();
}

template <typename CollectionType>
typename CollectionType::iterator Genotype<CollectionType>::rend() {
    return genes.rend();
}

template <typename CollectionType>
typename CollectionType::const_iterator Genotype<CollectionType>::cend() const {
    return genes.cend();
}

}
