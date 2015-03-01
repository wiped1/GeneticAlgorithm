#pragma once

#include "Genotype.hpp"

template <typename T>
class Evaluator {
public:
    virtual ~Evaluator() = default;
    virtual double evaluate(Genotype<T> &genotype) const = 0;
};