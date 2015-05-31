#include "Catch/catch.hpp"

#include "Genotype.hpp"
#include "Population.hpp"
#include "PopulationInitializer.hpp"
#include "GenotypeInitializer.hpp"
#include "Evaluator.hpp"

using namespace gall;

class MockEvaluator : public Evaluator<Genotype<std::vector<int>>> {
public:
    double evaluate(const Genotype<std::vector<int>> &genotype) const {
        return *genotype.asCollection().cbegin();
    }
};

class MockGenotypeInitializer : public GenotypeInitializer<Genotype<std::vector<int>>> {
public:
    void initialize(Genotype<std::vector<int>>::CollectionType &genes) const {
        // do nothing
    };
};

SCENARIO("Population is used to store genotypes") {
    GIVEN("A population and vector of newly bred genotypes") {
        MockEvaluator evaluator;
        MockGenotypeInitializer genotypeInitializer;
        PopulationInitializer<Genotype<std::vector<int>>> initializer{genotypeInitializer, 0};
        Population<Genotype<std::vector<int>>> pop{initializer, evaluator};

        //precheck
        REQUIRE(pop.asCollection().size() == 0);

        // vector of genotypes that are going to be added to population
        std::vector<Genotype<std::vector<int>>> newGenotypes;
        newGenotypes.push_back(Genotype<std::vector<int>>{std::vector<int>{0}});
        newGenotypes.push_back(Genotype<std::vector<int>>{std::vector<int>{1}});
        newGenotypes.push_back(Genotype<std::vector<int>>{std::vector<int>{2}});
        newGenotypes.push_back(Genotype<std::vector<int>>{std::vector<int>{3}});
        WHEN("New genotypes are inserted to population") {
            pop.insert(newGenotypes.begin(), newGenotypes.end());

            THEN("Genotypes were added to the population") {
                // check if genotypes store the same value
                REQUIRE(*pop.asCollection().begin()->first.asCollection().cbegin() == 0);
                REQUIRE(*std::next(pop.asCollection().begin(), 1)->first.asCollection().cbegin() == 1);
                REQUIRE(*std::next(pop.asCollection().begin(), 2)->first.asCollection().cbegin() == 2);
                REQUIRE(*std::next(pop.asCollection().begin(), 3)->first.asCollection().cbegin() == 3);
                // check if fitness is the same
                REQUIRE(pop.asCollection().begin()->second == 0);
                REQUIRE(std::next(pop.asCollection().begin(), 1)->second == 1);
                REQUIRE(std::next(pop.asCollection().begin(), 2)->second == 2);
                REQUIRE(std::next(pop.asCollection().begin(), 3)->second == 3);
            }
        }
    }
}
