#include "EvolvingProcess.hpp"

//template <typename T>
//EvolvingProcess<T>& EvolvingProcess<T>::operator<<(auto dependency) {
//    return use((GenotypeInitializer<T>)dependency);
//}

//template <typename T>
//EvolvingProcess<T>& EvolvingProcess<T>::operator<<(const GenotypeInitializer<T>& genotypeInitializer) {
//    return use(genotypeInitializer);
//}

//template <typename T>
//EvolvingProcess<T>& EvolvingProcess<T>::use(const GenotypeInitializer<T> &genotypeInitializer) {
//    _genotypeInitializer = std::unique_ptr<GenotypeInitializer<T>>(&genotypeInitializer);
//    return *this;
//}
//
//template <typename T>
//EvolvingProcess<T>& EvolvingProcess<T>::use(const Evaluator<T> &evaluator) {
//    _evaluator = std::unique_ptr<Evaluator<T>>(&evaluator);
//    return *this;
//}
//
//template <typename T>
//EvolvingProcess<T>& EvolvingProcess<T>::use(const CrossoverStrategy<T> &crossoverStrategy) {
//    _crossoverStrategy = std::unique_ptr<CrossoverStrategy<T>>(&crossoverStrategy);
//    return *this;
//}
//
//template <typename T>
//EvolvingProcess<T>& EvolvingProcess<T>::use(const MutationStrategy<T> &mutationStrategy) {
//    _mutationStrategy = std::unique_ptr<MutationStrategy<T>>(&mutationStrategy);
//    return *this;
//}
