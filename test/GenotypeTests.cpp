#include "Catch/catch.hpp"

// include to test compilation
// TODO remove
#include "GeneticAlgorithm.hpp"
#include "Genotype.hpp"
#include <vector>

SCENARIO("Genotype can be constructed") {
    GIVEN("A vector of integers") {
        std::vector<int> vec{1, 2, 3, 4, 5};

        WHEN("Genotype is constructed") {
            Genotype<int> genotype(vec);
            THEN("Genes match given vector") {
                REQUIRE(genotype.getGenes() == vec);
            }
        }
    }
}