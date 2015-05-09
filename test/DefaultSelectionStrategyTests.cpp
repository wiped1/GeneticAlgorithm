#include "Catch/catch.hpp"

#include <iostream>
#include "Population.hpp"
#include "DefaultEliminationStrategy.hpp"
#include "GenotypeInitializer.hpp"
#include "PopulationInitializer.hpp"
#include "Evaluator.hpp"

using namespace gall;

// anonymous namespace to prevent linker errors
namespace {
class IntGenotypeEvaluator : public Evaluator<Genotype<int>> {
public:
    virtual double evaluate(const Genotype<int> &genotype) const {
        double score = 0;
        std::for_each(genotype.cbegin(), genotype.cend(), [&](int value) {
            score += value;
        });
        return score;
    }
};
}

SCENARIO("EliminationStrategy removes half of a Population that has lower fitness") {
    GIVEN("A population of genotypes with integer genes with uneven number of genotypes") {
        Population<Genotype<int>>::CollectionType genotypes;
        genotypes.insert(std::pair<Genotype<int>, double>(
                Genotype<int>{std::vector<int>{0}}, 0));
        genotypes.insert(std::pair<Genotype<int>, double>(
                Genotype<int>{std::vector<int>{1}}, 0));
        genotypes.insert(std::pair<Genotype<int>, double>(
                Genotype<int>{std::vector<int>{2}}, 0));
        genotypes.insert(std::pair<Genotype<int>, double>(
                Genotype<int>{std::vector<int>{3}}, 0));
        genotypes.insert(std::pair<Genotype<int>, double>(
                Genotype<int>{std::vector<int>{4}}, 0));
        Population<Genotype<int>> pop{genotypes};

        WHEN("Population is evaluated") {
            IntGenotypeEvaluator evaluator;
            DefaultEliminationStrategy<Genotype<int>> selectionStrategy;
            selectionStrategy.eliminate(pop);

            THEN("Population size has shrunken in half") {
                REQUIRE(std::distance(pop.begin(), pop.end()) == 2);
            }
            THEN("Genotypes that were left are sorted in correct order") {
                auto it = pop.begin();
                REQUIRE(*(*it).first.cbegin() == 3);
                it = std::next(it);
                REQUIRE(*(*it).first.cbegin() == 4);
            }
        }
    }
    GIVEN("A population of genotypes with integer genes with even number of genotypes") {
        Population<Genotype<int>>::CollectionType genotypes;
        genotypes.insert(std::pair<Genotype<int>, double>(
                Genotype<int>{std::vector<int>{0}}, 0));
        genotypes.insert(std::pair<Genotype<int>, double>(
                Genotype<int>{std::vector<int>{1}}, 0));
        genotypes.insert(std::pair<Genotype<int>, double>(
                Genotype<int>{std::vector<int>{2}}, 0));
        genotypes.insert(std::pair<Genotype<int>, double>(
                Genotype<int>{std::vector<int>{3}}, 0));
        genotypes.insert(std::pair<Genotype<int>, double>(
                Genotype<int>{std::vector<int>{4}}, 0));
        genotypes.insert(std::pair<Genotype<int>, double>(
                Genotype<int>{std::vector<int>{5}}, 0));
        Population<Genotype<int>> pop{genotypes};

        WHEN("Population is evaluated") {
            IntGenotypeEvaluator evaluator;
            DefaultEliminationStrategy<Genotype<int>> selectionStrategy;
            selectionStrategy.eliminate(pop);

            THEN("Population size has shrunken in half") {
                REQUIRE(std::distance(pop.begin(), pop.end()) == 3);
            }
            THEN("Genotypes that were left are sorted in correct order") {
                auto it = pop.begin();
                REQUIRE(*(*it).first.cbegin() == 3);
                it = std::next(it);
                REQUIRE(*(*it).first.cbegin() == 4);
                it = std::next(it);
                REQUIRE(*(*it).first.cbegin() == 5);
            }
        }
    }
    GIVEN("A population of 100 genotypes") {
        Population<Genotype<int>>::CollectionType genotypes;
        for (int i = 0; i < 100; i++) {
            genotypes.emplace(Genotype<int>{std::vector<int>{i}}, 0);
        }
        Population<Genotype<int>> pop {genotypes};
        WHEN("Population is evaluated") {
            IntGenotypeEvaluator evaluator;
            DefaultEliminationStrategy<Genotype<int>> selectionStrategy;
            selectionStrategy.eliminate(pop);

            THEN("Population size is exactly 50") {
                REQUIRE(std::distance(pop.begin(), pop.end()) == 50);
            }
        }
    }
    GIVEN("A population of 100 genotypes with same values") {
        Population<Genotype<int>>::CollectionType genotypes;
        for (int i = 0; i < 100; i++) {
            genotypes.emplace(std::vector<int>{i}, 0);
        }
        Population<Genotype<int>> pop {genotypes};
        WHEN("Population is evaluated") {
            IntGenotypeEvaluator evaluator;
            DefaultEliminationStrategy<Genotype<int>> selectionStrategy;
            selectionStrategy.eliminate(pop);

            THEN("Population size is exactly 50") {
                REQUIRE(std::distance(pop.begin(), pop.end()) == 50);
            }
        }
    }
}
