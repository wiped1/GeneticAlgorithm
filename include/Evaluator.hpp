#pragma once

#include "Genotype.hpp"

namespace gall {

// TODO add Translator that returns Phenotype and let evaluator operate on pure Phenotypes
template <typename Genotype>
class Evaluator {
public:
    virtual ~Evaluator() = default;
    virtual double evaluate(const Genotype &genotype) const = 0;
};

}
