#include <cstdlib>
#include <stdexcept>
#include <string>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

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
  
  std::cout << "Успешно открыл изначальный файл!" << std::endl;

  for (int current_line = 0; current_line < image_height; ++current_line) {
    for (int current_column = 0; current_column < image_width; ++current_column) {
      unsigned char* current_pixel = data + (image_width * current_line + current_column) * image_number_of_channels;

      unsigned char& red = current_pixel[0];
      unsigned char& green = current_pixel[1];
      unsigned char& blue = current_pixel[2];

      red /= 2;
      green /= 2;
      blue /= 2;
    }
  }

  int stride = image_width * image_number_of_channels;
  
  if (!stbi_write_png("result.png", image_width, image_height, image_number_of_channels, data, stride)) {
    stbi_image_free(data);

    throw std::runtime_error("Не удалось сохранить файл result.png");
  }

  std::cout << "Результат сохранен в файл " << "result.png" << "." << std::endl;
  
  stbi_image_free(data);
  return 0;
}
