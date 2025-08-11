#include "../../include/imageformatter/ImageToASCIIOperation.hpp"

#include <cstdint>
#include <fstream>
#include <iostream>

void ImageToASCIIOperation::Apply(Image& image) const {
  std::ofstream out_file("ascii.txt", std::ios::app);
  
  for (size_t current_line = 0; current_line < image.GetHeight(); ++current_line) {
    for (size_t current_column = 0; current_column < image.GetWidth(); ++current_column) {
      const std::vector<std::uint8_t> current_pixel = image.GetNonAlphaPixelValues(current_line, current_column);
      const char corresponding_ascii_character = ascii_alphabet_[current_pixel[0] / bucket_size_];

      out_file << corresponding_ascii_character;
    }
    out_file << '\n';
  }

  out_file.close();

  std::cout << "ascii image successfully written to ascii.txt\n";
}

std::string ImageToASCIIOperation::GetName() const {
  return "ascii";
}

bool ImageToASCIIOperation::ValidateArguments(const std::vector<std::string> &arguments) {
  return arguments.empty();
}
