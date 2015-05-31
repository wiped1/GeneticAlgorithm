#pragma once

#include <stdexcept>

template <typename Genotype>
struct MutationFunctor {
    virtual void mutate(Genotype&) const = 0;
    virtual ~MutationFunctor() { }
};
