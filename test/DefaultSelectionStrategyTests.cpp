#include "Catch/catch.hpp"

#include <iostream>
#include "Population.hpp"
#include "DefaultSelectionStrategy.hpp"
#include "GenotypeInitializer.hpp"
#include "PopulationInitializer.hpp"
#include "Evaluator.hpp"

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
        std::vector<Genotype<int>> genotypes {
            Genotype<int>{std::vector<int>{0}},
            Genotype<int>{std::vector<int>{1}},
            Genotype<int>{std::vector<int>{2}},
            Genotype<int>{std::vector<int>{3}},
            Genotype<int>{std::vector<int>{4}}
        };
        Population<int> pop {genotypes};

        WHEN("Population is evaluated") {
            IntGenotypeEvaluator evaluator;
            Ranking<int> ranking;
            DefaultSelectionStrategy<int> selectionStrategy;
            selectionStrategy.eliminate(pop, ranking.rank(pop, evaluator));

            THEN("Population size has shrunken in half") {
                REQUIRE(std::distance(pop.begin(), pop.end()) == 2);
            }

            THEN("Genotypes that are left have highest scores") {
                auto it = pop.begin();
                REQUIRE(*((*pop.begin()).begin()) == 4);
                REQUIRE(*((*(pop.begin() + 1)).begin()) == 3);
            }
        }
    }
    GIVEN("A population of 100 genotypes") {
        std::vector<Genotype<int>> genotypes;
        for (std::vector<Genotype<int>>::size_type i = 0; i < 100; i++) {
            genotypes.emplace_back(std::vector<int>{static_cast<int>(i)});
        }
        Population<int> pop {genotypes};
        WHEN("Population is evaluated") {
            IntGenotypeEvaluator evaluator;
            Ranking<int> ranking;
            DefaultSelectionStrategy<int> selectionStrategy;
            selectionStrategy.eliminate(pop, ranking.rank(pop, evaluator));

            THEN("Population size is exactly 50") {
                REQUIRE(std::distance(pop.begin(), pop.end()) == 50);
            }
        }
    }
    GIVEN("A population of 100 genotypes with same values") {
        std::vector<Genotype<int>> genotypes;
        for (std::vector<Genotype<int>>::size_type i = 0; i < 100; i++) {
            genotypes.emplace_back(std::vector<int>{0});
        }
        Population<int> pop {genotypes};
        WHEN("Population is evaluated") {
            IntGenotypeEvaluator evaluator;
            Ranking<int> ranking;
            DefaultSelectionStrategy<int> selectionStrategy;
            selectionStrategy.eliminate(pop, ranking.rank(pop, evaluator));

            THEN("Population size is exactly 50") {
                REQUIRE(std::distance(pop.begin(), pop.end()) == 50);
            }
        }
    }
}
