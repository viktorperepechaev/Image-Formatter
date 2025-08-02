#include "../../include/imageformatter/RotateOperation.hpp"

RotateOperation::RotateOperation(const int rotation_degree)
    : rotation_degree_(rotation_degree) {}

RotateOperation::RotateOperation(const std::vector<std::string> &arguments)
    : rotation_degree_(std::stoi(arguments[0])) {}

void RotateOperation::Apply(Image &image) const {
  if (rotation_degree_ == 0) {
    return;
  }

  const bool is_positive = (rotation_degree_ > 0);

  const int normalized_rotation_degree = std::abs(rotation_degree_) % 360;

  if (is_positive) {
    for (int counter = 0; counter < normalized_rotation_degree / 90;
         ++counter) {
      Rotate90DegreesClockwiseOperation::Apply(image);
    }
  } else {
    for (int counter = 0; counter < normalized_rotation_degree / 90;
         ++counter) {
      Rotate90DegreesCounterclockwiseOperation::Apply(image);
    }
  }
}

std::string RotateOperation::GetName() const { return "rotate"; }

bool RotateOperation::ValidateArguments(
    const std::vector<std::string> &arguments) {
  return arguments.size() == 1 &&
         arguments[0].find_first_not_of("-0123456789") == std::string::npos &&
         std::stoi(arguments[0]) % 90 == 0;
}
