#pragma once

#include "Population.hpp"

namespace gall {

template <typename Genotype>
class MutationStrategy {
public:
    virtual ~MutationStrategy() = default;
    virtual void mutate(Population<Genotype>& population) const = 0;
};

}
