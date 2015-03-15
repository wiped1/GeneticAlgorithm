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
    EvolvingProcess<int> process(10);

    GIVEN("A mock GenotypeInitializer, Evaluator, CrossoverStrategy, and MutationStrategy implementations") {
        MockGenotypeInitializer genotypeInitializer;
        MockEvaluator evaluator;
        MockSelectionStrategy selectionStrategy;
        MockCrossoverStrategy crossoverStrategy;
        MockMutationStrategy mutationStrategy;

        WHEN("Dependencies are injected") {
            process << genotypeInitializer << evaluator << crossoverStrategy <<
                    mutationStrategy << selectionStrategy;

            /*
                process.setTerminationCondition([]() {
                    return
                });
             */
            // great test...
            THEN("Nothing happens.") {
                process.evolve([](auto pop, auto generations) {
                    return true;
                });
            }
        }
    }
}

SCENARIO("Evolving process has user defined termination condition") {
    EvolvingProcess<int> process(10);

    GIVEN("A mock GenotypeInitializer, Evaluator, CrossoverStrategy, and MutationStrategy implementations") {
        MockGenotypeInitializer genotypeInitializer;
        MockEvaluator evaluator;
        MockSelectionStrategy selectionStrategy;
        MockCrossoverStrategy crossoverStrategy;
        MockMutationStrategy mutationStrategy;

        process << genotypeInitializer << evaluator << crossoverStrategy <<
                mutationStrategy << selectionStrategy;
        WHEN("Condition function is set") {
            THEN("Evolving process quits at tenth generation") {
                process.evolve([](const Population<int>& pop,
                        unsigned int generations) -> bool {
                    return generations >= 10;
                });
                REQUIRE(process.getNumberOfGenerations() == 10);
            }
        }
    }
}

