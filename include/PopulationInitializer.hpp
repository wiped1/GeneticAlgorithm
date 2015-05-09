#pragma once

#include <vector>
#include "GenotypeInitializer.hpp"
#include "Population.hpp"
#include "Evaluator.hpp"

namespace gall {

template <typename Genotype>
class Population;

template <typename Genotype>
class PopulationInitializer {
private:
    unsigned int size;
    const GenotypeInitializer<Genotype>& initializer;
public:
    PopulationInitializer(const GenotypeInitializer<Genotype>& initializer, unsigned int size);
    void initialize(typename Population<Genotype>::CollectionType& genotypes,
                    const Evaluator<Genotype>& evaluator) const;
};

template <typename Genotype>
PopulationInitializer<Genotype>::PopulationInitializer(const GenotypeInitializer<Genotype>& initializer,
        unsigned int size) : initializer(initializer), size(size) {
    // do nothing
}

template <typename Genotype>
void PopulationInitializer<Genotype>::initialize(typename Population<Genotype>::CollectionType& genotypes,
                                          const Evaluator<Genotype>& evaluator) const {
    for (unsigned int i = 0; i < size; i++) {
        Genotype genotype{initializer};
        double fitness = evaluator.evaluate(genotype);
        typename Population<Genotype>::ValueType value{genotype, fitness};
        genotypes.insert(value);
    }
}

}
