#pragma once

#include "OperationEnforceInterface.hpp"

class Rotate90DegreesClockwiseOperation
    : public OperationEnforceInterface<Rotate90DegreesClockwiseOperation> {
public:
  Rotate90DegreesClockwiseOperation() = default;
  Rotate90DegreesClockwiseOperation(const std::vector<std::string> &arguments);

  void Apply(Image &image) const override;

  static bool ValidateArguments(const std::vector<std::string> &arguments);
};
