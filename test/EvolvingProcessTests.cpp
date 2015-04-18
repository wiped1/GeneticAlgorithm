#include "Catch/catch.hpp"

#include "EvolvingProcess.hpp"
#include "GenotypeInitializer.hpp"
#include "Evaluator.hpp"
#include "SelectionStrategy.hpp"
#include "CrossoverStrategy.hpp"
#include "MutationStrategy.hpp"

namespace {
class MockGenotypeInitializer : public GenotypeInitializer<int> {
public:
    virtual void initialize(std::vector<int> &genes) const {
        for (int i = 0; i < 10; i++) {
            genes.emplace_back(0);
        }
    }
};

class MockEvaluator : public Evaluator<int> {
public:
    virtual double evaluate(Genotype<int> &genotype) const {
        return 0;
    }
};

class MockSelectionStrategy : public SelectionStrategy<int> {
public:
    virtual void eliminate(Population<int> &population, Ranking<int>::Type ranking) {
        // do nothing
    }
};

class MockCrossoverStrategy : public CrossoverStrategy<int> {
public:
    virtual void cross(Population<int> &population) const {
        // do nothing
    }
};

class MockMutationStrategy : public MutationStrategy<int> {
public:
    virtual void mutate(Population<int> &population) const {
        // do nothing
    }
};
}

SCENARIO("Evolving process uses objects defined by library users") {
    GIVEN("An EvolvingProcess instantiation") {
        EvolvingProcess<int> process(10);

        WHEN("Dependencies are injected") {
            process << new MockGenotypeInitializer << new MockEvaluator <<
                    new MockSelectionStrategy << new MockCrossoverStrategy <<
                    new MockMutationStrategy;
            // great test...
            THEN("Nothing happens.") {
                process.evolve([](auto pop, auto generations) {
                    return true;
                });
            }
        }

        WHEN("All dependencies are missing") {
            THEN("Exception is thrown") {
                REQUIRE_THROWS(process.evolve([](auto pop, auto generations) {
                    return true;
                }));
            }
        }
    }
}

SCENARIO("Evolving process has user defined termination condition") {
    GIVEN("An EvolvingProcess instantiation") {
        EvolvingProcess<int> process(10);

        process << new MockGenotypeInitializer() << new MockEvaluator() <<
                new MockCrossoverStrategy() << new MockMutationStrategy() <<
                new MockSelectionStrategy();
        WHEN("Evolving process is run") {
            process.evolve([](const Population<int>& pop,
                    unsigned int generations) -> bool {
                return generations >= 10;
            });
            THEN("Evolving process quits at tenth generation") {
                REQUIRE(process.getNumberOfGenerations() == 10);
            }
        }
    }
}

