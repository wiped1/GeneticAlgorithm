#pragma once

#include "Genotype.hpp"

namespace gall {

// TODO add Translator that returns Phenotype and let evaluator operate on pure Phenotypes
template <typename T>
class Evaluator {
public:
    virtual ~Evaluator() = default;
    virtual double evaluate(Genotype<T> &genotype) const = 0;
};

}
