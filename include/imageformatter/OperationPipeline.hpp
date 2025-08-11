#pragma once

#include "ImageOperation.hpp"
#include <unordered_map>

class OperationPipeline {
public:
  OperationPipeline() = default;

  void AddOperation(std::unique_ptr<ImageOperation> &&operation);

  void Run(Image &image);

private:
  std::vector<std::unique_ptr<ImageOperation>> operations;
  std::unordered_map<std::string, bool> had_operation_;
};
