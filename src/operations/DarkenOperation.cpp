#include "../../include/imageformatter/DarkenOperation.hpp"

DarkenOperation::DarkenOperation(const std::uint8_t percent)
    : needed_share_(0.01 * percent) {}

DarkenOperation::DarkenOperation(const std::vector<std::string> &arguments)
    : DarkenOperation(std::stoi(arguments[0])) {}

void DarkenOperation::Apply(Image &image) const {
  for (size_t current_line = 0; current_line < image.GetHeight();
       ++current_line) {
    for (size_t current_column = 0; current_column < image.GetWidth();
         ++current_column) {
      auto nonalpha_pixel_values =
          image.GetNonAlphaPixelValues(current_line, current_column);

      for (auto &value : nonalpha_pixel_values) {
        value = std::lround(value * needed_share_);
      }

      image.ApplyNewPixelValues(current_line, current_column,
                                nonalpha_pixel_values);
    }
  }
}

bool DarkenOperation::ValidateArguments(
    const std::vector<std::string> &arguments) {
  return arguments.size() == 1 &&
         arguments[0].find_first_not_of("0123456789") == std::string::npos &&
         0 <= std::stoi(arguments[0]) && std::stoi(arguments[0]) <= 100;
}
