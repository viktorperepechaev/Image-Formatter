#include <cstdint>
#include <iostream>
#include <sys/types.h>
#include <vector>

#include "../include/imageformatter/Image.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "../third_party/stb/stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../third_party/stb/stb_image_write.h"

Image::Image(const std::string &filename) {
  unsigned char *stbi_data =
      stbi_load(filename.c_str(), &width_, &height_, &number_of_channels_, 0);

  if (stbi_data == NULL) {
    throw std::runtime_error("Can not open " + filename +
                             "\nReason: " + stbi_failure_reason());
  }

  data_.assign(reinterpret_cast<std::uint8_t *>(stbi_data),
               reinterpret_cast<std::uint8_t *>(stbi_data) +
                   width_ * height_ * number_of_channels_);

  stbi_image_free(stbi_data);

  std::cout << "Succesfully opened " + filename << std::endl;
}

Image::Image(const size_t height, const size_t width,
             const size_t number_of_channels)
    : height_(height), width_(width), number_of_channels_(number_of_channels),
      data_(width_ * height_ * number_of_channels_) {}

Image &Image::operator=(Image &&other) {
  if (this != &other) {
    width_ = other.width_;
    height_ = other.height_;
    number_of_channels_ = other.number_of_channels_;

    other.width_ = 0;
    other.height_ = 0;
    other.number_of_channels_ = 0;

    data_ = std::move(other.data_);
  }

  return *this;
}

std::vector<std::uint8_t>
Image::GetNonAlphaPixelValues(const int line_index,
                              const int column_index) const {
  auto pixel = GetPixelSpan(line_index, column_index);

  const int output_number_of_channels = [number_of_channels_ =
                                             number_of_channels_]() {
    if (number_of_channels_ < 3) {
      return std::max(1, number_of_channels_);
    }
    return std::max(3, number_of_channels_);
  }();

  std::vector<std::uint8_t> result(output_number_of_channels);

  for (int current_channel = 0; current_channel < output_number_of_channels;
       ++current_channel) {
    result[current_channel] = pixel[current_channel];
  }

  return result;
}

std::vector<std::uint8_t>
Image::GetFullPixelValues(const int line_index, const int column_index) const {
  auto pixel = GetPixelSpan(line_index, column_index);

  std::vector<std::uint8_t> result(number_of_channels_);

  for (int current_channel = 0; current_channel < number_of_channels_;
       ++current_channel) {
    result[current_channel] = pixel[current_channel];
  }

  return result;
}

void Image::ApplyNewPixelValues(
    const int line_index, const int column_index,
    const std::vector<std::uint8_t> &new_pixel_values) {
  auto pixel = GetPixelSpan(line_index, column_index);

  for (size_t current_channel = 0; current_channel < new_pixel_values.size();
       ++current_channel) {
    pixel[current_channel] = new_pixel_values[current_channel];
  }
}

size_t Image::GetWidth() const noexcept { return static_cast<size_t>(width_); }

size_t Image::GetHeight() const noexcept {
  return static_cast<size_t>(height_);
}

size_t Image::GetNumberOfChannels() const noexcept {
  return static_cast<size_t>(number_of_channels_);
}

inline std::span<const std::uint8_t>
Image::GetPixelSpan(const int line_index, const int column_index) const {
  const int start_index =
      (line_index * width_ + column_index) * number_of_channels_;
  return std::span<const std::uint8_t>(data_.begin() + start_index,
                                       number_of_channels_);
}

inline std::span<std::uint8_t> Image::GetPixelSpan(const int line_index,
                                                   const int column_index) {
  const int start_index =
      (line_index * width_ + column_index) * number_of_channels_;
  return std::span<std::uint8_t>(data_.begin() + start_index,
                                 number_of_channels_);
}

bool Image::PixelIsInsideImage(const int line_index,
                               const int column_index) const noexcept {
  return 0 <= line_index && line_index < height_ && 0 <= column_index &&
         column_index < width_;
}

void Image::CreateOutputImage(const std::string &output_image_name) const {
  const int stride = width_ * number_of_channels_;

  if (!stbi_write_png(output_image_name.c_str(), width_, height_,
                      number_of_channels_, data_.data(), stride)) {
    throw std::runtime_error("Unable to create output file: " +
                             output_image_name);
  }

  std::cout << "Output file has been created: " << output_image_name << "!"
            << std::endl;
}
