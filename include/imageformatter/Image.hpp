#pragma once

#include <span>
#include <string>
#include <vector>

#include "../../third_party/stb/stb_image.h"
#include "../../third_party/stb/stb_image_write.h"

class Image {
public:
  static_assert(sizeof(unsigned char) == sizeof(std::uint8_t),
                "Platform incomplatibile: unsigned char != 8 bits!");

  Image() = delete;
  Image(const std::string &filename);
  Image(const size_t height, const size_t width,
        const size_t number_of_channels);

  ~Image() = default;

  Image &operator=(Image &&image);

  std::vector<std::uint8_t>
  GetNonAlphaPixelValues(const int line_index, const int column_index) const;
  std::vector<std::uint8_t> GetFullPixelValues(const int line_index,
                                               const int column_index) const;

  void ApplyNewPixelValues(const int line_index, const int column_index,
                           const std::vector<std::uint8_t> &new_pixel_values);

  size_t GetWidth() const noexcept;
  size_t GetHeight() const noexcept;
  size_t GetNumberOfChannels() const noexcept;

  void CreateOutputImage(const std::string &output_image_name) const;

  struct Constants {
    static constexpr std::uint8_t kMaxColorValue = 255;
  };

private:
  std::span<const std::uint8_t> GetPixelSpan(const int line_index,
                                             const int column_index) const;
  std::span<std::uint8_t> GetPixelSpan(const int line_index,
                                       const int column_index);

  bool PixelIsInsideImage(const int line_index,
                          const int column_index) const noexcept;

  int width_;
  int height_;
  int number_of_channels_;
  std::vector<std::uint8_t> data_;
};

// class image {
//  public:
//   image() = delete;
//   image(const std::string& file_name);
//
//   ~image() = default;
//
//   void Darken(std::uint8_t percent);  // Получившаяся картинка будет иметь
//   percent% света от изначального, TODO: add float support or make sure the
//   user understands that you can input only int
//
//   void Invert();  // Инвертирует цвет
//
//   void ReflectHorizontally();  // Отражает по горизонтали
//   void ReflectVertically();  //  Отражает по вертикали
//
//   void Rotate90DegreesClockwise();  // Поворачивает картинку на 90 градусов
//   по часовой void Rotate90DegreesCounterclockwise();  // Поворачивает
//   картинку на 90 градусов против часов void Rotate(const int degree);  //
//   Поворачиает картинку на degree градусов (degree должно делиться на 90)
//
//   void SobelOperator();  // Применяет оператор Собеля, TODO: добавить
//   параметр "жёсткости" оператора
//
//   void CreateOutputImage(const std::string& output_image_name) const;  //
//   Создаёт файл (.png) с указанным названием
//
//  private:
//   bool CoordinatesInsidePicture(int line_index, int column_index) const;
//
//   struct Constants {
//     static constexpr int kMaxColorValue = 255;
//   };
//
//   std::unique_ptr<unsigned char, void(*)(void*)> data_;
//   int width_;
//   int height_;
//   int number_of_channels_;
// };
