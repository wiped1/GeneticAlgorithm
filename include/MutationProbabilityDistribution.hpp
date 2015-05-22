#pragma once

#include <functional>
#include "MutationFunctor.hpp"

template <typename T>
struct NullMutationFunctor : public MutationFunctor<T> {
    virtual void operator()(T&) const {
        throw new std::runtime_error("NullMutationFunctor called, probably no MutationFunctors were added");
    }
};

template <typename T, typename RandomEngine>
class MutationProbabilityDistribution {
/* private aliases */
private:
    using FunctionProbabilityPair = std::pair<std::unique_ptr<MutationFunctor<T>>, double>;

/* private members */
private:
    /* TODO nullFunctor jest zwracany gdy wektor funktorów jest pusty
     * jest jakiś spośób, żeby zwrócić go w taki sposób (w funkcji roll)
     * żeby obiekt nie został usunięty?
     * const &&? */
    NullMutationFunctor<T> nullFunctor;
    RandomEngine* rng;
    std::vector<FunctionProbabilityPair> dist;
    double distSum = 0;
/* public functions */
public:
    MutationProbabilityDistribution() = delete;
    MutationProbabilityDistribution(RandomEngine& rng);
    template <typename Functor>
    bool add(double range);
    const MutationFunctor<T>& draw() const;
    const MutationFunctor<T>& draw(double min, double max) const;
/* private functions */
private:
    inline const MutationFunctor<T>& roll(std::uniform_real_distribution<double> d) const;
};

template <typename T, typename RandomEngine>
MutationProbabilityDistribution<T, RandomEngine>::MutationProbabilityDistribution(
        RandomEngine& rng) : rng(&rng) {
    // do nothing
}

template <typename T, typename RandomEngine>
template <typename Functor>
bool MutationProbabilityDistribution<T, RandomEngine>::add(double range) {
    dist.emplace_back(FunctionProbabilityPair(std::unique_ptr<Functor>(new Functor), range));
    distSum += range;
}

/*
 * roll generates random number in range specified by std::uniform_real_distribution,
 * and iterates through std::vector member returning MutationFunctor within roll range.
 *
 * For two MutationFunctors both with range 0.5 roll within range [0.0, 0.5] returns the first functor
 * and roll within range (0.5, 1.0] return the second functor.
 */
template <typename T, typename RandomEngine>
inline const MutationFunctor<T>& MutationProbabilityDistribution<T, RandomEngine>::roll(
        std::uniform_real_distribution<double> d) const {
    double roll = d(*rng);
    double walkingSum = 0;
    for (auto it = dist.cbegin(); it != dist.cend(); it++) {
        if (walkingSum <= roll && walkingSum + it->second >= roll) {
            return *it->first;
        }
        walkingSum += it->second;
    }
    return nullFunctor;
}

template <typename T, typename RandomEngine>
const MutationFunctor<T>& MutationProbabilityDistribution<T, RandomEngine>::draw() const {
    std::uniform_real_distribution<double> d(0, distSum);
    return roll(d);
}

template <typename T, typename RandomEngine>
const MutationFunctor<T>& MutationProbabilityDistribution<T, RandomEngine>::draw(double min, double max) const {
    std::uniform_real_distribution<double> d(min, max);
    return roll(d);
}
