#include <iostream>
#include <vector>

#include "image.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

Image::Image(const std::string& file_name) : data_(stbi_load(file_name.c_str(), &width_, &height_, &number_of_channels_, 0), stbi_image_free) {
  if (data_ == NULL) {
    throw std::runtime_error("Не смог открыть изначальный файл" + file_name + "\nПричина: " + stbi_failure_reason());
  }

  std::cout << "Успешно открыл изначальный файл!" << std::endl;
}

void Image::Darken(const char percent) {
  for (int current_line = 0; current_line < height_; ++current_line) {
    for (int current_column = 0; current_column < width_; ++current_column) {
      unsigned char* current_pixel = data_.get() + (width_ * current_line + current_column) * number_of_channels_;

      for (int current_channel = 0; current_channel < number_of_channels_; ++current_channel) {
        current_pixel[current_channel] = std::lround(current_pixel[current_channel] * 0.01 * percent);
      }
    }
  }
}

void Image::Invert() {
  for (int current_line = 0; current_line < height_; ++current_line) {
    for (int current_column = 0; current_column < width_; ++current_column) {
      unsigned char* current_pixel = data_.get() + (width_ * current_line + current_column) * number_of_channels_;

      for (int current_channel = 0; current_channel < number_of_channels_; ++current_channel) {
        current_pixel[current_channel] = Constants::kMaxColorValue - current_pixel[current_channel];
      }
    }
  }
}

void Image::ReflectHorizontally() {
  for (int current_line = 0; current_line < height_ / 2; ++current_line) {
    for (int current_column = 0; current_column < width_; ++current_column) {
      unsigned char* current_pixel = data_.get() + (width_ * current_line + current_column) * number_of_channels_;
      unsigned char* horizontally_symmetrical_pixel = data_.get() + (width_ * (height_ - current_line - 1) + current_column) * number_of_channels_;

      for (int current_channel = 0; current_channel < number_of_channels_; ++current_channel) {
        std::swap(current_pixel[current_channel], horizontally_symmetrical_pixel[current_channel]);
      }
    }
  }
}


void Image::ReflectVertically() {
  for (int current_line = 0; current_line < height_; ++current_line) {
    for (int current_column = 0; current_column < width_ / 2; ++current_column) {
      unsigned char* current_pixel = data_.get() + (width_ * current_line + current_column) * number_of_channels_;
      unsigned char* horizontally_symmetrical_pixel = data_.get() + (width_ * current_line + (width_ - current_column - 1)) * number_of_channels_;

      for (int current_channel = 0; current_channel < number_of_channels_; ++current_channel) {
        std::swap(current_pixel[current_channel], horizontally_symmetrical_pixel[current_channel]);
      }
    }
  }
}

void Image::Rotate90DegreesClockwise() {
  size_t new_data_size = height_ * width_ * number_of_channels_;
  
  std::unique_ptr<unsigned char, void(*)(void*)> new_data(static_cast<unsigned char*>(malloc(new_data_size)), stbi_image_free);  // using malloc here because stbi_image_free uses free
  
  for (int current_line = 0; current_line < height_; ++current_line) {
    for (int current_column = 0; current_column < width_; ++current_column) {
      int rotated_line_index = current_column;
      int rotated_column_index = height_ - current_line - 1;

      unsigned char* current_pixel = data_.get() + (width_ * current_line + current_column) * number_of_channels_;
      unsigned char* rotated_pixel = new_data.get() + (height_ * rotated_line_index + rotated_column_index) * number_of_channels_;
      
      memcpy(rotated_pixel, current_pixel, number_of_channels_);
    }
  }

  std::swap(height_, width_);

  data_ = std::move(new_data);
}

