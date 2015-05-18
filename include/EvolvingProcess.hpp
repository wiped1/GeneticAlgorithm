#pragma once

#include <iostream>
#include <functional>
#include <memory>
#include <exception>
#include <thread>
#include <mutex>
#if defined GALL_USE_TIMER
    #include <boost/timer/timer.hpp>
#endif
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
    EvolvingProcess<Genotype>& operator<<(Dependency);
    template <typename Dependency>
    EvolvingProcess<Genotype>& use(Dependency);
    void evolve(const std::function<bool(ObservableEvolutionStatus<Genotype>& status)>&);

private:
    void checkDependencies();
    void eliminationRoutine(Population<Genotype>&);
    void breedingRoutine(Population<Genotype>&);
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
    status.incrementNumberOfGenerations();
}

template <typename Collection>
unsigned int calculateDistance(const Collection& c) {
    return static_cast<unsigned int>(std::distance(c.cbegin(), c.cend()));
}


template <typename Genotype>
void EvolvingProcess<Genotype>::checkDependencies() {
    if (!(GenotypeInitializerDependency::get() && EvaluatorDependency::get() &&
          EliminationStrategyDependency::get() && BreedingOperatorDependency::get() &&
          CrossoverOperatorDependency::get() &&
          MutationOperatorDependency::get())) {
        throw new std::runtime_error("Dependencies aren't properly initialized. "
                                     "Check if all dependencies were injected.");
    }
}

template <typename Genotype>
void EvolvingProcess<Genotype>::eliminationRoutine(Population<Genotype>& population) {
    EliminationStrategyDependency::get()->eliminate(population);
}

template <typename Genotype>
void breedPopulation(Population<Genotype>& pop,
                     std::vector<Genotype>& auxGenotypes,
                     unsigned int maxPopulationSize,
                     const BreedingOperator<Genotype>& breedingOperator,
                     const CrossoverOperator<Genotype>& crossoverOperator,
                     const MutationOperator<Genotype>& mutationOperator,
                     std::mutex& populationMutex) {
    /* pop won't change throughout breedPopulation execution,
     * it's iterators are bidirectional, and calculate distance will perform
     * pop.size() steps in order to calculate the distance, hence we assign it
     * to local variable */
    auto currentPopulationSize = calculateDistance(pop);
    /* distance is calculated from remaining genotypes in population added
     * to auxiliary population members auxGenotypes is a vector which has
     * random access iterators, calculateDistance will perform in constant time */
    auto distance = [&]() { return currentPopulationSize + calculateDistance(auxGenotypes); };
    while (distance() < maxPopulationSize) {
        auto parentGenotypes = std::move(breedingOperator.breed(pop));
        auto newGenotype = std::move(crossoverOperator.cross(parentGenotypes));
        mutationOperator.mutate(newGenotype);
        populationMutex.lock();
        if (distance() <= maxPopulationSize) {
            auxGenotypes.push_back(std::move(newGenotype));
        }
        populationMutex.unlock();
        std::this_thread::yield();
    }
}

template <typename Genotype>
void EvolvingProcess<Genotype>::breedingRoutine(Population<Genotype>& population) {
    std::mutex populationMutex;
    /* auxPopulation is used as an auxiliary container to store newly bred genotypes */
    std::vector<Genotype> auxGenotypes;
    std::vector<std::thread> threads;
    for (std::vector<std::thread>::size_type i = 0; i < numOfThreads; i++) {
        threads.emplace_back(breedPopulation<Genotype>, std::ref(population),
                             std::ref(auxGenotypes), populationSize,
                             std::ref(*BreedingOperatorDependency::get()),
                             std::ref(*CrossoverOperatorDependency::get()),
                             std::ref(*MutationOperatorDependency::get()),
                             std::ref(populationMutex));
    }
    std::for_each(threads.begin(), threads.end(), [](std::thread& thread) {
        thread.join();
    });
    population.insert(auxGenotypes.begin(), auxGenotypes.end());
}

template <typename Genotype>
void EvolvingProcess<Genotype>::evolve(const std::function<bool(ObservableEvolutionStatus<Genotype>& status)>& terminationCondition) {
    checkDependencies();
    PopulationInitializer<Genotype> populationInitializer(*GenotypeInitializerDependency::get(), populationSize);
    Population<Genotype> pop(populationInitializer, *EvaluatorDependency::get());
    EvolutionStatus<Genotype> status(pop);
    do {
        #if defined GALL_USE_TIMER
            boost::timer::auto_cpu_timer t;
        #endif
        eliminationRoutine(pop);
        breedingRoutine(pop);
        updateEvolutionStatus(status, pop);
    } while (!terminationCondition(status));
}

}
