#pragma once

#include <iostream>
#include <vector>
#include "Population.hpp"
#include "ObservableEvolutionStatus.hpp"

namespace gall {

template <typename Genotype>
class EvolutionStatus : public ObservableEvolutionStatus<Genotype> {
private:
    const Genotype* bestGenotype; /* TODO remove? */
    const Population<Genotype>* population;
    unsigned int numberOfGenerations;
    double currentHighestFitness;
    double fitnessDelta; /* difference between last fitness and current fitness */

public:
    EvolutionStatus(const Population<Genotype>& population);

    virtual unsigned int getNumberOfGenerations();
    virtual double getHighestFitness();
    virtual double getFitnessDelta();
    virtual const Genotype& getGenotypeWithBestFitness();
    virtual const Population<Genotype>& getPopulation();
    virtual long getPopulationSize() const;

    void incrementNumberOfGenerations();
    void updateFitness(double fitness);
    void setGenotypeWithBestFitness(const Genotype& genotype);
};

template <typename Genotype>
EvolutionStatus<Genotype>::EvolutionStatus(const Population<Genotype> &population) :
        bestGenotype(nullptr), population(&population), numberOfGenerations(0),
        currentHighestFitness(0), fitnessDelta(0) {
    // do nothing
}

template <typename Genotype>
unsigned int EvolutionStatus<Genotype>::getNumberOfGenerations() {
    return numberOfGenerations;
}

template <typename Genotype>
double EvolutionStatus<Genotype>::getHighestFitness() {
    return currentHighestFitness;
}

template <typename Genotype>
double EvolutionStatus<Genotype>::getFitnessDelta() {
    return fitnessDelta;
}

template <typename Genotype>
const Genotype& EvolutionStatus<Genotype>::getGenotypeWithBestFitness() {
    return *bestGenotype;
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
void EvolutionStatus<Genotype>::updateFitness(double fitness) {
    fitnessDelta = fitness - currentHighestFitness;
    currentHighestFitness = fitness;
}

template <typename Genotype>
void EvolutionStatus<Genotype>::setGenotypeWithBestFitness(const Genotype& genotype) {
    bestGenotype = &genotype;
}

template <typename Genotype>
long EvolutionStatus<Genotype>::getPopulationSize() const {
    return std::distance(population->cbegin(), population->cend());
}

}
