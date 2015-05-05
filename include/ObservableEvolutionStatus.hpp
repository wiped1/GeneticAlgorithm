#pragma once

#include "Genotype.hpp"
#include "Population.hpp"

namespace gall {

template <typename T>
class ObservableEvolutionStatus {
public:
    virtual unsigned int getNumberOfGenerations() = 0;
    /* Best genotype fitness of current generation */
    virtual double getHighestFitness() = 0;
    /* Difference between last best fitness and current best fitness */
    virtual double getFitnessDelta() = 0;
    virtual const Genotype<T>& getGenotypeWithBestFitness() = 0;
    virtual const Population<T>& getPopulation() = 0;
    virtual long getPopulationSize() const = 0;
};

}
