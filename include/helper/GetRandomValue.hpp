#pragma once

#include <random>

inline int GetRandomValue(std::mt19937& rnd, const int min, const int max) {  // Generates a random value from [min, max].
  return rnd() % (max - min + 1) + min;
}
