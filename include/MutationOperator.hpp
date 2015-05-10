#pragma once

#include "Population.hpp"

namespace gall {

template <typename Genotype>
class MutationOperator {
public:
    virtual ~MutationOperator() = default;
    virtual void mutate(Genotype &genotype) const = 0;
};

}
