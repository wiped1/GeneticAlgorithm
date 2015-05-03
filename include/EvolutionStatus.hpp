#pragma once

#include <vector>
#include "Genotype.hpp"
#include "Population.hpp"
#include "ObservableEvolutionStatus.hpp"

template <typename T>
class EvolutionStatus : public ObservableEvolutionStatus<T> {
private:
    const Genotype<T>* bestGenotype; /* TODO remove? */
    const Population<T>* population;
    unsigned int numberOfGenerations;
    double currentHighestFitness;
    double fitnessDelta; /* difference between last fitness and current fitness */

public:
    EvolutionStatus(const Population<T>& population);

    virtual unsigned int getNumberOfGenerations();
    virtual double getHighestFitness();
    virtual double getFitnessDelta();
    virtual const std::vector<T>& getGenesWithBestFitness();
    virtual const Population<T>& getPopulation();
    virtual long getPopulationSize() const;

    void incrementNumberOfGenerations();
    void updateFitness(double fitness);
    void setGenotypeWithBestFitness(const Genotype<T>& genotype);
};

template <typename T>
EvolutionStatus<T>::EvolutionStatus(const Population<T> &population) :
        bestGenotype(nullptr), population(&population), numberOfGenerations(0),
        currentHighestFitness(0), fitnessDelta(0) {
    // do nothing
}

template <typename T>
unsigned int EvolutionStatus<T>::getNumberOfGenerations() {
    return numberOfGenerations;
}

template <typename T>
double EvolutionStatus<T>::getHighestFitness() {
    return currentHighestFitness;
}

template <typename T>
double EvolutionStatus<T>::getFitnessDelta() {
    return fitnessDelta;
}

template <typename T>
const std::vector<T>& EvolutionStatus<T>::getGenesWithBestFitness() {
    return const_cast<Genotype<T>*>(bestGenotype)->getGenes();
}

template <typename T>
const Population<T>& EvolutionStatus<T>::getPopulation() {
    return *population;
}

template <typename T>
void EvolutionStatus<T>::incrementNumberOfGenerations() {
    numberOfGenerations++;
}

template <typename T>
void EvolutionStatus<T>::updateFitness(double fitness) {
    fitnessDelta = fitness - currentHighestFitness;
    currentHighestFitness = fitness;
}

template <typename T>
void EvolutionStatus<T>::setGenotypeWithBestFitness(const Genotype<T> &genotype) {
    bestGenotype = &genotype;
}

template <typename T>
long EvolutionStatus<T>::getPopulationSize() const {
    return std::distance(population->cbegin(), population->cend());
}
