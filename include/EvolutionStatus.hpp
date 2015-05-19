#pragma once

#include <iostream>
#include <vector>
#include "Population.hpp"
#include "ObservableEvolutionStatus.hpp"

namespace gall {

template <typename Genotype>
class EvolutionStatus : public ObservableEvolutionStatus<Genotype> {
private:
    const Population<Genotype>* population;
    unsigned int numberOfGenerations;

public:
    EvolutionStatus(const Population<Genotype>& population);

    virtual unsigned int getNumberOfGenerations();
    virtual double getHighestFitness();
    virtual const Genotype& getGenotypeWithBestFitness();
    virtual const Population<Genotype>& getPopulation();
    virtual long getPopulationSize() const;

    void incrementNumberOfGenerations();
};

template <typename Genotype>
EvolutionStatus<Genotype>::EvolutionStatus(const Population<Genotype> &population) :
        population(&population), numberOfGenerations(0) {
    // do nothing
}

template <typename Genotype>
unsigned int EvolutionStatus<Genotype>::getNumberOfGenerations() {
    return numberOfGenerations;
}

template <typename Genotype>
double EvolutionStatus<Genotype>::getHighestFitness() {
    return std::prev(population->cend())->second;
}

template <typename Genotype>
const Genotype& EvolutionStatus<Genotype>::getGenotypeWithBestFitness() {
    return std::prev(population->cend())->first;
}

template <typename Genotype>
const Population<Genotype>& EvolutionStatus<Genotype>::getPopulation() {
    return *population;
}

template <typename Genotype>
void EvolutionStatus<Genotype>::incrementNumberOfGenerations() {
    numberOfGenerations++;
}

template <typename Genotype>
long EvolutionStatus<Genotype>::getPopulationSize() const {
    return std::distance(population->cbegin(), population->cend());
}

}
