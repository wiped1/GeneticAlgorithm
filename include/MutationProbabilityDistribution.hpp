#pragma once

#include <functional>
#include "MutationFunctor.hpp"

template <typename Genotype, typename Gene>
struct NullMutationFunctor : public MutationFunctor<Genotype, Gene> {
    virtual void operator()(Genotype&, Gene&) const {
        throw new std::runtime_error("NullMutationFunctor called, probably no MutationFunctors were added");
    }
};

template <typename Genotype, typename Gene, typename RandomEngine>
class MutationProbabilityDistribution {
/* private aliases */
private:
    using FunctionProbabilityPair = std::pair<std::unique_ptr<MutationFunctor<Genotype, Gene>>, double>;

/* private members */
private:
    /* TODO nullFunctor jest zwracany gdy wektor funktorów jest pusty
     * jest jakiś spośób, żeby zwrócić go w taki sposób (w funkcji roll)
     * żeby obiekt nie został usunięty?
     * const &&? */
    NullMutationFunctor<Genotype, Gene> nullFunctor;
    RandomEngine* rng;
    std::vector<FunctionProbabilityPair> dist;
    double distSum = 0;
/* public functions */
public:
    MutationProbabilityDistribution() = delete;
    MutationProbabilityDistribution(RandomEngine& rng);
    template <typename Functor>
    void add(double range);
    void add(MutationFunctor<Genotype, Gene>* functor, double range);
    const MutationFunctor<Genotype, Gene>& draw() const;
    const MutationFunctor<Genotype, Gene>& draw(double min, double max) const;
/* private functions */
private:
    inline const MutationFunctor<Genotype, Gene>& roll(std::uniform_real_distribution<double> d) const;
};

template <typename Genotype, typename Gene, typename RandomEngine>
MutationProbabilityDistribution<Genotype, Gene, RandomEngine>::MutationProbabilityDistribution(
        RandomEngine& rng) : rng(&rng) {
    // do nothing
}

template <typename Genotype, typename Gene, typename RandomEngine>
template <typename Functor>
void MutationProbabilityDistribution<Genotype, Gene, RandomEngine>::add(double range) {
    dist.emplace_back(FunctionProbabilityPair(std::unique_ptr<Functor>(new Functor), range));
    distSum += range;
}

template <typename Genotype, typename Gene, typename RandomEngine>
void MutationProbabilityDistribution<Genotype, Gene, RandomEngine>::add(MutationFunctor<Genotype, Gene>* functor, double range) {
    dist.emplace_back(FunctionProbabilityPair(std::unique_ptr<MutationFunctor<Genotype, Gene>>(std::move(functor)), range));
    distSum += range;
}

/*
 * roll generates random number in range specified by std::uniform_real_distribution,
 * and iterates through std::vector member returning MutationFunctor within roll range.
 *
 * For two MutationFunctors both with range 0.5 roll within range [0.0, 0.5] returns the first functor
 * and roll within range (0.5, 1.0] return the second functor.
 */
template <typename Genotype, typename Gene, typename RandomEngine>
inline const MutationFunctor<Genotype, Gene>& MutationProbabilityDistribution<Genotype, Gene, RandomEngine>::roll(
        std::uniform_real_distribution<double> d) const {
    double roll = d(*rng);
    double walkingSum = 0;
    /* TODO zamienić na std::find */
    for (auto it = dist.cbegin(); it != dist.cend(); it++) {
        if (walkingSum <= roll && walkingSum + it->second >= roll) {
            return *it->first;
        }
        walkingSum += it->second;
    }
    return nullFunctor;
}

template <typename Genotype, typename Gene, typename RandomEngine>
const MutationFunctor<Genotype, Gene>& MutationProbabilityDistribution<Genotype, Gene, RandomEngine>::draw() const {
    std::uniform_real_distribution<double> d(0, distSum);
    return roll(d);
}

template <typename Genotype, typename Gene, typename RandomEngine>
const MutationFunctor<Genotype, Gene>& MutationProbabilityDistribution<Genotype, Gene, RandomEngine>::draw(double min, double max) const {
    std::uniform_real_distribution<double> d(min, max);
    return roll(d);
}
