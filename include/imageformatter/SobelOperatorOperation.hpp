#pragma once

#include "OperationEnforceInterface.hpp"

class SobelOperatorOperation
    : public OperationEnforceInterface<SobelOperatorOperation> {
public:
  SobelOperatorOperation() = default;
  SobelOperatorOperation(const std::vector<std::string> &arguments);

  void Apply(Image &image) const override;

  static bool ValidateArguments(const std::vector<std::string> &arguments);
};
