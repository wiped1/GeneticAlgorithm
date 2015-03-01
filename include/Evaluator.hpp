#pragma once

#include "Genotype.hpp"

class Evaluator {
public:
    virtual ~Evaluator() = default;
    template <typename T>
    virtual double evalute(const Genotype<T> &genotype) = 0;
};