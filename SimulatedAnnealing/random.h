#pragma once
#include <cstdlib>

template <typename T>
T getRandom(T min, T max) {
    static const double fraction = 1.0 / (static_cast<double>(RAND_MAX) + 1.0);
    return static_cast<T>(rand() * fraction * (max - min) + min);
}
