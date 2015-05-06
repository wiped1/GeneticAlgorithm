#include "Catch/catch.hpp"

#include "Genotype.hpp"
#include "Population.hpp"
#include "EvolutionStatus.hpp"

using namespace gall;

SCENARIO("EvolutionStatus is used in safe representation of current evolution data") {
    GIVEN("Initialized EvolutionStatus") {
        std::vector<int> vec = {0};
        Genotype<int> genotype(vec);
        Population<int>::CollectionType genotypes;
        genotypes.emplace(Population<int>::GenotypeType(genotype, 0));
        Population<int> pop(genotypes);
        EvolutionStatus<int> status(pop);

        WHEN("Number of generations is incremented") {
            // precheck
            REQUIRE(status.getNumberOfGenerations() == 0);
            status.incrementNumberOfGenerations();

            THEN("It equals 1") {
                REQUIRE(status.getNumberOfGenerations() == 1);
            }
        }
        WHEN("Current positive fitness is higher") {
            // precheck
            REQUIRE(status.getHighestFitness() == 0);
            status.updateFitness(100);

            THEN("Best fitness equals 100, fitness delta equals 100") {
                REQUIRE(status.getHighestFitness() == 100);
                REQUIRE(status.getFitnessDelta() == 100);
            }
        }
        WHEN("Current positive fitness is lower") {
            status.updateFitness(100);
            status.updateFitness(0);

            THEN("Best fitness equals 0, fitness delta equals -100") {
                REQUIRE(status.getHighestFitness() == 0);
                REQUIRE(status.getFitnessDelta() == -100);
            }
        }
        WHEN("Current negative fitness is higher") {
            status.updateFitness(-100);
            status.updateFitness(0);

            THEN("Best fitness equals 0, fitness delta equals 100") {
                REQUIRE(status.getHighestFitness() == 0);
                REQUIRE(status.getFitnessDelta() == 100);
            }
        }
        WHEN("Current negative fitness is lower") {
            status.updateFitness(-100);
            status.updateFitness(-200);

            THEN("Best fitness equals -200, fitness delta equals -100") {
                REQUIRE(status.getHighestFitness() == -200);
                REQUIRE(status.getFitnessDelta() == -100);
            }
        }
        WHEN("Population size is queried") {
            THEN("It's size is proper") {
                REQUIRE(status.getPopulationSize() == 1);
            }
        }
    }
}
