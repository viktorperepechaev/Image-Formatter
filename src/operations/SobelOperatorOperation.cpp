#include "../../include/imageformatter/SobelOperatorOperation.hpp"

SobelOperatorOperation::SobelOperatorOperation(
    const std::vector<std::string> &arguments)
    : SobelOperatorOperation() {}

void SobelOperatorOperation::Apply(Image &image) const {
  Image buffer_image(image.GetHeight(), image.GetWidth(),
                     image.GetNumberOfChannels());

  for (size_t current_line = 1; current_line + 1 < image.GetHeight();
       ++current_line) {
    for (size_t current_column = 1; current_column + 1 < image.GetWidth();
         ++current_column) {
      auto container_mean_value =
          [](const std::vector<std::uint8_t> &vec) -> double {
        int sum = 0;
        for (const auto &el : vec) {
          sum += el;
        }
        return 1.0 * sum / vec.size();
      };

      const double gx = (container_mean_value(image.GetNonAlphaPixelValues(
                             current_line - 1, current_column + 1)) +
                         2 * container_mean_value(image.GetNonAlphaPixelValues(
                                 current_line, current_column + 1)) +
                         container_mean_value(image.GetNonAlphaPixelValues(
                             current_line + 1, current_column + 1))) -
                        (container_mean_value(image.GetNonAlphaPixelValues(
                             current_line - 1, current_column - 1)) +
                         2 * container_mean_value(image.GetNonAlphaPixelValues(
                                 current_line, current_column - 1)) +
                         container_mean_value(image.GetNonAlphaPixelValues(
                             current_line + 1, current_column - 1)));

      const double gy = (container_mean_value(image.GetNonAlphaPixelValues(
                             current_line + 1, current_column - 1)) +
                         2 * container_mean_value(image.GetNonAlphaPixelValues(
                                 current_line + 1, current_column)) +
                         container_mean_value(image.GetNonAlphaPixelValues(
                             current_line + 1, current_column + 1))) -
                        (container_mean_value(image.GetNonAlphaPixelValues(
                             current_line - 1, current_column - 1)) +
                         2 * container_mean_value(image.GetNonAlphaPixelValues(
                                 current_line - 1, current_column)) +
                         container_mean_value(image.GetNonAlphaPixelValues(
                             current_line - 1, current_column + 1)));

      const double magnitude = std::sqrt(gx * gx + gy * gy);

      std::vector<std::uint8_t> current_pixel_new_color(
          image.GetNumberOfChannels(), 0);

      if (magnitude > 85) {
        std::fill(current_pixel_new_color.begin(),
                  current_pixel_new_color.end(), 255);
      }

      buffer_image.ApplyNewPixelValues(current_line, current_column,
                                       current_pixel_new_color);
    }
  }

  image = std::move(buffer_image);
}

std::string SobelOperatorOperation::GetName() const { return "sobel"; }

bool SobelOperatorOperation::ValidateArguments(
    const std::vector<std::string> &arguments) {
  return arguments.empty();
}
