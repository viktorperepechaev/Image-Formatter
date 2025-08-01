#pragma once

#include "OperationEnforceInterface.hpp"

class ReflectHorizontallyOperation
    : public OperationEnforceInterface<ReflectHorizontallyOperation> {
public:
  ReflectHorizontallyOperation() = default;
  ReflectHorizontallyOperation(const std::vector<std::string> &arguments);

  void Apply(Image &image) const override;

  static bool ValidateArguments(const std::vector<std::string> &arguments);
};
