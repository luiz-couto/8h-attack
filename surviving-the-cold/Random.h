#pragma once

#include <random>

class RandomInt {
    private:
    std::uniform_int_distribution<int> distribution;
    std::mt19937 mt;

    public:
    RandomInt(int maxValue) {
        std::random_device rd;
        this->mt = std::mt19937(rd());
        this->distribution = std::uniform_int_distribution<int>(0, maxValue);
    }

    int generate() {
        return this->distribution(this->mt);
    }
};