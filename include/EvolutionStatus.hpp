#pragma once

#include "Genotype.hpp"
#include "Population.hpp"
#include "ObservableEvolutionStatus.hpp"

template <typename T>
class EvolutionStatus : public ObservableEvolutionStatus<T> {
private:
    const Genotype<T>* bestGenotype;
    const Population<T>* population;
    unsigned int numberOfGenerations;
    double currentFitness;
    double fitnessDelta; // difference between last fitness and current fitness
public:
    EvolutionStatus(const Population<T>& population);

    virtual unsigned int getNumberOfGenerations();
    virtual double getHighestFitness();
    virtual double getFitnessDelta();
    virtual const Genotype<T>& getGenotypeWithBestFitness();
    virtual const Population<T>& getPopulation();
    virtual unsigned int getPopulationSize() const;

    void incrementNumberOfGenerations();
    void updateFitness(double fitness);
    void setGenotypeWithBestFitness(const Genotype<T>& genotype);
};

template <typename T>
EvolutionStatus<T>::EvolutionStatus(const Population<T> &population) :
        bestGenotype(nullptr), population(&population), numberOfGenerations(0),
        currentFitness(0), fitnessDelta(0) {
    // do nothing
}

template <typename T>
unsigned int EvolutionStatus<T>::getNumberOfGenerations() {
    return numberOfGenerations;
}

template <typename T>
double EvolutionStatus<T>::getHighestFitness() {
    return currentFitness;
}

template <typename T>
double EvolutionStatus<T>::getFitnessDelta() {
    return fitnessDelta;
}

template <typename T>
const Genotype<T>& EvolutionStatus<T>::getGenotypeWithBestFitness() {
    return *bestGenotype;
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
    fitnessDelta = fitness - currentFitness;
    currentFitness = fitness;
}

template <typename T>
void EvolutionStatus<T>::setGenotypeWithBestFitness(const Genotype<T> &genotype) {
    bestGenotype = &genotype;
}

template <typename T>
unsigned int EvolutionStatus<T>::getPopulationSize() const {
    return static_cast<unsigned int>(const_cast<Population<T>*>(population)->getGenotypes().size());
}
