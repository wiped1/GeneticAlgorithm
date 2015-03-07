#include "Catch/catch.hpp"

#include "EvolvingProcess.hpp"
#include "GenotypeInitializer.hpp"
#include "Evaluator.hpp"
#include "CrossoverStrategy.hpp"
#include "MutationStrategy.hpp"

namespace {
class MockGenotypeInitializer : public GenotypeInitializer<int> {
public:
    virtual void initialize(std::vector<int> &genes) const {
        // do nothing
    }
};

class MockEvaluator : public Evaluator<int> {
public:
    virtual double evaluate(Genotype<int> &genotype) const {
        return 0;
    }
};

class MockCrossoverStrategy : public CrossoverStrategy<int> {
public:
    virtual void cross(Population<int> &population) {
        // do nothing
    }
};

class MockMutationStrategy : public MutationStrategy<int> {
public:
    virtual void mutate(Population<int> &population) {

    }
};
}

SCENARIO("Evolving process uses objects defined by library users") {
    EvolvingProcess<int> process;

    GIVEN("A mock GenotypeInitializer, Evaluator, CrossoverStrategy, and MutationStrategy implementations") {
        MockGenotypeInitializer genotypeInitializer;
        MockEvaluator evaluator;
        MockCrossoverStrategy crossoverStrategy;
        MockMutationStrategy mutationStrategy;

        WHEN("Dependencies are injected") {
            process << genotypeInitializer << evaluator << crossoverStrategy << mutationStrategy;

            // great test...
            THEN("Nothing happens.") {
                // nothing
            }
        }
    }
}
