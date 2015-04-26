#include "Catch/catch.hpp"

#include <vector>
#include "Genotype.hpp"
#include "Population.hpp"
#include "EvolutionStatus.hpp"

SCENARIO("EvolutionStatus is used in safe representation of current evolution data") {
    GIVEN("Initialized EvolutionStatus") {
        std::vector<int> vec = {0};
        Genotype<int> genotype(vec);
        std::vector<Genotype<int>> genotypes = {genotype};
        Population<int> pop(genotypes);
        EvolutionStatus<int> status(pop);

        // precheck genotype member equals nullptr
        REQUIRE(&(status.getGenotypeWithBestFitness()) == nullptr);

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
        WHEN("Best genotype is set") {
            Genotype<int> newGenotype(std::vector<int>({0}));
            status.setGenotypeWithBestFitness(newGenotype);

            THEN("EvolutionStatus genotype has the same address as local variable") {
                REQUIRE(&(status.getGenotypeWithBestFitness()) == &newGenotype);
            }
        }
        WHEN("Population size is queried") {
            THEN("It's size is proper") {
                REQUIRE(status.getPopulationSize() == 1);
            }
        }
    }
}
