#pragma once

#include <stdexcept>

template <typename GenotypeCollection>
struct MutationFunctor {
    virtual void mutate(GenotypeCollection&) const = 0;
    virtual ~MutationFunctor() { }
};
