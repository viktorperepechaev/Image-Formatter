#include "../../include/imageformatter/SobelOperatorOperation.hpp"

SobelOperatorOperation::SobelOperatorOperation(const std::vector<std::string>& arguments) : SobelOperatorOperation() {}

void SobelOperatorOperation::Apply(Image& image) const {
  Image buffer_image(image.GetHeight(), image.GetWidth(), image.GetNumberOfChannels());
  
  for (size_t current_line = 1; current_line + 1 < image.GetHeight(); ++current_line) {
    for (size_t current_column = 1; current_column + 1 < image.GetWidth(); ++current_column) {
      auto container_mean_value = [](const std::vector<std::uint8_t>& vec) -> double {
        int sum = 0;
        for (const auto& el : vec) {
          sum += el;
        }
        return 1.0 * sum / vec.size();
      };

      const double gx = (container_mean_value(image.GetNonAlphaPixelValues(current_line - 1, current_column + 1)) + 2 * container_mean_value(image.GetNonAlphaPixelValues(current_line, current_column + 1)) + container_mean_value(image.GetNonAlphaPixelValues(current_line + 1, current_column + 1))) - (container_mean_value(image.GetNonAlphaPixelValues(current_line - 1, current_column - 1)) + 2 * container_mean_value(image.GetNonAlphaPixelValues(current_line, current_column - 1)) + container_mean_value(image.GetNonAlphaPixelValues(current_line + 1, current_column - 1)));

      const double gy = (container_mean_value(image.GetNonAlphaPixelValues(current_line + 1, current_column - 1)) + 2 * container_mean_value(image.GetNonAlphaPixelValues(current_line + 1, current_column)) + container_mean_value(image.GetNonAlphaPixelValues(current_line + 1, current_column + 1))) - (container_mean_value(image.GetNonAlphaPixelValues(current_line - 1, current_column - 1)) + 2 * container_mean_value(image.GetNonAlphaPixelValues(current_line - 1, current_column)) + container_mean_value(image.GetNonAlphaPixelValues(current_line - 1, current_column + 1)));

      const double magnitude = std::sqrt(gx * gx + gy * gy);

      std::vector<std::uint8_t> current_pixel_new_color(image.GetNumberOfChannels(), 0);

      if (magnitude > 85) {
        std::fill(current_pixel_new_color.begin(), current_pixel_new_color.end(), 255);
      }

      buffer_image.ApplyNewPixelValues(current_line, current_column, current_pixel_new_color);
    }
  }

  image = std::move(buffer_image);
}

bool SobelOperatorOperation::ValidateArguments(const std::vector<std::string> &arguments) {
  return arguments.empty();
}


/*
void Image::SobelOperator() {
  const size_t new_data_size = height_ * width_ * number_of_channels_;
  
  std::unique_ptr<unsigned char, void(*)(void*)> new_data(static_cast<unsigned char*>(malloc(new_data_size)), stbi_image_free);

  for (int current_line = 1; current_line < height_ - 1; ++current_line) {
    for (int current_column = 1; current_column < width_ - 1; ++current_column) {

      const unsigned char* const current_pixel = data_.get() + (width_ * current_line + current_column) * number_of_channels_;
      unsigned char* const current_pixel_in_new_data = new_data.get() + (width_ * current_line + current_column) * number_of_channels_;

      auto get_average_of_RGB = [this](int line_index, int column_index) -> double {  
        const unsigned char* const current_pixel = data_.get() + (width_ * line_index + column_index) * number_of_channels_;
        int sum = 0;
        for (int current_channel = 0; current_channel < number_of_channels_; ++current_channel) {
          sum += current_pixel[current_channel];
        }
        return 1.0 * sum / number_of_channels_;
      };

      const double gx = (get_average_of_RGB(current_line - 1, current_column + 1) + 2 * get_average_of_RGB(current_line, current_column + 1) + get_average_of_RGB(current_line + 1, current_column + 1)) - (get_average_of_RGB(current_line - 1, current_column - 1) + 2 * get_average_of_RGB(current_line, current_column - 1) + get_average_of_RGB(current_line + 1, current_column - 1));

      const double gy = (get_average_of_RGB(current_line + 1, current_column - 1) + 2 * get_average_of_RGB(current_line + 1, current_column) + get_average_of_RGB(current_line + 1, current_column + 1)) - (get_average_of_RGB(current_line - 1, current_column - 1) + 2 * get_average_of_RGB(current_line - 1, current_column) + get_average_of_RGB(current_line - 1, current_column + 1));

      const double magnitude = std::sqrt(gx * gx + gy * gy);

      unsigned char output_pixel_new_color = Constants::kMaxColorValue;
      if (magnitude <= 85) {
        output_pixel_new_color = 0;
      }

      for (int current_channel = 0; current_channel < number_of_channels_; ++current_channel) {
        current_pixel_in_new_data[current_channel] = output_pixel_new_color;
      }
    }
  }
  data_ = std::move(new_data);
}
*/
