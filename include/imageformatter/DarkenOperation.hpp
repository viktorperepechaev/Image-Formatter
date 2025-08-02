#pragma once

#include "OperationEnforceInterface.hpp"

class DarkenOperation : public OperationEnforceInterface<DarkenOperation> {
public:
  DarkenOperation(const std::uint8_t percent);
  DarkenOperation(const std::vector<std::string> &arguments);

  void Apply(Image &image) const override;
  std::string GetName() const override;

  static bool ValidateArguments(const std::vector<std::string> &arguments);

private:
  double needed_share_;
};
