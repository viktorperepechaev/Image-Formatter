#pragma once

#include "OperationEnforceInterface.hpp"

class InvertOperation : public OperationEnforceInterface<InvertOperation> {
 public:
  InvertOperation() = default;

  void Apply(Image& image) const override;
 
  static bool ValidateArguments(const std::vector<std::string>& arguments);
 private:
};
