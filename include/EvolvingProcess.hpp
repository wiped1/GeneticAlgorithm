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

namespace gall {

template <typename Genotype>
class EvolvingProcess :
        private PolymorphicDependency<GenotypeInitializer<Genotype>>,
        private PolymorphicDependency<Evaluator<Genotype>>,
        private PolymorphicDependency<SelectionStrategy<Genotype>>,
        private PolymorphicDependency<CrossoverStrategy<Genotype>>,
        private PolymorphicDependency<MutationStrategy<Genotype>> {
private:
    unsigned int _populationSize;

    // TODO zamienić na makro, które wygeneruje te wszystkie usingi
    // introduction of base classes members in order for use(auto dependency) to work
    using PolymorphicDependency<GenotypeInitializer<Genotype>>::set;
    using PolymorphicDependency<Evaluator<Genotype>>::set;
    using PolymorphicDependency<SelectionStrategy<Genotype>>::set;
    using PolymorphicDependency<CrossoverStrategy<Genotype>>::set;
    using PolymorphicDependency<MutationStrategy<Genotype>>::set;
    // aliases for easier base classes access
    using GenotypeInitializerDependency = PolymorphicDependency<GenotypeInitializer<Genotype>>;
    using EvaluatorDependency           = PolymorphicDependency<Evaluator<Genotype>>;
    using SelectionStrategyDependency   = PolymorphicDependency<SelectionStrategy<Genotype>>;
    using CrossoverStrategyDependency   = PolymorphicDependency<CrossoverStrategy<Genotype>>;
    using MutationStrategyDependency    = PolymorphicDependency<MutationStrategy<Genotype>>;

public:
    EvolvingProcess(unsigned int populationSize);
    template <typename Dependency>
    EvolvingProcess<Genotype>& operator<<(Dependency dependency);
    template <typename Dependency>
    EvolvingProcess<Genotype>& use(Dependency dependency);
    void evolve(const std::function<bool(ObservableEvolutionStatus<Genotype>& status)>& terminationCondition);
};

template <typename Genotype>
EvolvingProcess<Genotype>::EvolvingProcess(unsigned int populationSize) :
        _populationSize(populationSize) {
    SelectionStrategyDependency::set(new DefaultSelectionStrategy<Genotype>());
}

template <typename Genotype>
template <typename Dependency>
EvolvingProcess<Genotype>& EvolvingProcess<Genotype>::operator<<(Dependency dependency) {
    return use(dependency);
}

template <typename Genotype>
template <typename Dependency>
EvolvingProcess<Genotype>& EvolvingProcess<Genotype>::use(Dependency dependency) {
    set(dependency);
    return *this;
}

template <typename Genotype>
void updateEvolutionStatus(EvolutionStatus<Genotype>& status, Population<Genotype> population) {
    // population genotypes are sorted high to low, first element is bound to be the one with highest fitness
    auto it = population.cbegin();
    status.setGenotypeWithBestFitness((*it).first);
    status.updateFitness((*it).second);
    status.incrementNumberOfGenerations();
}

template <typename Genotype>
void EvolvingProcess<Genotype>::evolve(const std::function<bool(ObservableEvolutionStatus<Genotype>& status)>& terminationCondition) {
    /* TODO ta funkcja jest strasznie nieodporna na zmiany, łatwo zapomnieć jeśli
     * doda się jakiś dependency i będzie kiszka, jak to zrobić żeby się nie narobić? */
    // check if all dependencies are properly initialized
    if (!(GenotypeInitializerDependency::get() && EvaluatorDependency::get() &&
          SelectionStrategyDependency::get() && CrossoverStrategyDependency::get() &&
          MutationStrategyDependency::get())) {
        throw new std::runtime_error("Dependencies aren't properly initialized. "
                                     "Check if all dependencies were injected.");
    }

    PopulationInitializer<Genotype> populationInitializer(*GenotypeInitializerDependency::get(), _populationSize);
    Population<Genotype> pop(populationInitializer, *EvaluatorDependency::get());
    EvolutionStatus<Genotype> status(pop);
    do {
        SelectionStrategyDependency::get()->eliminate(pop);
        CrossoverStrategyDependency::get()->cross(pop);
        MutationStrategyDependency::get()->mutate(pop);
        updateEvolutionStatus(status, pop);
    } while (!terminationCondition(status));
}

}
