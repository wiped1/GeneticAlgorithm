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
#include "EvolutionStatus.hpp"
#include "ObservableEvolutionStatus.hpp"

template <typename T>
class EvolvingProcess :
        private PolymorphicDependency<GenotypeInitializer<T>>,
        private PolymorphicDependency<Evaluator<T>>,
        private PolymorphicDependency<SelectionStrategy<T>>,
        private PolymorphicDependency<CrossoverStrategy<T>>,
        private PolymorphicDependency<MutationStrategy<T>> {
private:
    unsigned int _populationSize;

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
    template <typename Dependency>
    EvolvingProcess<T>& operator<<(Dependency dependency);
    template <typename Dependency>
    EvolvingProcess<T>& use(Dependency dependency);
    void evolve(const std::function<bool(ObservableEvolutionStatus<T>& status)>& terminationCondition);
};

template <typename T>
EvolvingProcess<T>::EvolvingProcess(unsigned int populationSize) :
        _populationSize(populationSize) {
    SelectionStrategyDependency::set(new DefaultSelectionStrategy<T>());
}

template <typename T>
template <typename Dependency>
EvolvingProcess<T>& EvolvingProcess<T>::operator<<(Dependency dependency) {
    return use(dependency);
}

template <typename T>
template <typename Dependency>
EvolvingProcess<T>& EvolvingProcess<T>::use(Dependency dependency) {
    set(dependency);
    return *this;
}

template <typename T>
void updateEvolutionStatus(EvolutionStatus<T>& status, const typename Ranking<T>::CollectionType& rankedGenotypes) {
    // set is sorted high to low, first element is bound to be the one with highest fitness
    auto it = rankedGenotypes.cbegin();
    status.setGenotypeWithBestFitness(*(*it).first);
    status.updateFitness((*it).second);
    status.incrementNumberOfGenerations();
}

template <typename T>
void EvolvingProcess<T>::evolve(const std::function<bool(ObservableEvolutionStatus<T>& status)>& terminationCondition) {
    // check if all dependencies are properly initialized
    if (!(GenotypeInitializerDependency::get() && EvaluatorDependency::get() &&
          SelectionStrategyDependency::get() && CrossoverStrategyDependency::get() &&
          MutationStrategyDependency::get())) {
        throw new std::runtime_error("Dependencies aren't properly initialized. "
                                     "Check if all dependencies were injected.");
    }

    PopulationInitializer<T> populationInitializer(*GenotypeInitializerDependency::get(), _populationSize);
    Population<T> pop(populationInitializer);
    EvolutionStatus<T> status(pop);
    do {
        Ranking<T> ranking;
        typename Ranking<T>::CollectionType rankedGenotypes = ranking.rank(pop, *EvaluatorDependency::get());
        SelectionStrategyDependency::get()->eliminate(pop, rankedGenotypes);
        CrossoverStrategyDependency::get()->cross(pop);
        MutationStrategyDependency::get()->mutate(pop);
        updateEvolutionStatus(status, rankedGenotypes);
    } while (!terminationCondition(status));
}

