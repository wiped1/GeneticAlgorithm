#pragma once

#include <memory>

template <typename T>
class PolymorphicDependency {
private:
    std::unique_ptr<T> _dependency;
public:
    PolymorphicDependency();
    PolymorphicDependency(std::unique_ptr<T> dependency);
    void set(T* dependency);
    void set(std::unique_ptr<T> dependency);
    T* get();
};

template <typename T>
PolymorphicDependency<T>::PolymorphicDependency() : _dependency() {

}

template <typename T>
PolymorphicDependency<T>::PolymorphicDependency(std::unique_ptr<T> dependency) : _dependency(dependency) {

}

template <typename T>
void PolymorphicDependency<T>::set(T* dependency) {
    _dependency = std::unique_ptr<T>(dependency);
}

template <typename T>
void PolymorphicDependency<T>::set(std::unique_ptr<T> dependency) {
    _dependency = dependency;
}

template <typename T>
T* PolymorphicDependency<T>::get() {
    return _dependency.get();
}
