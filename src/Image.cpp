#include <cstdint>
#include <iostream>
#include <sys/types.h>
#include <vector>

#include "../include/imageformatter/Image.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "../third_party/stb/stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../third_party/stb/stb_image_write.h"

Image::Image(const std::string& filename)  {
  unsigned char* stbi_data = stbi_load(filename.c_str(), &width_, &height_, &number_of_channels_, 0); 

  if (stbi_data == NULL) { 
    throw std::runtime_error("Can not open " + filename + "\nReason: " + stbi_failure_reason());
  }

  data_.assign(reinterpret_cast<std::uint8_t*>(stbi_data), reinterpret_cast<std::uint8_t*>(stbi_data) + width_ * height_ * number_of_channels_);

  stbi_image_free(stbi_data);

  std::cout << "Succesfully opened " + filename << std::endl;
}

Image::Image(const size_t height, const size_t width, const size_t number_of_channels) : height_(height), width_(width), number_of_channels_(number_of_channels), data_(width_ * height_ * number_of_channels_) {}

std::vector<std::uint8_t> Image::GetNonAlphaPixelValues(const int line_index, const int column_index) const {
  auto pixel = GetPixelSpan(line_index, column_index);

  const int output_number_of_channels = [number_of_channels_ = number_of_channels_](){
    if (number_of_channels_ < 3) {
      return std::max(1, number_of_channels_);
    }
    return std::max(3, number_of_channels_);
  }();

    std::vector<std::uint8_t> result(output_number_of_channels);
  
  for (int current_channel = 0; current_channel < output_number_of_channels; ++current_channel) {
    result[current_channel] = pixel[current_channel];
  }

  return result;
}

std::vector<std::uint8_t> Image::GetFullPixelValues(const int line_index, const int column_index) const {
  auto pixel = GetPixelSpan(line_index, column_index);

  std::vector<std::uint8_t> result(number_of_channels_);

  for (int current_channel = 0; current_channel < number_of_channels_; ++current_channel) {
    result[current_channel] = pixel[current_channel];
  }

  return result;
}

void Image::ApplyNewPixelValues(const int line_index, const int column_index, const std::vector<std::uint8_t>& new_pixel_values) {
  auto pixel = GetPixelSpan(line_index, column_index);

  for (size_t current_channel = 0; current_channel < new_pixel_values.size(); ++current_channel) {
    pixel[current_channel] = new_pixel_values[current_channel];
  }
}

size_t Image::GetWidth() const noexcept {
  return static_cast<size_t>(width_);
}

size_t Image::GetHeight() const noexcept {
  return static_cast<size_t>(height_);
}

size_t Image::GetNumberOfChannels() const noexcept {
  return static_cast<size_t>(number_of_channels_);
}

inline std::span<const std::uint8_t> Image::GetPixelSpan(const int line_index, const int column_index) const {
  const int start_index = (line_index * width_ + column_index) * number_of_channels_;
  return std::span<const std::uint8_t>(data_.begin() + start_index, number_of_channels_);
}

inline std::span<std::uint8_t> Image::GetPixelSpan(const int line_index, const int column_index) {
  const int start_index = (line_index * width_ + column_index) * number_of_channels_;
  return std::span<std::uint8_t>(data_.begin() + start_index, number_of_channels_);
}

bool Image::PixelIsInsideImage(const int line_index, const int column_index) const noexcept {
  return 0 <= line_index && line_index < height_ && 0 <= column_index && column_index < width_;
}

void Image::Rotate90DegreesClockwise() {
  const size_t new_data_size = height_ * width_ * number_of_channels_;
  
  std::unique_ptr<unsigned char, void(*)(void*)> new_data(static_cast<unsigned char*>(malloc(new_data_size)), stbi_image_free);  // using malloc here because stbi_image_free uses free
  
  for (int current_line = 0; current_line < height_; ++current_line) {
    for (int current_column = 0; current_column < width_; ++current_column) {
      const int rotated_line_index = current_column;
      const int rotated_column_index = height_ - current_line - 1;

      const unsigned char* const current_pixel = data_.get() + (width_ * current_line + current_column) * number_of_channels_;
      unsigned char* const rotated_pixel = new_data.get() + (height_ * rotated_line_index + rotated_column_index) * number_of_channels_;
     
      memcpy(rotated_pixel, current_pixel, number_of_channels_);
    }
  }

  std::swap(height_, width_);

  data_ = std::move(new_data);
}

void Image::Rotate90DegreesCounterclockwise() {
  const size_t new_data_size = height_ * width_ * number_of_channels_;

  std::unique_ptr<unsigned char, void(*)(void*)> new_data(static_cast<unsigned char*>(malloc(new_data_size)), stbi_image_free);

  for (int current_line = 0; current_line < height_; ++current_line) {
    for (int current_column = 0; current_column < width_; ++current_column) {
      const int rotated_line_index = width_ - current_column - 1;
      const int rotated_column_index = current_line;

      const unsigned char* const current_pixel = data_.get() + (width_ * current_line + current_column) * number_of_channels_;
      unsigned char* const rotated_pixel = new_data.get() + (height_ * rotated_line_index + rotated_column_index) * number_of_channels_;
      
      memcpy(rotated_pixel, current_pixel, number_of_channels_);
    }
  }

  std::swap(height_, width_);

  data_ = std::move(new_data);
}

void Image::Rotate(const int input_degree) {
  const bool is_positive = (input_degree > 0);

  const int degree_of_rotation = std::abs(input_degree) % 360;

  if (input_degree == 0) { 
    return; 
  }

  if (is_positive) {
    for (int counter = 0; counter < degree_of_rotation/ 90; ++counter) {
      Rotate90DegreesClockwise();
    }
  } else {
    for (int counter = 0; counter < degree_of_rotation / 90; ++counter) {
      Rotate90DegreesCounterclockwise();
    }
  }
}

void Image::SobelOperator() {
  const size_t new_data_size = height_ * width_ * number_of_channels_;
  
  std::unique_ptr<unsigned char, void(*)(void*)> new_data(static_cast<unsigned char*>(malloc(new_data_size)), stbi_image_free);

  for (int current_line = 1; current_line < height_ - 1; ++current_line) {
    for (int current_column = 1; current_column < width_ - 1; ++current_column) {
      std::vector<int> channel_sums(number_of_channels_);

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

bool Image::CoordinatesInsidePicture(const int line_index, const int column_index) const {
  return 0 <= line_index && line_index < height_ && 0 <= column_index && column_index < width_;
}

void Image::CreateOutputImage(const std::string& output_image_name) const {
  const int stride = width_ * number_of_channels_;
  
  if (!stbi_write_png(output_image_name.c_str(), width_, height_, number_of_channels_, data_.get(), stride)) {
    throw std::runtime_error("Unable to create output file: " + output_image_name);
  }

  std::cout << "Output file has been created: " << output_image_name << "!" << std::endl;
}
