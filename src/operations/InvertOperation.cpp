#include "../../include/imageformatter/InvertOperation.hpp"

InvertOperation::InvertOperation(const std::vector<std::string>& arguments) : InvertOperation() {}

void InvertOperation::Apply(Image& image) const {
  for (size_t current_line = 0; current_line < image.GetHeight(); ++current_line) {
    for (size_t current_column = 0; current_column < image.GetWidth(); ++current_column) {
      auto nonalpha_pixel_values = image.GetNonAlphaPixelValues(current_line, current_column);

      for (auto& value : nonalpha_pixel_values) {
        value = Image::Constants::kMaxColorValue - value;
      }

      image.ApplyNewPixelValues(current_line, current_column, nonalpha_pixel_values);
    }
  }
}

bool InvertOperation::ValidateArguments(const std::vector<std::string>& arguments) {
  return arguments.empty();
}
