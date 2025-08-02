#pragma once

#include <stdexcept>
#include <string>

inline std::string BinaryToString(const std::string& binary_message) {
  if (binary_message.empty() || binary_message.size() % 8 != 0) {
    throw std::runtime_error("Invalid binary_message with size: " + std::to_string(binary_message.size()));
  }

  std::string result;

  for (size_t index = 0; index < binary_message.size(); index += 8) {
    result += static_cast<char>(std::stoi(binary_message.substr(index, 8), nullptr, 2));
  }
  
  return result;
}
