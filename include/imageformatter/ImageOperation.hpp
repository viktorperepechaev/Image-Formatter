#pragma once

#include "Image.hpp"

class ImageOperation {
public:
  virtual ~ImageOperation() = default;
  virtual void Apply(Image &image) const = 0;
};
