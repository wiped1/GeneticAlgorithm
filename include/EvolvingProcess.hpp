#pragma once

#include <iostream>
#include <functional>
#include <memory>
#include <exception>
#include "PolymorphicDependency.hpp"
#include "GenotypeInitializer.hpp"
#include "PopulationInitializer.hpp"
#include "Ranking.hpp"
#include "Evaluator.hpp"
#include "SelectionStrategy.hpp"
#include "DefaultSelectionStrategy.hpp"
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

    // TODO zamienić na makro, które wygeneruje te wszystkie usingi
    // introduction of base classes members in order for use(auto dependency) to work
    using PolymorphicDependency<GenotypeInitializer<T>>::set;
    using PolymorphicDependency<Evaluator<T>>::set;
    using PolymorphicDependency<SelectionStrategy<T>>::set;
    using PolymorphicDependency<CrossoverStrategy<T>>::set;
    using PolymorphicDependency<MutationStrategy<T>>::set;
    // aliases for easier base classes access
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
    SelectionStrategyDependency::set(new DefaultSelectionStrategy<T>());
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
    // check if all dependencies are properly initialized
    if (!(GenotypeInitializerDependency::get() && EvaluatorDependency::get() &&
          SelectionStrategyDependency::get() && CrossoverStrategyDependency::get() &&
          MutationStrategyDependency::get())) {
        throw new std::runtime_error("Dependencies aren't properly initialized. Check if all dependencies were injected.");
    }

    PopulationInitializer<T> populationInitializer(*GenotypeInitializerDependency::get(), _populationSize);
    Population<T> pop(populationInitializer);
    while (!terminationCondition(pop, _generations)) {
        //ranking.rank(pop, evaluator, translator); // returns std::set where key is genotype reference and value is it's fitness
        Ranking<T> ranking;
        SelectionStrategyDependency::get()->eliminate(pop, ranking.rank(pop, *EvaluatorDependency::get()));
        CrossoverStrategyDependency::get()->cross(pop);
        MutationStrategyDependency::get()->mutate(pop);
        _generations++;
    }
}

