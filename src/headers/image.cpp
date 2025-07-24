#include <iostream>

#include "image.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

Image::Image(const std::string& file_name) {
  data_ = stbi_load(file_name.c_str(), &width_, &height_, &number_of_channels_, 0);

  if (data_ == NULL) {
    throw std::runtime_error("Не смог открыть изначальный файл" + file_name + "\nПричина: " + stbi_failure_reason());
  }

  std::cout << "Успешно открыл изначальный файл!" << std::endl;
}

Image::~Image() {
  stbi_image_free(data_);
}

void Image::Darken(const char percent) {
  for (int current_line = 0; current_line < height_; ++current_line) {
    for (int current_column = 0; current_column < width_; ++current_column) {
      unsigned char* current_pixel = data_ + (width_ * current_line + current_column) * number_of_channels_;

      for (int current_channel = 0; current_channel < number_of_channels_; ++current_channel) {
        current_pixel[current_channel] = std::lround(current_pixel[current_channel] * 0.01 * percent);
      }
    }
  }
}

void Image::Invert() {
  for (int current_line = 0; current_line < height_; ++current_line) {
    for (int current_column = 0; current_column < width_; ++current_column) {
      unsigned char* current_pixel = data_ + (width_ * current_line + current_column) * number_of_channels_;

      for (int current_channel = 0; current_channel < number_of_channels_; ++current_channel) {
        current_pixel[current_channel] = Constants::kMaxColorValue - current_pixel[current_channel];
      }
    }
  }
}

void Image::ReflectHorizontally() {
  for (int current_line = 0; current_line < height_ / 2; ++current_line) {
    for (int current_column = 0; current_column < width_; ++current_column) {
      unsigned char* current_pixel = data_ + (width_ * current_line + current_column) * number_of_channels_;
      unsigned char* horizontally_symmetrical_pixel = data_ + (width_ * (height_ - current_line - 1) + current_column) * number_of_channels_;

      for (int current_channel = 0; current_channel < number_of_channels_; ++current_channel) {
        std::swap(current_pixel[current_channel], horizontally_symmetrical_pixel[current_channel]);
      }
    }
  }
}


void Image::ReflectVertically() {
  for (int current_line = 0; current_line < height_; ++current_line) {
    for (int current_column = 0; current_column < width_ / 2; ++current_column) {
      unsigned char* current_pixel = data_ + (width_ * current_line + current_column) * number_of_channels_;
      unsigned char* horizontally_symmetrical_pixel = data_ + (width_ * current_line + (width_ - current_column - 1)) * number_of_channels_;

      for (int current_channel = 0; current_channel < number_of_channels_; ++current_channel) {
        std::swap(current_pixel[current_channel], horizontally_symmetrical_pixel[current_channel]);
      }
    }
  }
}

void Image::CreateOutputImage(const std::string& output_image_name) const {
  int stride = width_ * number_of_channels_;
  
  if (!stbi_write_png(output_image_name.c_str(), width_, height_, number_of_channels_, data_, stride)) { // Может стоит здесь как-то память очистить???
    throw std::runtime_error("Не удалось сохранить файл " + output_image_name);
  }

  std::cout << "Результат сохранен в файл " << output_image_name << "!" << std::endl;
}
