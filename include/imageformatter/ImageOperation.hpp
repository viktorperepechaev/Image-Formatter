#pragma once

#include "Image.hpp"

class ImageOperation {
 public:
  virtual void Apply(Image& image) const = 0;
};
