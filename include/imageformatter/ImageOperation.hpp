#pragma once

#include "Image.hpp"
#include <string>

class ImageOperation {
public:
  virtual ~ImageOperation() = default;
  virtual void Apply(Image &image) const = 0;
  virtual std::string GetName() const = 0;
};
