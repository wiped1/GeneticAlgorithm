#pragma once

#include <iostream>
#include <functional>
#include <memory>
#include <exception>
#include <thread>
#include <mutex>
#include "PolymorphicDependency.hpp"
#include "GenotypeInitializer.hpp"
#include "PopulationInitializer.hpp"
#include "Evaluator.hpp"
#include "EliminationStrategy.hpp"
#include "DefaultEliminationStrategy.hpp"
#include "BreedingOperator.hpp"
#include "CrossoverOperator.hpp"
#include "MutationOperator.hpp"
#include "EvolutionStatus.hpp"
#include "ObservableEvolutionStatus.hpp"

namespace gall {

template <typename Genotype>
class EvolvingProcess :
        private PolymorphicDependency<GenotypeInitializer<Genotype>>,
        private PolymorphicDependency<Evaluator<Genotype>>,
        private PolymorphicDependency<EliminationStrategy<Genotype>>,
        private PolymorphicDependency<BreedingOperator<Genotype>>,
        private PolymorphicDependency<CrossoverOperator<Genotype>>,
        private PolymorphicDependency<MutationOperator<Genotype>> {
private:
    unsigned int populationSize;
    unsigned int numOfThreads;

    // TODO zamienić na makro, które wygeneruje te wszystkie usingi
    // introduction of base classes members in order for use(auto dependency) to work
    using PolymorphicDependency<GenotypeInitializer<Genotype>>::set;
    using PolymorphicDependency<Evaluator<Genotype>>::set;
    using PolymorphicDependency<EliminationStrategy<Genotype>>::set;
    using PolymorphicDependency<BreedingOperator<Genotype>>::set;
    using PolymorphicDependency<CrossoverOperator<Genotype>>::set;
    using PolymorphicDependency<MutationOperator<Genotype>>::set;
    // aliases for easier base classes access
    using GenotypeInitializerDependency = PolymorphicDependency<GenotypeInitializer<Genotype>>;
    using EvaluatorDependency           = PolymorphicDependency<Evaluator<Genotype>>;
    using EliminationStrategyDependency   = PolymorphicDependency<EliminationStrategy<Genotype>>;
    using BreedingOperatorDependency   = PolymorphicDependency<BreedingOperator<Genotype>>;
    using CrossoverOperatorDependency   = PolymorphicDependency<CrossoverOperator<Genotype>>;
    using MutationOperatorDependency    = PolymorphicDependency<MutationOperator<Genotype>>;

public:
    EvolvingProcess(unsigned int populationSize, unsigned int numOfThreads = 1);
    template <typename Dependency>
    EvolvingProcess<Genotype>& operator<<(Dependency dependency);
    template <typename Dependency>
    EvolvingProcess<Genotype>& use(Dependency dependency);
    void evolve(const std::function<bool(ObservableEvolutionStatus<Genotype>& status)>& terminationCondition);
};

template <typename Genotype>
EvolvingProcess<Genotype>::EvolvingProcess(unsigned int populationSize, unsigned int numOfThreads) :
        populationSize(populationSize), numOfThreads(numOfThreads) {
    EliminationStrategyDependency::set(new DefaultEliminationStrategy<Genotype>());
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
void breed(Population<Genotype>& pop, unsigned int populationSize,
        const BreedingOperator<Genotype>& breedingOperator,
        const CrossoverOperator<Genotype>& crossoverOperator,
        const MutationOperator<Genotype>& mutationOperator, std::mutex& populationMutex) {
    while (std::distance(pop.begin(), pop.end()) < populationSize) {
        auto parentGenotypes = breedingOperator.breed(pop);
        auto newGenotype = std::move(crossoverOperator.cross(parentGenotypes));
        mutationOperator.mutate(newGenotype);
        populationMutex.lock();
        if (std::distance(pop.begin(), pop.end()) =< populationSize) {
            pop.insert(newGenotype);
        }
        populationMutex.unlock();
        std::this_thread::yield();
    }
}

template <typename Genotype>
void EvolvingProcess<Genotype>::evolve(const std::function<bool(ObservableEvolutionStatus<Genotype>& status)>& terminationCondition) {
    /* TODO ta funkcja jest strasznie nieodporna na zmiany, łatwo zapomnieć jeśli
     * doda się jakiś dependency i będzie kiszka, jak to zrobić żeby się nie narobić? */
    // check if all dependencies are properly initialized
    if (!(GenotypeInitializerDependency::get() && EvaluatorDependency::get() &&
          EliminationStrategyDependency::get() && BreedingOperatorDependency::get() &&
          CrossoverOperatorDependency::get() &&
          MutationOperatorDependency::get())) {
        throw new std::runtime_error("Dependencies aren't properly initialized. "
                                             "Check if all dependencies were injected.");
    }

    PopulationInitializer<Genotype> populationInitializer(*GenotypeInitializerDependency::get(),
                                                          populationSize);
    Population<Genotype> pop(populationInitializer, *EvaluatorDependency::get());
    EvolutionStatus<Genotype> status(pop);
    std::mutex populationMutex;
    do {
        EliminationStrategyDependency::get()->eliminate(pop);
        std::vector<std::thread> threads;
        for (std::vector<std::thread>::size_type i = 0; i < numOfThreads; i++) {
            threads.emplace_back(breed<Genotype>, std::ref(pop), populationSize,
                    std::ref(*BreedingOperatorDependency::get()),
                    std::ref(*CrossoverOperatorDependency::get()),
                    std::ref(*MutationOperatorDependency::get()), std::ref(populationMutex));
        }
        std::for_each(threads.begin(), threads.end(), [](std::thread& thread) {
            thread.join();
        });
//        while (std::distance(pop.begin(), pop.end()) < _populationSize) {
//            auto parentGenotypes = BreedingOperatorDependency::get()->breed(pop);
//            auto newGenotype = std::move(CrossoverOperatorDependency::get()->cross(parentGenotypes));
//            MutationOperatorDependency::get()->mutate(newGenotype);
//            pop.insert(newGenotype);
//        }
        updateEvolutionStatus(status, pop);
    } while (!terminationCondition(status));
}

}
