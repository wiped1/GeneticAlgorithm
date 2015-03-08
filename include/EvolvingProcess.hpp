#pragma once

#include <iostream>
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
    std::unique_ptr<SelectionStrategy<T>> _selectionStrategy;
    const CrossoverStrategy<T>* _crossoverStrategy;
    const MutationStrategy<T>* _mutationStrategy;
    unsigned int _populationSize;

public:
    EvolvingProcess(unsigned int populationSize);
    EvolvingProcess<T>& operator<<(auto& dependency);
    EvolvingProcess<T>& use(const GenotypeInitializer<T>& genotypeInitializer);
    EvolvingProcess<T>& use(const Evaluator<T>& evaluator);
    EvolvingProcess<T>& use(SelectionStrategy<T> selectionStrategy);
    EvolvingProcess<T>& use(const CrossoverStrategy<T>& crossoverStrategy);
    EvolvingProcess<T>& use(const MutationStrategy<T>& mutationStrategy);
    EvolvingProcess<T>& setPopulationSize(unsigned int populationSize);
    void evolve();
};

template <typename T>
EvolvingProcess<T>::EvolvingProcess(unsigned int populationSize) : _populationSize(populationSize) {
    _selectionStrategy = std::unique_ptr<SelectionStrategy<T>>(new SelectionStrategy<T>());
}

/*
    The pitfalls of using auto:
        when dependency was passed as (auto dependency) the code produced
        dangling pointers and caused segmentation faults.
        Solution was to use (auto& dependency).
 */
template <typename T>
EvolvingProcess<T>& EvolvingProcess<T>::operator<<(auto& dependency) {
    return use(dependency);
}

template <typename T>
EvolvingProcess<T>& EvolvingProcess<T>::use(const GenotypeInitializer<T>& genotypeInitializer) {
    _genotypeInitializer = &genotypeInitializer;
    return *this;
}

template <typename T>
EvolvingProcess<T>& EvolvingProcess<T>::use(const Evaluator<T>& evaluator) {
    _evaluator = &evaluator;
    return *this;
}

template <typename T>
EvolvingProcess<T>& EvolvingProcess<T>::use(SelectionStrategy<T> selectionStrategy) {
    std::unique_ptr<SelectionStrategy<T>> ptr(std::make_unique<SelectionStrategy<T>>(selectionStrategy));
    _selectionStrategy = std::move(ptr);
    return *this;
}

template <typename T>
EvolvingProcess<T>& EvolvingProcess<T>::use(const CrossoverStrategy<T>& crossoverStrategy) {
    _crossoverStrategy = &crossoverStrategy;
    return *this;
}

template <typename T>
EvolvingProcess<T>& EvolvingProcess<T>::use(const MutationStrategy<T>& mutationStrategy) {
    _mutationStrategy = &mutationStrategy;
    return *this;
}

template <typename T>
EvolvingProcess<T>& EvolvingProcess<T>::setPopulationSize(unsigned int populationSize) {
    _populationSize = populationSize;
    return *this;
}

template <typename T>
void EvolvingProcess<T>::evolve() {
    PopulationInitializer<T> populationInitializer(*_genotypeInitializer, _populationSize);
    Population<T> pop(populationInitializer);
    _selectionStrategy->eliminate(pop, *_evaluator);
    _crossoverStrategy->cross(pop);
    _mutationStrategy->mutate(pop);
}

