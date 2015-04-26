#pragma once

#include "Genotype.hpp"
#include "Population.hpp"

template <typename T>
class EvolutionStatus {
private:
    const Genotype<T>* bestGenotype;
    const Population<T>* population;
    unsigned int numberOfGenerations;
    float currentFitness;
    float fitnessDelta; // difference between last fitness and current fitness
public:
    // constructor
    EvolutionStatus(const Population<T>& population);

    // getters
    unsigned int getNumberOfGenerations();
    // best genotype fitness of current generation
    // TODO rename to getHighestFitness
    float getBestFitness();
    // difference between last best fitness and current best fitness
    float getFitnessDelta();
    const Genotype<T>& getGenotypeWithBestFitness();
    const Population<T>& getPopulation();

    // setters
    void incrementNumberOfGenerations();
    void updateFitness(float fitness);
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
float EvolutionStatus<T>::getBestFitness() {
    return currentFitness;
}

template <typename T>
float EvolutionStatus<T>::getFitnessDelta() {
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
void EvolutionStatus<T>::updateFitness(float fitness) {
    fitnessDelta = fitness - currentFitness;
    currentFitness = fitness;
}

template <typename T>
void EvolutionStatus<T>::setGenotypeWithBestFitness(const Genotype<T> &genotype) {
    bestGenotype = &genotype;
}
