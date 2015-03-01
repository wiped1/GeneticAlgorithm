#pragma once

#include "Genotype.hpp"

template <typename T>
class GenotypeInitializer {
public:
    virtual void initialize(std::vector<T> &genes) const = 0;
};