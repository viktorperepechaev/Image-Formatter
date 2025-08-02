#pragma once

#include "OperationEnforceInterface.hpp"

#include <random>
#include <vector>

class SteganographyDecodeOperation : public OperationEnforceInterface<SteganographyDecodeOperation> {
public:
  SteganographyDecodeOperation();

  void Apply(Image &image) const override;
  std::string GetName() const override;

  static bool ValidateArguments(const std::vector<std::string> &arguments);

  std::string GetDecodedMessage() const;

private:
  int GetRandomValue(int min, int max) const;

  mutable std::mt19937 rnd;
  mutable std::string decoded_message_;
};
