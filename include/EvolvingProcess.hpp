#pragma once

#include <iostream>
#include <functional>
#include <memory>
#include <exception>
#include "PolymorphicDependency.hpp"
#include "GenotypeInitializer.hpp"
#include "PopulationInitializer.hpp"
#include "Evaluator.hpp"
#include "SelectionStrategy.hpp"
#include "CrossoverStrategy.hpp"
#include "MutationStrategy.hpp"

template <typename T>
class EvolvingProcess :
        public PolymorphicDependency<GenotypeInitializer<T>>,
        public PolymorphicDependency<Evaluator<T>>,
        public PolymorphicDependency<SelectionStrategy<T>>,
        public PolymorphicDependency<CrossoverStrategy<T>>,
        public PolymorphicDependency<MutationStrategy<T>> {
private:
    unsigned int _populationSize;
    unsigned int _generations;

    // introduction of base classes members in order for use(auto dependency) to work
    using PolymorphicDependency<GenotypeInitializer<T>>::set;
    using PolymorphicDependency<Evaluator<T>>::set;
    using PolymorphicDependency<SelectionStrategy<T>>::set;
    using PolymorphicDependency<CrossoverStrategy<T>>::set;
    using PolymorphicDependency<MutationStrategy<T>>::set;
    using GenotypeInitializerDependency = PolymorphicDependency<GenotypeInitializer<T>>;
    using EvaluatorDependency           = PolymorphicDependency<Evaluator<T>>;
    using SelectionStrategyDependency   = PolymorphicDependency<SelectionStrategy<T>>;
    using CrossoverStrategyDependency   = PolymorphicDependency<CrossoverStrategy<T>>;
    using MutationStrategyDependency    = PolymorphicDependency<MutationStrategy<T>>;

public:
    EvolvingProcess(unsigned int populationSize);
    EvolvingProcess<T>& operator<<(auto dependency);
    EvolvingProcess<T>& use(auto dependency);
    unsigned int getNumberOfGenerations();
    void evolve(const std::function<bool(const Population<T>& pop,
            unsigned int generations)>& terminationCondition);
};

template <typename T>
EvolvingProcess<T>::EvolvingProcess(unsigned int populationSize) :
        _populationSize(populationSize), _generations(0) {
    //TODO make SelectionStrategy an interface and create default implementation for it
    SelectionStrategyDependency::set(new SelectionStrategy<T>());
}

/*
    The pitfalls of using auto:
        when dependency was passed as (auto dependency) the code produced
        dangling pointers and caused segmentation faults.
        Solution was to use (auto& dependency).
 */
template <typename T>
EvolvingProcess<T>& EvolvingProcess<T>::operator<<(auto dependency) {
    return use(dependency);
}

template <typename T>
EvolvingProcess<T>& EvolvingProcess<T>::use(auto dependency) {
    set(dependency);
    return *this;
}

template <typename T>
unsigned int EvolvingProcess<T>::getNumberOfGenerations() {
    return _generations;
}

/*
 * TODO change explicit Population, and generations reference to class that 'remembers'
 * the history of specific generations (EvolvingHistory)
 */
template <typename T>
void EvolvingProcess<T>::evolve(const std::function<bool(const Population<T>& pop,
            unsigned int generations)>& terminationCondition) {
    PopulationInitializer<T> populationInitializer(*GenotypeInitializerDependency::get(), _populationSize);
    Population<T> pop(populationInitializer);
    while (!terminationCondition(pop, _generations)) {
        SelectionStrategyDependency::get()->eliminate(pop, *EvaluatorDependency::get());
        CrossoverStrategyDependency::get()->cross(pop);
        MutationStrategyDependency::get()->mutate(pop);
        _generations++;
    }
}

