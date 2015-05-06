#include "Catch/catch.hpp"

#include <iostream>
#include "Population.hpp"
#include "DefaultSelectionStrategy.hpp"
#include "GenotypeInitializer.hpp"
#include "PopulationInitializer.hpp"
#include "Evaluator.hpp"

using namespace gall;

// anonymous namespace to prevent linker errors
namespace {
class IntGenotypeEvaluator : public Evaluator<int> {
public:
    virtual double evaluate(Genotype<int> &genotype) const {
        double score = 0;
        std::for_each(genotype.begin(), genotype.end(), [&](int value) {
            score += value;
        });
        return score;
    }
};
}

SCENARIO("SelectionStrategy removes half of a Population that has lower fitness") {
    GIVEN("A population of genotypes with integer genes") {
        Population<int>::CollectionType genotypes {
            std::pair<Genotype<int>, double>(Genotype<int>{std::vector<int>{0}}, 0),
            std::pair<Genotype<int>, double>(Genotype<int>{std::vector<int>{1}}, 0),
            std::pair<Genotype<int>, double>(Genotype<int>{std::vector<int>{2}}, 0),
            std::pair<Genotype<int>, double>(Genotype<int>{std::vector<int>{3}}, 0),
            std::pair<Genotype<int>, double>(Genotype<int>{std::vector<int>{4}}, 0),
        };
        Population<int> pop{genotypes};

        WHEN("Population is evaluated") {
            IntGenotypeEvaluator evaluator;
            DefaultSelectionStrategy<int> selectionStrategy;
            selectionStrategy.eliminate(pop);

            THEN("Population size has shrunken in half") {
                REQUIRE(std::distance(pop.begin(), pop.end()) == 2);
            }
        }
    }
    GIVEN("A population of 100 genotypes") {
        Population<int>::CollectionType genotypes;
        for (int i = 0; i < 100; i++) {
            genotypes.emplace(Genotype<int>{std::vector<int>{i}}, 0);
        }
        Population<int> pop {genotypes};
        WHEN("Population is evaluated") {
            IntGenotypeEvaluator evaluator;
            DefaultSelectionStrategy<int> selectionStrategy;
            selectionStrategy.eliminate(pop);

            THEN("Population size is exactly 50") {
                REQUIRE(std::distance(pop.begin(), pop.end()) == 50);
            }
        }
    }
    GIVEN("A population of 100 genotypes with same values") {
        Population<int>::CollectionType genotypes;
        for (int i = 0; i < 100; i++) {
            genotypes.emplace(std::vector<int>{i}, 0);
        }
        Population<int> pop {genotypes};
        WHEN("Population is evaluated") {
            IntGenotypeEvaluator evaluator;
            DefaultSelectionStrategy<int> selectionStrategy;
            selectionStrategy.eliminate(pop);

            THEN("Population size is exactly 50") {
                REQUIRE(std::distance(pop.begin(), pop.end()) == 50);
            }
        }
    }
}
