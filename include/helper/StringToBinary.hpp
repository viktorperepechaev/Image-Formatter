#pragma once

#include <string>
#include <bitset>

inline std::string CharToBinary(const char symbol) {
  return std::bitset<8>(symbol).to_string();
}

inline std::string StringToBinary(const std::string& message) {
  std::string result;

  for (const auto& symbol : message) {
    result += CharToBinary(symbol);
  }

  return result;
}
