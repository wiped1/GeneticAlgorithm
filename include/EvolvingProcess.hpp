#pragma once

#include <memory>
#include "GenotypeInitializer.hpp"
#include "PopulationInitializer.hpp"
#include "Evaluator.hpp"
#include "SelectionStrategy.hpp"
#include "CrossoverStrategy.hpp"
#include "MutationStrategy.hpp"

template <typename T>
class EvolvingProcess {
private:
    const GenotypeInitializer<T>* _genotypeInitializer;
    const Evaluator<T>* _evaluator;
    const CrossoverStrategy<T>* _crossoverStrategy;
    const MutationStrategy<T>* _mutationStrategy;

public:
    EvolvingProcess<T>& operator<<(auto dependency);
    EvolvingProcess<T>& use(const GenotypeInitializer<T>& genotypeInitializer);
    EvolvingProcess<T>& use(const Evaluator<T>& evaluator);
    EvolvingProcess<T>& use(const CrossoverStrategy<T>& crossoverStrategy);
    EvolvingProcess<T>& use(const MutationStrategy<T>& mutationStrategy);
};

template <typename T>
EvolvingProcess<T>& EvolvingProcess<T>::operator<<(auto dependency) {
    return use(dependency);
}

template <typename T>
EvolvingProcess<T>& EvolvingProcess<T>::use(const GenotypeInitializer<T>& genotypeInitializer) {
    _genotypeInitializer = &genotypeInitializer;
    return *this;
}

template <typename T>
EvolvingProcess<T>& EvolvingProcess<T>::use(const Evaluator<T> &evaluator) {
    _evaluator = &evaluator;
    return *this;
}

template <typename T>
EvolvingProcess<T>& EvolvingProcess<T>::use(const CrossoverStrategy<T> &crossoverStrategy) {
    _crossoverStrategy = &crossoverStrategy;
    return *this;
}

template <typename T>
EvolvingProcess<T>& EvolvingProcess<T>::use(const MutationStrategy<T> &mutationStrategy) {
    _mutationStrategy = &mutationStrategy;
    return *this;
}
