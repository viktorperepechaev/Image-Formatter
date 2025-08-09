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
