#include "../../include/imageformatter/DownscaleOperation.hpp"
#include <iostream>

DownscaleOperation::DownscaleOperation(const size_t height_scale_factor,
                                       const size_t width_scale_factor)
    : height_scale_factor_(height_scale_factor),
      width_scale_factor_(width_scale_factor) {}

DownscaleOperation::DownscaleOperation(
    const std::vector<std::string>& arguments)
    : DownscaleOperation(std::stoi(arguments[0]), std::stoi(arguments[1])) {}

std::vector<std::uint8_t> DownscaleOperation::GetAverageOfSubimage(
    const Image& image, const size_t top_left_corner_line,
    const size_t top_left_corner_column, const size_t bottom_right_corner_line,
    const size_t bottom_right_corner_column) const {
  std::vector<std::uint32_t> result_pixel_values(image.GetNumberOfChannels());

  for (size_t current_line = top_left_corner_line;
       current_line <= bottom_right_corner_line; ++current_line) {
    for (size_t current_column = top_left_corner_column;
         current_column <= bottom_right_corner_column; ++current_column) {
      std::vector<std::uint8_t> current_pixel =
          image.GetFullPixelValues(current_line, current_column);

      for (size_t current_channel = 0;
           current_channel < image.GetNumberOfChannels(); ++current_channel) {
        result_pixel_values[current_channel] += current_pixel[current_channel];
      }
    }
  }

  for (std::uint32_t& channel_value : result_pixel_values) {
    channel_value /= (bottom_right_corner_line - top_left_corner_line + 1) *
                     (bottom_right_corner_column - top_left_corner_column + 1);
  }

  return std::vector<std::uint8_t>(result_pixel_values.begin(),
                                   result_pixel_values.end());
}

void DownscaleOperation::Apply(Image& image) const {
  const size_t new_image_height = image.GetHeight() / height_scale_factor_;
  const size_t new_image_width = image.GetWidth() / width_scale_factor_;

  Image new_image(new_image_height, new_image_width,
                  image.GetNumberOfChannels());

  const size_t pixel_matrix_height = height_scale_factor_;
  const size_t pixel_matrix_width = width_scale_factor_;

  for (size_t current_line = 0;
       current_line + pixel_matrix_height - 1 < image.GetHeight();
       current_line += pixel_matrix_height) {
    for (size_t current_column = 0;
         current_column + pixel_matrix_width - 1 < image.GetWidth();
         current_column += pixel_matrix_width) {
      std::vector<std::uint8_t> new_pixel =
          GetAverageOfSubimage(image, current_line, current_column,
                               current_line + pixel_matrix_height - 1,
                               current_column + pixel_matrix_width - 1);
      new_image.ApplyNewPixelValues(current_line / height_scale_factor_,
                                    current_column / width_scale_factor_,
                                    new_pixel);
    }
  }
  std::cout << "new_image height = " + std::to_string(new_image.GetHeight()) << '\n';
  std::cout << "new_image width = " + std::to_string(new_image.GetWidth()) << '\n';
  image = std::move(new_image);
}

std::string DownscaleOperation::GetName() const { return "downscale"; }

bool DownscaleOperation::ValidateArguments(
    const std::vector<std::string>& arguments) {
  return arguments.size() == 2 &&
         arguments[0].find_first_not_of("0123456789") == std::string::npos &&
         arguments[1].find_first_not_of("0123456789") == std::string::npos &&
         std::stoi(arguments[0]) > 0 && std::stoi(arguments[1]) > 0;
}
