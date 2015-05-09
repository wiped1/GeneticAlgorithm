#pragma once

#include <vector>
#include <algorithm>
#include "GenotypeInitializer.hpp"

namespace gall {

template <typename T,
template <typename, typename = std::allocator<T>> class CollectionType = std::vector>
class Genotype {
private:
    CollectionType<T> genes;

public:
    using Collection = CollectionType<T>;
    Genotype(const GenotypeInitializer<Genotype<T, CollectionType>>& genotypeInitializer);
    Genotype(CollectionType<T> genes);
    bool operator==(const Genotype<T, CollectionType>& other);
    bool operator<(const Genotype<T, CollectionType>& other) const;
    typename Collection::iterator insert(typename Collection::iterator pos, const T& value);
    void forEach(const std::function<void(T&)>&);
    void reverseForEach(const std::function<void(T&)>&);
    typename CollectionType<T>::iterator begin();
    typename CollectionType<T>::const_iterator cbegin() const;
    typename CollectionType<T>::iterator end();
    typename CollectionType<T>::const_iterator cend() const;
};

template <typename T,
template <typename, typename = std::allocator<T>> class CollectionType>
Genotype<T, CollectionType>::Genotype(const GenotypeInitializer<Genotype<T, CollectionType>>& genotypeInitializer) :
        genes() {
    genotypeInitializer.initialize(genes);
}

template <typename T,
template <typename, typename = std::allocator<T>> class CollectionType>
Genotype<T, CollectionType>::Genotype(CollectionType<T> genes) : genes(std::move(genes)) {
    // do nothing
}

template <typename T,
template <typename, typename = std::allocator<T>> class CollectionType>
bool Genotype<T, CollectionType>::operator==(const Genotype<T, CollectionType>& other) {
    return genes == other.genes;
}

template <typename T,
template <typename, typename = std::allocator<T>> class CollectionType>
bool Genotype<T, CollectionType>::operator<(const Genotype<T, CollectionType>& other) const {
    return std::lexicographical_compare(genes.cbegin(), genes.cend(),
                                        other.cbegin(), other.cend());
}


template <typename T,
template <typename, typename = std::allocator<T>> class CollectionType>
typename Genotype<T, CollectionType>::Collection::iterator
Genotype<T, CollectionType>::insert(typename Collection::iterator pos, const T& value) {
    return genes.insert(pos, value);
}

template <typename T,
template <typename, typename = std::allocator<T>> class CollectionType>
void Genotype<T, CollectionType>::forEach(const std::function<void(T&)>& callback) {
    std::for_each(genes.begin(), genes.end(), [&](T& gene) {
        callback(gene);
    });
}

template <typename T,
template <typename, typename = std::allocator<T>> class CollectionType>
void Genotype<T, CollectionType>::reverseForEach(const std::function<void(T&)>& callback) {
    std::for_each(genes.end(), genes.begin(), [&](T& gene) {
        callback(gene);
    });
}

template <typename T,
template <typename, typename = std::allocator<T>> class CollectionType>
typename CollectionType<T>::iterator Genotype<T, CollectionType>::begin() {
    return genes.begin();
}

template <typename T,
template <typename, typename = std::allocator<T>> class CollectionType>
typename CollectionType<T>::const_iterator Genotype<T, CollectionType>::cbegin() const {
    return genes.cbegin();
}

template <typename T,
template <typename, typename = std::allocator<T>> class CollectionType>
typename CollectionType<T>::iterator Genotype<T, CollectionType>::end() {
    return genes.end();
}

template <typename T,
template <typename, typename = std::allocator<T>> class CollectionType>
typename CollectionType<T>::const_iterator Genotype<T, CollectionType>::cend() const {
    return genes.cend();
}

}
