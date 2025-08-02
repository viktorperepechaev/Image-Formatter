#include "../../include/imageformatter/SteganographyDecodeOperation.hpp"

SteganographyDecodeOperation::SteganographyDecodeOperation() : rnd(228) {}

void SteganographyDecodeOperation::Apply(Image& image) const {
  std::string result;
  std::string buffer;

  while (result.empty() || result.back() != '$') {
    std::pair<size_t, size_t> pixel_coordinates{GetRandomValue(0, image.GetHeight() - 1), GetRandomValue(0, image.GetWidth() - 1)};

    std::uint8_t channel_index = GetRandomValue(0, image.GetNumberOfChannels() - 1);

    std::uint8_t needed_channel_value = image.GetNonAlphaPixelValues(pixel_coordinates.first, pixel_coordinates.second)[channel_index];

    std::uint8_t bit_value = needed_channel_value & 1;

    buffer += std::to_string(bit_value);

    if (buffer.size() % 8 == 0) {
      result.push_back(static_cast<char>(std::stoi(buffer, nullptr, 2)));
      buffer.clear();
    }
  }

  decoded_message_ = result;
}

std::string SteganographyDecodeOperation::GetName() const {
  return "decode";
}

bool SteganographyDecodeOperation::ValidateArguments(const std::vector<std::string> &arguments) {
  return !arguments.empty();
}

std::string SteganographyDecodeOperation::GetDecodedMessage() const {
  return decoded_message_;
}

int SteganographyDecodeOperation::GetRandomValue(int min, int max) const {
  return rnd() % (max - min + 1) + min;
}
