#include "../../include/imageformatter/OperationPipeline.hpp"

#include <iostream>

void OperationPipeline::AddOperation(
    std::unique_ptr<ImageOperation> &&operation) {
  operations.emplace_back(std::move(operation));
}

void OperationPipeline::Run(Image &image) const {
  for (const auto &operation : operations) {
    std::cout << "Applying " << operation->GetName() << std::endl;
    operation->Apply(image);
  }
}
