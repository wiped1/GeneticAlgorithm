#pragma once

#include <stdexcept>

template <typename Genotype>
struct MutationFunctor {
    virtual void operator()(Genotype&) const = 0;
    virtual ~MutationFunctor() { }
};
