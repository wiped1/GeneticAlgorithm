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

/*
 * TODO spróbować stworzyć template PolymorphicDependency, który będzie posiadał
 * unique ptr jako zmienną składową i settery/gettery. EvolvingProcess będzie dziedziczył po tych
 * template'ach, dzięki czemu odwołanie do poszczególnych depsów będzie zależało od typu a nie od nazwy (którą trzeba powielać jak widać w tych prywatnych zmiennych)
 */
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

    using GenotypeInitializerDependency = PolymorphicDependency<GenotypeInitializer<T>>;
    using EvaluatorDependency           = PolymorphicDependency<Evaluator<T>>;
    using SelectionStrategyDependency   = PolymorphicDependency<SelectionStrategy<T>>;
    using CrossoverStrategyDependency   = PolymorphicDependency<CrossoverStrategy<T>>;
    using MutationStrategyDependency    = PolymorphicDependency<MutationStrategy<T>>;

public:
    EvolvingProcess(unsigned int populationSize);
    EvolvingProcess<T>& operator<<(auto dependency);
//    EvolvingProcess<T>& use(auto dependency);
    EvolvingProcess<T>& use(GenotypeInitializer<T>* genotypeInitializer);
    EvolvingProcess<T>& use(Evaluator<T>* evaluator);
    EvolvingProcess<T>& use(SelectionStrategy<T>* selectionStrategy);
    EvolvingProcess<T>& use(CrossoverStrategy<T>* crossoverStrategy);
    EvolvingProcess<T>& use(MutationStrategy<T>* mutationStrategy);
    EvolvingProcess<T>& setPopulationSize(unsigned int populationSize);
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

// TODO check if possible
// templates seem to not accept polymorphic parameters
// meaning that if i try to use PolymorphicDependency<MockGenotypeInitializer>
// it will not compile, as it expects GenotypeInitializer<T> (even though MockGenotypeInitializer inherits from it)
//template <typename T>
//EvolvingProcess<T>& EvolvingProcess<T>::use(auto dependency) {
//    this->PolymorphicDependency<typename std::remove_pointer<decltype(dependency)>::type>::set(dependency);
//    return *this;
//}

template <typename T>
EvolvingProcess<T>& EvolvingProcess<T>::use(GenotypeInitializer<T>* genotypeInitializer) {
    GenotypeInitializerDependency::set(genotypeInitializer);
    return *this;
}

template <typename T>
EvolvingProcess<T>& EvolvingProcess<T>::use(Evaluator<T>* evaluator) {
    EvaluatorDependency::set(evaluator);
    return *this;
}

template <typename T>
EvolvingProcess<T>& EvolvingProcess<T>::use(SelectionStrategy<T>* selectionStrategy) {
    SelectionStrategyDependency::set(selectionStrategy);
    return *this;
}

template <typename T>
EvolvingProcess<T>& EvolvingProcess<T>::use(CrossoverStrategy<T>* crossoverStrategy) {
    CrossoverStrategyDependency::set(crossoverStrategy);
    return *this;
}

template <typename T>
EvolvingProcess<T>& EvolvingProcess<T>::use(MutationStrategy<T>* mutationStrategy) {
    MutationStrategyDependency::set(mutationStrategy);
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

