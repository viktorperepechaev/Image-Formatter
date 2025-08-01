#include "../../include/imageformatter/ReflectVerticallyOperation.hpp"

ReflectVerticallyOperation::ReflectVerticallyOperation(
    const std::vector<std::string> &arguments)
    : ReflectVerticallyOperation() {}

void ReflectVerticallyOperation::Apply(Image &image) const {
  for (size_t current_line = 0; current_line < image.GetHeight();
       ++current_line) {
    for (size_t current_column = 0; current_column < image.GetWidth() / 2;
         ++current_column) {
      auto current_pixel_nonalpha_values =
          image.GetNonAlphaPixelValues(current_line, current_column);
      auto vertically_symmetrical_pixel_nonalpha_values =
          image.GetNonAlphaPixelValues(current_line,
                                       image.GetWidth() - current_column - 1);

      std::swap(current_pixel_nonalpha_values,
                vertically_symmetrical_pixel_nonalpha_values);

      image.ApplyNewPixelValues(current_line, current_column,
                                current_pixel_nonalpha_values);
      image.ApplyNewPixelValues(current_line,
                                image.GetWidth() - current_column - 1,
                                vertically_symmetrical_pixel_nonalpha_values);
    }
  }
}

bool ReflectVerticallyOperation::ValidateArguments(
    const std::vector<std::string> &arguments) {
  return arguments.empty();
}
