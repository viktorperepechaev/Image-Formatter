#include "../../include/imageformatter/ReflectHorizontallyOperation.hpp"

ReflectHorizontallyOperation::ReflectHorizontallyOperation(const std::vector<std::string>& arguments) : ReflectHorizontallyOperation() {}

void ReflectHorizontallyOperation::Apply(Image& image) const {
  for (size_t current_line = 0; current_line < image.GetHeight() / 2; ++current_line) {
    for (size_t current_column = 0; current_column < image.GetWidth(); ++current_column) {
      auto current_pixel_nonalpha_values = image.GetNonAlphaPixelValues(current_line, current_column);
      auto horizontally_symmetrical_pixel_nonalpha_values = image.GetNonAlphaPixelValues(image.GetHeight() - current_line - 1, current_column);

      std::swap(current_pixel_nonalpha_values, horizontally_symmetrical_pixel_nonalpha_values);

      image.ApplyNewPixelValues(current_line, current_column, current_pixel_nonalpha_values);
      image.ApplyNewPixelValues(image.GetHeight() - current_line - 1, current_column, horizontally_symmetrical_pixel_nonalpha_values);
    }
  } 
};

bool ReflectHorizontallyOperation::ValidateArguments(const std::vector<std::string> &arguments) {
  return arguments.empty();
}
