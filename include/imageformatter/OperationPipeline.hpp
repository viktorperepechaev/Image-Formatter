#pragma once

#include "ImageOperation.hpp"

class OperationPipeline {
public:
  OperationPipeline() = default;

  void AddOperation(std::unique_ptr<ImageOperation> &&operation);

  void Run(Image &image) const;

private:
  std::vector<std::unique_ptr<ImageOperation>> operations;
};
