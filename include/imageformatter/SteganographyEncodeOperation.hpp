#pragma once

#include "OperationEnforceInterface.hpp"

#include <random>
#include <vector>

class SteganographyEncodeOperation : public OperationEnforceInterface<SteganographyEncodeOperation> {
public:
  SteganographyEncodeOperation(const std::string& secret_message);
  SteganographyEncodeOperation(const std::vector<std::string> &arguments);

  void Apply(Image &image) const override;
  std::string GetName() const override;

  static bool ValidateArguments(const std::vector<std::string> &arguments);

private:
  int GetRandomValue(int min, int max) const;

  mutable std::mt19937 rnd;
  std::string secret_message_;
};
