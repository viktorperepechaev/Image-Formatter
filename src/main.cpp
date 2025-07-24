#include <cstdlib>
#include <stdexcept>
#include <string>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "headers/stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "headers/stb_image_write.h"

#include "headers/cxxopts.hpp"

class Image {
 public:
  Image() = default;
  Image(const std::string& file_name);
  
  ~Image();

  void Darken(const char percent);  // Получившаяся картинка будет иметь percent% света от изначального
  void Invert();  // Инвертирует цвет
  void ReflectHorizontally();  // Отражает по горизонтали
  void ReflectVertically();  //  Отражает по вертикали 

  void CreateOutputImage(const std::string& output_image_name) const;

 private:
  struct Constants {
    static constexpr int kMaxColorValue = 255;
  };

  unsigned char* data_ = nullptr;
  int width_ = 0;
  int height_ = 0;
  int number_of_channels_ = 0;
};

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

int main(int argc, char** argv) {
  // TODO: Сделать проверку аргументов сразу
  
  cxxopts::Options options("ImageFormatter", "This program formats the input picture based on cli arguments");

  options.add_options()
    ("i,input", "Input file", cxxopts::value<std::string>())
    ("o,output", "Output file", cxxopts::value<std::string>())
    ("operations", "Names of operations to perform", cxxopts::value<std::vector<std::string>>())
    ;
  
  options.parse_positional({"operations"});

  auto result = options.parse(argc, argv);

  if (!result.contains("input") || !result.contains("output")) {
    throw std::runtime_error("Input or Output files are not mentioned in the argument list");
  }

  Image image(result["input"].as<std::string>());

  std::vector<std::string> operation_list;

  for (const auto& kv : result.arguments()) {
    if (kv.key() != "operations") {
      continue;
    }
    operation_list.emplace_back(kv.value());
  }

  for (size_t operation_list_index = 0; operation_list_index < operation_list.size(); ++operation_list_index) {
    if (operation_list[operation_list_index] == "darken") {
      image.Darken(std::stoi(operation_list[operation_list_index + 1]));
      ++operation_list_index;
    } else if (operation_list[operation_list_index] == "invert") {
      image.Invert();
    } else if (operation_list[operation_list_index] == "ref-hor") {
      image.ReflectHorizontally();
    } else if (operation_list[operation_list_index] == "ref-ver") {
      image.ReflectVertically();
    } else {
      throw std::runtime_error("Unknown operation: " + operation_list[operation_list_index]);
    }
  }

  image.CreateOutputImage(result["output"].as<std::string>());
  
  return 0;
}
