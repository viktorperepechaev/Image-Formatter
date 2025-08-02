#include "../../include/imageformatter/Rotate90DegreesCounterclockwiseOperation.hpp"

Rotate90DegreesCounterclockwiseOperation::
    Rotate90DegreesCounterclockwiseOperation(
        const std::vector<std::string> &arguments)
    : Rotate90DegreesCounterclockwiseOperation() {}

void Rotate90DegreesCounterclockwiseOperation::Apply(Image &image) const {
  const size_t new_data_size =
      image.GetHeight() * image.GetWidth() * image.GetNumberOfChannels();

  Image buffer_image(image.GetWidth(), image.GetHeight(),
                     image.GetNumberOfChannels());

  for (size_t current_line = 0; current_line < image.GetHeight();
       ++current_line) {
    for (size_t current_column = 0; current_column < image.GetWidth();
         ++current_column) {
      const size_t rotated_line_index = image.GetWidth() - current_column - 1;
      const size_t rotated_column_index = current_line;

      auto current_pixel_values =
          image.GetFullPixelValues(current_line, current_column);

      buffer_image.ApplyNewPixelValues(rotated_line_index, rotated_column_index,
                                       current_pixel_values);
    }
  }

  image = std::move(buffer_image);
}

std::string Rotate90DegreesCounterclockwiseOperation::GetName() const {
  return "rot-ccw";
}

bool Rotate90DegreesCounterclockwiseOperation::ValidateArguments(
    const std::vector<std::string> &arguments) {
  return arguments.empty();
}
