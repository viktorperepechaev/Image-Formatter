#include "../../include/imageformatter/Rotate90DegreesClockwiseOperation.hpp"

Rotate90DegreesClockwiseOperation::Rotate90DegreesClockwiseOperation(
    const std::vector<std::string> &arguments)
    : Rotate90DegreesClockwiseOperation() {}

void Rotate90DegreesClockwiseOperation::Apply(Image &image) const {
  const size_t new_data_size =
      image.GetHeight() * image.GetWidth() * image.GetNumberOfChannels();

  Image buffer_image(image.GetWidth(), image.GetHeight(),
                     image.GetNumberOfChannels());

  for (size_t current_line = 0; current_line < image.GetHeight();
       ++current_line) {
    for (size_t current_column = 0; current_column < image.GetWidth();
         ++current_column) {
      const size_t rotated_line_index = current_column;
      const size_t rotated_column_index = image.GetHeight() - current_line - 1;

      auto current_pixel_values =
          image.GetFullPixelValues(current_line, current_column);

      buffer_image.ApplyNewPixelValues(rotated_line_index, rotated_column_index,
                                       current_pixel_values);
    }
  }

  image = std::move(buffer_image);
}

bool Rotate90DegreesClockwiseOperation::ValidateArguments(
    const std::vector<std::string> &arguments) {
  return arguments.empty();
}
