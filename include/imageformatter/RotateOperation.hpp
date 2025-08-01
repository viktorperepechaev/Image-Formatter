#pragma once

#include "OperationEnforceInterface.hpp"
#include "Rotate90DegreesClockwiseOperation.hpp"
#include "Rotate90DegreesCounterclockwiseOperation.hpp"

class RotateOperation : public OperationEnforceInterface<RotateOperation>,
                        public Rotate90DegreesClockwiseOperation,
                        public Rotate90DegreesCounterclockwiseOperation {
public:
  RotateOperation(const int rotation_degree);
  RotateOperation(const std::vector<std::string> &arguments);

  void Apply(Image &image) const override;

  static bool ValidateArguments(const std::vector<std::string> &arguments);

private:
  int rotation_degree_;
};
