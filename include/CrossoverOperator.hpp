#pragma once

#include "Population.hpp"

namespace gall {

template <typename Genotype>
class CrossoverOperator {
public:
    virtual ~CrossoverOperator() = default;
    virtual Genotype cross(std::vector<Genotype>& genotypes) const = 0;
};

}
