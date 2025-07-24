#pragma once

#include <string>

#include "stb_image.h"
#include "stb_image_write.h"

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
