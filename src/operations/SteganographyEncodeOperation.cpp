#include "../../include/helper/StringToBinary.hpp"
#include "../../include/helper/GetRandomValue.hpp"
#include "../../include/imageformatter/SteganographyEncodeOperation.hpp"

SteganographyEncodeOperation::SteganographyEncodeOperation(const std::string& secret_message) : rnd(228), secret_message_(secret_message) {}

SteganographyEncodeOperation::SteganographyEncodeOperation(const std::vector<std::string>& arguments) : SteganographyEncodeOperation(arguments[0]) {}

void SteganographyEncodeOperation::Apply(Image& image) const {
  std::string binary_message = StringToBinary(secret_message_ + "$");

  std::vector<std::pair<size_t, size_t>> pixel_coordinates;
  std::vector<std::uint8_t> channel_indexes;

  for (const auto& el : binary_message) {
    pixel_coordinates.emplace_back(GetRandomValue(rnd, 0, image.GetHeight() - 1), GetRandomValue(rnd, 0, image.GetWidth() - 1));
    channel_indexes.emplace_back(GetRandomValue(rnd, 0, image.GetNumberOfChannels() - 1));
    
    std::vector<std::uint8_t> nonalpha_pixel_values = image.GetNonAlphaPixelValues(pixel_coordinates.back().first, pixel_coordinates.back().second);

    if (el == '0') {
      nonalpha_pixel_values[channel_indexes.back()] &= ~1u;
    } else {
      nonalpha_pixel_values[channel_indexes.back()] |= 1u;
    }
    
    image.ApplyNewPixelValues(pixel_coordinates.back().first, pixel_coordinates.back().second, nonalpha_pixel_values);
  }
}

std::string SteganographyEncodeOperation::GetName() const {
  return "encode";
}

bool SteganographyEncodeOperation::ValidateArguments(const std::vector<std::string> &arguments) {
  return !arguments.empty();
}
