#pragma once

#include <string>
#include <memory>

#include "stb_image.h"
#include "stb_image_write.h"

class Image {
 public:
  Image() = delete;
  Image(const std::string& file_name);
  
  ~Image() = default;

  void Darken(const char percent);  // Получившаяся картинка будет иметь percent% света от изначального
  
  void Invert();  // Инвертирует цвет
  
  void ReflectHorizontally();  // Отражает по горизонтали
  void ReflectVertically();  //  Отражает по вертикали 
  
  void Rotate90DegreesClockwise();  // Поворачивает картинку на 90 градусов по часовой
  void Rotate90DegreesCounterclockwise();  // Поворачивает картинку на 90 градусов против часов
  void Rotate(const int degree);  // Поворачиает картинку на degree градусов (degree должно делиться на 90) 
  
  void CreateOutputImage(const std::string& output_image_name) const;  // Создаёт файл (.png) с указанным названием

 private:
  struct Constants {
    static constexpr int kMaxColorValue = 255;
  };

  std::unique_ptr<unsigned char, void(*)(void*)> data_;
  int width_;
  int height_;
  int number_of_channels_;
};