void Image::Rotate90DegreesCounterclockwise() {
  size_t new_data_size = height_ * width_ * number_of_channels_;

  std::unique_ptr<unsigned char, void(*)(void*)> new_data(static_cast<unsigned char*>(malloc(new_data_size)), stbi_image_free);

  for (int current_line = 0; current_line < height_; ++current_line) {
    for (int current_column = 0; current_column < width_; ++current_column) {
      int rotated_line_index = width_ - current_column - 1;
      int rotated_column_index = current_line;

      unsigned char* current_pixel = data_.get() + (width_ * current_line + current_column) * number_of_channels_;
      unsigned char* rotated_pixel = new_data.get() + (height_ * rotated_line_index + rotated_column_index) * number_of_channels_;
      
      memcpy(rotated_pixel, current_pixel, number_of_channels_);
    }
  }

  std::swap(height_, width_);

  data_ = std::move(new_data);
}

void Image::Rotate(int degree) {
  bool is_positive = (degree > 0);

  degree = std::abs(degree);
  degree %= 360;

  if (degree == 0) { 
    return; 
  }

  if (is_positive) {
    for (int counter = 0; counter < degree / 90; ++counter) {
      Rotate90DegreesClockwise();
    }
  } else {
    for (int counter = 0; counter < degree / 90; ++counter) {
      Rotate90DegreesCounterclockwise();
    }
  }
}

void Image::SobelOperator() {
  size_t new_data_size = height_ * width_ * number_of_channels_;
  
  std::unique_ptr<unsigned char, void(*)(void*)> new_data(static_cast<unsigned char*>(malloc(new_data_size)), stbi_image_free);

  std::vector<std::pair<int, int>> shifts = {
    {-1, 0},
    {-1, 1},
    {0, 1},
    {1, 1},
    {1, 0},
    {1, -1},
    {0, -1},
    {-1, -1}
  };

   
  for (int current_line = 1; current_line < height_ - 1; ++current_line) {
    for (int current_column = 1; current_column < width_ - 1; ++current_column) {
      std::vector<int> channel_sums(number_of_channels_);

      unsigned char* current_pixel = data_.get() + (width_ * current_line + current_column) * number_of_channels_;
      unsigned char* current_pixel_in_new_data = new_data.get() + (width_ * current_line + current_column) * number_of_channels_;

      auto get_average_of_RGB = [this](int line_index, int column_index) -> double {  
        unsigned char* current_pixel = data_.get() + (width_ * line_index + column_index) * number_of_channels_;
        int sum = 0;
        for (int current_channel = 0; current_channel < number_of_channels_; ++current_channel) {
          sum += current_pixel[current_channel];
        }
        return 1.0 * sum / number_of_channels_;
      };

      double gx = (get_average_of_RGB(current_line - 1, current_column + 1) + 2 * get_average_of_RGB(current_line, current_column + 1) + get_average_of_RGB(current_line + 1, current_column + 1)) - (get_average_of_RGB(current_line - 1, current_column - 1) + 2 * get_average_of_RGB(current_line, current_column - 1) + get_average_of_RGB(current_line + 1, current_column - 1));

      double gy = (get_average_of_RGB(current_line + 1, current_column - 1) + 2 * get_average_of_RGB(current_line + 1, current_column) + get_average_of_RGB(current_line + 1, current_column + 1)) - (get_average_of_RGB(current_line - 1, current_column - 1) + 2 * get_average_of_RGB(current_line - 1, current_column) + get_average_of_RGB(current_line - 1, current_column + 1));

      double magnitude = std::sqrt(gx * gx + gy * gy);

      unsigned char output_pixel_new_color = 255;
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

void Image::CreateOutputImage(const std::string& output_image_name) const {
  int stride = width_ * number_of_channels_;
  
  if (!stbi_write_png(output_image_name.c_str(), width_, height_, number_of_channels_, data_.get(), stride)) {
    throw std::runtime_error("Не удалось сохранить файл " + output_image_name);
  }

  std::cout << "Результат сохранен в файл " << output_image_name << "!" << std::endl;
}
