#pragma once

#include "OperationEnforceInterface.hpp"

class InvertOperation : public OperationEnforceInterface<InvertOperation> {
public:
  InvertOperation() = default;
  InvertOperation(const std::vector<std::string> &arguments);

  void Apply(Image &image) const override;
  std::string GetName() const override;

  static bool ValidateArguments(const std::vector<std::string> &arguments);

private:
};
