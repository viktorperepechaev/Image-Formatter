#include <cstdlib>
#include <stdexcept>
#include <string>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

class Image {
 public:
  Image() = default;
  Image(const char* name);
  
  ~Image();

  void Darken(const char percent);
  
  void CreateOutputImage(const char* new_image_name) const;

 private:
  unsigned char* data_ = nullptr;
  int width_ = 0;
  int height_ = 0;
  int number_of_channels_ = 0;
};

Image::Image(const char* name) {
  data_ = stbi_load(name, &width_, &height_, &number_of_channels_, 0);

  if (data_ == NULL) {
    throw std::runtime_error("Не смог открыть изначальный файл" + std::string(name) + "\nПричина: " + stbi_failure_reason());
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

      unsigned char& red = current_pixel[0];
      unsigned char& green = current_pixel[1];
      unsigned char& blue = current_pixel[2];

      red = std::lround(red * 0.01 * percent);
      green = std::lround(green * 0.01 * percent);
      blue = std::lround(blue * 0.01 * percent);
    }
  }
}

void Image::CreateOutputImage(const char* new_image_name) const {
  int stride = width_ * number_of_channels_;
  
  if (!stbi_write_png(new_image_name, width_, height_, number_of_channels_, data_, stride)) { // Может стоит здесь как-то память очистить???
    throw std::runtime_error("Не удалось сохранить файл " + std::string(new_image_name));
  }

  std::cout << "Результат сохранен в файл " << std::string(new_image_name) << "!" << std::endl;
}

int main(int argc, char** argv) {
  if (argc != 2) { 
    throw std::runtime_error("Неправильный список аргументов. Он должен выглядеть так: [<name_of_the_picture>]");
  }

  int image_width;
  int image_height;
  int image_number_of_channels;

  unsigned char* data = nullptr;

  data = stbi_load(argv[1], &image_width, &image_height, &image_number_of_channels, 0);
  
  if (data == NULL) {
    throw std::runtime_error("Не смог открыть изначальный файл " + std::string(argv[1]) + "\nПричина: " + stbi_failure_reason());
  }
  
  Image image(argv[1]);

  image.Darken(10);
  image.CreateOutputImage("sigma.png");
  
  return 0;
}
