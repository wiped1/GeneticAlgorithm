#pragma once

#include <functional>
#include "MutationFunctor.hpp"

template <typename Genotype>
struct NullMutationFunctor : public MutationFunctor<Genotype> {
    virtual void mutate(Genotype&) const {
        throw new std::runtime_error("NullMutationFunctor called, probably no MutationFunctors were added");
    }
};

template <typename Genotype, typename RandomEngine>
class MutationProbabilityDistribution {
/* private aliases */
private:
    using FunctionProbabilityPair = std::pair<std::unique_ptr<MutationFunctor<Genotype>>, double>;

/* private members */
private:
    /* TODO nullFunctor jest zwracany gdy wektor funktorów jest pusty
     * jest jakiś spośób, żeby zwrócić go w taki sposób (w funkcji roll)
     * żeby obiekt nie został usunięty?
     * const &&? */
    NullMutationFunctor<Genotype> nullFunctor;
    RandomEngine* rng;
    std::vector<FunctionProbabilityPair> dist;
    double distSum = 0;
/* public functions */
public:
    MutationProbabilityDistribution() = delete;
    MutationProbabilityDistribution(RandomEngine& rng);
    template <typename Functor>
    void add(double range);
    void add(MutationFunctor<Genotype>* functor, double range);
    const MutationFunctor<Genotype>& draw() const;
    const MutationFunctor<Genotype>& draw(double min, double max) const;
/* private functions */
private:
    inline const MutationFunctor<Genotype>& roll(std::uniform_real_distribution<double> d) const;
};

template <typename Genotype, typename RandomEngine>
MutationProbabilityDistribution<Genotype, RandomEngine>::MutationProbabilityDistribution(
        RandomEngine& rng) : rng(&rng) {
    // do nothing
}

template <typename Genotype, typename RandomEngine>
template <typename Functor>
void MutationProbabilityDistribution<Genotype, RandomEngine>::add(double range) {
    if (range > 0) {
        dist.emplace_back(FunctionProbabilityPair(std::unique_ptr<Functor>(new Functor), range));
        distSum += range;
    }
}

template <typename Genotype, typename RandomEngine>
void MutationProbabilityDistribution<Genotype, RandomEngine>::add(MutationFunctor<Genotype>* functor, double range) {
    if (range > 0) {
        dist.emplace_back(FunctionProbabilityPair(std::unique_ptr<MutationFunctor<Genotype>>(std::move(functor)), range));
        distSum += range;
    }
}

/*
 * roll generates random number in range specified by std::uniform_real_distribution,
 * and iterates through std::vector member returning MutationFunctor within roll range.
 *
 * For two MutationFunctors both with range 0.5 roll within range [0.0, 0.5] returns the first functor
 * and roll within range (0.5, 1.0] return the second functor.
 */
template <typename Genotype, typename RandomEngine>
inline const MutationFunctor<Genotype>& MutationProbabilityDistribution<Genotype, RandomEngine>::roll(
        std::uniform_real_distribution<double> d) const {
    double roll = d(*rng);
    double walkingSum = 0;
    /* TODO zamienić na std::find */
    for (auto it = dist.cbegin(); it != dist.cend(); it++) {
        walkingSum += it->second;
        if (walkingSum >= roll) {
            return *it->first;
        }
    }
    return nullFunctor;
}

template <typename Genotype, typename RandomEngine>
const MutationFunctor<Genotype>& MutationProbabilityDistribution<Genotype, RandomEngine>::draw() const {
    std::uniform_real_distribution<double> d(0, distSum);
    return roll(d);
}

template <typename Genotype, typename RandomEngine>
const MutationFunctor<Genotype>& MutationProbabilityDistribution<Genotype, RandomEngine>::draw(double min, double max) const {
    std::uniform_real_distribution<double> d(min, max);
    return roll(d);
}
