#pragma once

class CrossoverStrategy {
public:
    virtual ~CrossoverStrategy() = default;
    virtual void cross(const Popuplation &population) = 0;
};