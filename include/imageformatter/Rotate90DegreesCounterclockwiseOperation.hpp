#pragma once

#include "OperationEnforceInterface.hpp"

class Rotate90DegreesCounterclockwiseOperation
    : public OperationEnforceInterface<
          Rotate90DegreesCounterclockwiseOperation> {
public:
  Rotate90DegreesCounterclockwiseOperation() = default;
  Rotate90DegreesCounterclockwiseOperation(
      const std::vector<std::string> &arguments);

  void Apply(Image &image) const override;

  static bool ValidateArguments(const std::vector<std::string> &arguments);
};
