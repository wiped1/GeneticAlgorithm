#pragma once

#include "Population.hpp"

namespace gall {

template <typename Genotype>
class ObservableEvolutionStatus {
public:
    virtual unsigned int getNumberOfGenerations() = 0;
    /* Best genotype fitness of current generation */
    virtual double getHighestFitness() = 0;
    virtual const Genotype& getGenotypeWithBestFitness() = 0;
    virtual const Population<Genotype>& getPopulation() = 0;
    virtual long getPopulationSize() const = 0;
};

}
