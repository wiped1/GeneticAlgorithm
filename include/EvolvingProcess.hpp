#pragma once

#include <iostream>
#include <functional>
#include <memory>
#include <exception>
#include "GenotypeInitializer.hpp"
#include "PopulationInitializer.hpp"
#include "Evaluator.hpp"
#include "SelectionStrategy.hpp"
#include "CrossoverStrategy.hpp"
#include "MutationStrategy.hpp"

//inline class NullTerminationConditionException : public std::exception {
//
//};

/* TODO spróbować stworzyć template PolymorphicDependency, który będzie posiadał
 * unique ptr jako zmienną składową i settery/gettery. EvolvingProcess będzie dziedziczył po tych
 * template'ach, dzięki czemu odwołanie do poszczególnych depsów będzie zależało od typu a nie od nazwy (którą trzeba powielać jak widać w tych prywatnych zmiennych)
 */

template <typename T>
class EvolvingProcess {
private:
    const GenotypeInitializer<T>* _genotypeInitializer;
    const Evaluator<T>* _evaluator;
    std::unique_ptr<SelectionStrategy<T>> _selectionStrategy;
    const CrossoverStrategy<T>* _crossoverStrategy;
    const MutationStrategy<T>* _mutationStrategy;
    unsigned int _populationSize;
    unsigned int _generations;

public:
    EvolvingProcess(unsigned int populationSize);
    EvolvingProcess<T>& operator<<(auto& dependency);
    EvolvingProcess<T>& use(const GenotypeInitializer<T>& genotypeInitializer);
    EvolvingProcess<T>& use(const Evaluator<T>& evaluator);
    EvolvingProcess<T>& use(SelectionStrategy<T>& selectionStrategy);
    EvolvingProcess<T>& use(const CrossoverStrategy<T>& crossoverStrategy);
    EvolvingProcess<T>& use(const MutationStrategy<T>& mutationStrategy);
    EvolvingProcess<T>& setPopulationSize(unsigned int populationSize);
    unsigned int getNumberOfGenerations();
    void evolve(const std::function<bool(const Population<T>& pop,
            unsigned int generations)>& terminationCondition);
};

template <typename T>
EvolvingProcess<T>::EvolvingProcess(unsigned int populationSize) :
        _populationSize(populationSize), _generations(0) {
    //TODO make SelectionStrategy an interface and create default implementation for it
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
EvolvingProcess<T>& EvolvingProcess<T>::use(SelectionStrategy<T>& selectionStrategy) {
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
unsigned int EvolvingProcess<T>::getNumberOfGenerations() {
    return _generations;
}

/* TODO change explicit Population, and generations reference to class that 'remembers'
 * the history of specific generations (EvolvingHistory)
 */
template <typename T>
void EvolvingProcess<T>::evolve(const std::function<bool(const Population<T>& pop,
            unsigned int generations)>& terminationCondition) {
    PopulationInitializer<T> populationInitializer(*_genotypeInitializer, _populationSize);
    Population<T> pop(populationInitializer);
    while (!terminationCondition(pop, _generations)) {
        _selectionStrategy->eliminate(pop, *_evaluator);
        _crossoverStrategy->cross(pop);
        _mutationStrategy->mutate(pop);
        _generations++;
    }
}

