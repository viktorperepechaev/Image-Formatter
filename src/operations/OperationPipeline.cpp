#include "../../include/imageformatter/OperationPipeline.hpp"
#include "../../include/imageformatter/GrayscaleOperation.hpp"

#include <iostream>

void OperationPipeline::AddOperation(
    std::unique_ptr<ImageOperation> &&operation) {
  operations.emplace_back(std::move(operation));
}

void OperationPipeline::Run(Image &image) {
  for (const auto &operation : operations) {
    if (operation->GetName() == "ascii" && !had_operation_["grayscale"]) {
        std::cout << "Applying additional grayscale for ascii\n";
        GrayscaleOperation op;
        op.Apply(image);
    }
    std::cout << "Applying " << operation->GetName() << std::endl;
    operation->Apply(image);
  }
}
