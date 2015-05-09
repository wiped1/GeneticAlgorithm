#include "Catch/catch.hpp"

#include <iostream>
#include <vector>
#include "EvolvingProcess.hpp"
#include "GenotypeInitializer.hpp"
#include "Evaluator.hpp"
#include "EliminationStrategy.hpp"
#include "CrossoverOperator.hpp"
#include "MutationOperator.hpp"
#include "ObservableEvolutionStatus.hpp"

using namespace gall;

namespace {
class MockGenotypeInitializer : public GenotypeInitializer<Genotype<int>> {
public:
    virtual void initialize(std::vector<int> &genes) const {
        for (int i = 0; i < 10; i++) {
            genes.emplace_back(i + rand() % 10);
        }
    }
};

class MockEvaluator : public Evaluator<Genotype<int>> {
public:
    virtual double evaluate(const Genotype<int> &genotype) const {
        return 0;
    }
};

class MockEliminationStrategy : public EliminationStrategy<Genotype<int>> {
public:
    virtual void eliminate(Population<Genotype<int>>& population) {
        // do nothing
    }
};

class MockBreedingOperator : public BreedingOperator<Genotype<int>> {
    virtual std::vector<Genotype<int>> breed(const Population<Genotype<int>>& pop) const {
        MockGenotypeInitializer initializer;
        Genotype<int> genotype(initializer);
        std::vector<Genotype<int>> mock{genotype};
        return mock;
    }
};

class MockCrossoverOperator : public CrossoverOperator<Genotype<int>> {
public:
    virtual Genotype<int> cross(std::vector<Genotype<int>>& genotypes) const {
        MockGenotypeInitializer initializer;
        Genotype<int> genotype(initializer);
        return genotype;
    }
};

class MockMutationOperator : public MutationOperator<Genotype<int>> {
public:
    virtual Genotype<int> mutate(Genotype<int> genotype) const {
        return genotype;
    }
};
}

SCENARIO("Evolving process uses objects defined by library users") {
    GIVEN("An EvolvingProcess instantiation") {
        EvolvingProcess<Genotype<int>> process(10);

        WHEN("Dependencies are injected") {
            process << new MockGenotypeInitializer << new MockEvaluator << new MockBreedingOperator <<
                    new MockEliminationStrategy << new MockCrossoverOperator <<
                    new MockMutationOperator;
            // great test...
            THEN("Nothing happens.") {
                process.evolve([](ObservableEvolutionStatus<Genotype<int>>& status) {
                    return true;
                });
            }
        }

        WHEN("All dependencies are missing") {
            THEN("Exception is thrown") {
                REQUIRE_THROWS(process.evolve([](ObservableEvolutionStatus<Genotype<int>>& status) {
                    return true;
                }));
            }
        }
    }
}

SCENARIO("Evolving process has user defined termination condition") {
    GIVEN("An EvolvingProcess instantiation") {
        EvolvingProcess<Genotype<int>> process(10);

        process << new MockGenotypeInitializer() << new MockEvaluator() << new MockBreedingOperator() <<
                new MockCrossoverOperator() << new MockMutationOperator() <<
                new MockEliminationStrategy();
        WHEN("Evolving process is run") {
            int generations = 0;
            process.evolve([&](ObservableEvolutionStatus<Genotype<int>>& status) -> bool {
                generations++;
                return status.getNumberOfGenerations() >= 10;
            });
            THEN("Evolving process quits at tenth generation") {
                REQUIRE(generations == 10);
            }
        }
    }
}

