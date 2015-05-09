#pragma once

#include "Population.hpp"

namespace gall {

template <typename Genotype>
class BreedingOperator {
public:
    virtual ~BreedingOperator() = default;
    virtual std::vector<Genotype> breed(const Population<Genotype>& population) const = 0;
};

}
