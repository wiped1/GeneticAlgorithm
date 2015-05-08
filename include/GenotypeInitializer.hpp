#pragma once

#include <vector>

namespace gall {

template <typename Genotype>
class GenotypeInitializer {
public:
    virtual ~GenotypeInitializer() = default;
    virtual void initialize(typename Genotype::Collection &genes) const = 0;
};

}
