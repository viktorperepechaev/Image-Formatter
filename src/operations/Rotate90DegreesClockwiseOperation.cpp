#include "../../include/imageformatter/Rotate90DegreesClockwiseOperation.hpp"

Rotate90DegreesClockwiseOperation::Rotate90DegreesClockwiseOperation(const std::vector<std::string>& arguments) : Rotate90DegreesClockwiseOperation() {}

void Rotate90DegreesClockwiseOperation::Apply(Image& image) const {

}

void Image::Rotate90DegreesClockwise() {
  const size_t new_data_size = height_ * width_ * number_of_channels_;
  
  std::unique_ptr<unsigned char, void(*)(void*)> new_data(static_cast<unsigned char*>(malloc(new_data_size)), stbi_image_free);  // using malloc here because stbi_image_free uses free
  
  for (int current_line = 0; current_line < height_; ++current_line) {
    for (int current_column = 0; current_column < width_; ++current_column) {
      const int rotated_line_index = current_column;
      const int rotated_column_index = height_ - current_line - 1;

      const unsigned char* const current_pixel = data_.get() + (width_ * current_line + current_column) * number_of_channels_;
      unsigned char* const rotated_pixel = new_data.get() + (height_ * rotated_line_index + rotated_column_index) * number_of_channels_;
     
      memcpy(rotated_pixel, current_pixel, number_of_channels_);
    }
  }

  std::swap(height_, width_);

  data_ = std::move(new_data);
}
