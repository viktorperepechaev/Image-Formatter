#include "../../include/imageformatter/GrayscaleOperation.hpp"

#include <algorithm>

void GrayscaleOperation::Apply(Image &image) const {
  if (image.GetNumberOfChannels() < 3) {
    return;
  }

  const size_t new_image_number_of_channels =
      image.GetNumberOfChannels() % 2 == 0 ? 2 : 1;

  Image new_image(image.GetHeight(), image.GetWidth(),
                  new_image_number_of_channels);

  for (size_t current_line = 0; current_line < image.GetHeight();
       ++current_line) {
    for (size_t current_column = 0; current_column < image.GetWidth();
         ++current_column) {
      std::vector<std::uint8_t> current_pixel_nonalpha =
          image.GetFullPixelValues(current_line, current_column);

      std::vector<std::uint8_t> new_pixel_values(new_image_number_of_channels);

      new_pixel_values[0] = std::clamp(
          static_cast<std::uint8_t>(
              std::round(current_pixel_nonalpha[0] * 0.299 +
                         current_pixel_nonalpha[1] * 0.587 +
                         current_pixel_nonalpha[2] * 0.114)),
          static_cast<std::uint8_t>(0), Image::Constants::kMaxColorValue);

      if (new_image_number_of_channels > 1) {
        new_pixel_values.back() = current_pixel_nonalpha.back();
      }

      new_image.ApplyNewPixelValues(current_line, current_column, new_pixel_values);
    }
  }

  image = std::move(new_image);
}

std::string GrayscaleOperation::GetName() const {
  return "grayscale";
}

bool GrayscaleOperation::ValidateArguments(const std::vector<std::string> &arguments) {
  return arguments.empty();
}
