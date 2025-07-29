#include "../../include/imageformatter/OperationPipeline.hpp"

void OperationPipeline::AddOperation(std::unique_ptr<ImageOperation>&& operation) {
  operations.emplace_back(std::move(operation));
}

void OperationPipeline::Run(Image& image) const {
  for (const auto& operation : operations) {
    operation->Apply(image);
  }
}
