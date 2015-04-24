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
        for (unsigned int i = 0; i < genotype.getGenes().size(); i++) {
            score += genotype.getGenes().at(i);
        }
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
                REQUIRE(pop.getGenotypes().size() == 2);
            }

            THEN("Genotypes that are left have highest scores") {
                REQUIRE(pop.getGenotypes().at(0).getGenes().at(0) == 3);
                REQUIRE(pop.getGenotypes().at(1).getGenes().at(0) == 4);
            }
        }
    }
    GIVEN("A population of 100 genotypes") {
        std::vector<Genotype<int>> genotypes;
        for (std::vector<Genotype<int>>::size_type i = 0; i < 100; i++) {
            genotypes.push_back(std::vector<int>{static_cast<int>(i)});
        }
        Population<int> pop {genotypes};
        WHEN("Population is evaluated") {
            IntGenotypeEvaluator evaluator;
            Ranking<int> ranking;
            DefaultSelectionStrategy<int> selectionStrategy;
            selectionStrategy.eliminate(pop, ranking.rank(pop, evaluator));

            THEN("Population size is exactly 50") {
                REQUIRE(pop.getGenotypes().size() == 50);
            }
        }
    }
    GIVEN("A population of 100 genotypes with same values") {
        std::vector<Genotype<int>> genotypes;
        for (std::vector<Genotype<int>>::size_type i = 0; i < 100; i++) {
            genotypes.push_back(std::vector<int>{0});
        }
        Population<int> pop {genotypes};
        WHEN("Population is evaluated") {
            IntGenotypeEvaluator evaluator;
            Ranking<int> ranking;
            DefaultSelectionStrategy<int> selectionStrategy;
            selectionStrategy.eliminate(pop, ranking.rank(pop, evaluator));

            THEN("Population size is exactly 50") {
                REQUIRE(pop.getGenotypes().size() == 50);
            }
        }
    }
}
