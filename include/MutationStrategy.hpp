#pragma once

class MutationStrategy {
public:
    virtual ~MutationStrategy() = default;
    virtual void mutate(const Population &population);
};