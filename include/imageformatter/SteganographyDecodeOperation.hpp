#pragma once

#include <random>
#include <vector>

#include "OperationEnforceInterface.hpp"

class SteganographyDecodeOperation : public OperationEnforceInterface<SteganographyDecodeOperation> {
public:
  SteganographyDecodeOperation();

  void Apply(Image &image) const override;
  std::string GetName() const override;

  static bool ValidateArguments(const std::vector<std::string> &arguments);

private:
  mutable std::mt19937 rnd;
};
