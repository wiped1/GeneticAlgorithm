#include "Catch/catch.hpp"

#include "SelectionStrategy.hpp"
#include "GenotypeInitializer.hpp"
#include "PopulationInitializer.hpp"
#include <iostream>

// anonymous namespace to prevent linker errors
namespace {
class IntGenotypeInitializer : public GenotypeInitializer<int> {
public:
    virtual void initialize(std::vector<int> &genes) {
        // 10 is the size of the genotype
        for (int i = 0; i < 10; i++) {
            genes.push_back(rand() % 10);
        }
    }
};

class IntGenotypeEvaluator : public Evaluator<int> {
public:
    virtual double evaluate(Genotype<int> &genotype) const {
        double score = 0;
        for (int i = 0; i < genotype.getGenes().size(); i++) {
            score += genotype.getGenes().at(i);
        }
        return score;
    }
};
}

SCENARIO("SelectionStrategy removes half of a Population that has lower fitness") {
    GIVEN("A population of genotypes with integer genes") {
        IntGenotypeInitializer initializer {};
        PopulationInitializer<int> popInitializer {initializer, 10};
        Population<int> pop {popInitializer};
        WHEN("Population is evaluated") {
            IntGenotypeEvaluator evaluator;
            SelectionStrategy<int> selector {evaluator};

            // TODO wywalić i zamienić na osobny test
            double scoreBefore[10];
            for ( int i = 0; i < 10; i++ ) {
                scoreBefore[i] = evaluator.evaluate(pop.getGenotypes().at(i));
            }

            selector.eliminate(pop);
            THEN("Population size has shrunken in half") {
                REQUIRE(pop.getGenotypes().size() == 5);
            }

            // TODO wywalić i zamienić na osobny test
            double scoreAfter[pop.getGenotypes().size()];
            for ( int i = 0; i < pop.getGenotypes().size(); i++ ) {
                scoreAfter[i] = evaluator.evaluate(pop.getGenotypes().at(i));
            }

            // TODO wywalić i zamienić na osobny test
            for ( int i = 0; i < 10; i++ ) {
                std::cout << scoreBefore[i] << ", ";
            }
            std::cout << std::endl;

            // TODO wywalić i zamienić na osobny test
            for ( int i = 0; i < pop.getGenotypes().size(); i++) {
                std::cout << scoreAfter[i] << ", ";
            }
            std::cout << std::endl;
        }
    }
}