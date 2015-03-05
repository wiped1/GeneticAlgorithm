#pragma once

#include "Genotype.hpp"

template <typename T>
class GenotypeInitializer {
public:
    virtual ~GenotypeInitializer() = default;
    virtual void initialize(std::vector<T> &genes) const = 0;
};
