#pragma once

#include "OperationEnforceInterface.hpp"

class ReflectVerticallyOperation
    : public OperationEnforceInterface<ReflectVerticallyOperation> {
public:
  ReflectVerticallyOperation() = default;
  ReflectVerticallyOperation(const std::vector<std::string> &arguments);

  void Apply(Image &image) const override;

  static bool ValidateArguments(const std::vector<std::string> &arguments);
};
