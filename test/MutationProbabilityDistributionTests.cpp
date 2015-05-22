#include "Catch/catch.hpp"

#include <iostream>
#include <string>
#include <random>
#include "MutationProbabilityDistribution.hpp"
#include "MutationFunctor.hpp"

namespace {
struct MockMutationFunctorOne : MutationFunctor<std::string> {
    virtual void operator()(std::string& str) const {
        str = "mock1";
    }
};
struct MockMutationFunctorTwo : MutationFunctor<std::string> {
    virtual void operator()(std::string& str) const {
        str = "mock2";
    }
};
}

SCENARIO("MutationProbabilityDistribution is used to draw MutationFunctors from given vector") {
    GIVEN("MutationProbabilityDistribution object") {
        std::random_device rd;
        std::mt19937 rng(rd());
        MutationProbabilityDistribution<std::string, std::mt19937> dist(rng);
        WHEN("MockMutationFunctorOne is added") {
            dist.add<MockMutationFunctorOne>(0.5);

            THEN("Draw returns reference to object of type MockMutationFunctorOne") {
                /* dynamic_cast throws std::bad_cast when runtime check fails */
                CHECK_NOTHROW(dynamic_cast<const MockMutationFunctorOne&>(dist.draw()));
            }
            THEN("Returned reference to MockMutationFunctorOne is valid, and is able to change"
                 "empty string to 'mock1'") {
                std::string str;
                dist.draw()(str);
                REQUIRE(str == "mock1");
            }
        }
        WHEN("No MutationFunctor is added, and draw is called") {
            THEN("Exception is thrown") {
                std::string str;
                REQUIRE_THROWS(dist.draw()(str));
            }
        }
    }
    GIVEN("MutationProbabilityDistribution object, and two mutation functors") {
        std::random_device rd;
        std::mt19937 rng(rd());
        MutationProbabilityDistribution<std::string, std::mt19937> dist(rng);
        WHEN("Two MutationFunctors are added") {
            dist.add<MockMutationFunctorOne>(0.5);
            dist.add<MockMutationFunctorTwo>(0.5);
            THEN("When roll is 0.0 MutationFunctorOne is called") {
                CHECK_NOTHROW(dynamic_cast<const MockMutationFunctorOne&>(dist.draw(0.0, 0.0)));
            }
            THEN("When roll is 0.5 MutationFunctorOne is called") {
                CHECK_NOTHROW(dynamic_cast<const MockMutationFunctorOne&>(dist.draw(0.5, 0.5)));
            }
            THEN("When roll is 0.51 MutationFunctorTwo is called") {
                CHECK_NOTHROW(dynamic_cast<const MockMutationFunctorTwo&>(dist.draw(0.51, 0.51)));
            }
            THEN("When roll is 1.0 MutationFunctorTwo is called") {
                CHECK_NOTHROW(dynamic_cast<const MockMutationFunctorTwo&>(dist.draw(1.0, 1.0)));
            }
        }
    }
}
