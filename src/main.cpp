#include <cstdlib>
#include <initializer_list>
#include <memory>
#include <stdexcept>
#include <string>

#include "../include/imageformatter/DarkenOperation.hpp"
#include "../include/imageformatter/Image.hpp"
#include "../include/imageformatter/InvertOperation.hpp"
#include "../include/imageformatter/OperationPipeline.hpp"
#include "../include/imageformatter/ReflectHorizontallyOperation.hpp"
#include "../include/imageformatter/ReflectVerticallyOperation.hpp"
#include "../include/imageformatter/RotateOperation.hpp"
#include "../include/imageformatter/SobelOperatorOperation.hpp"
#include "../include/imageformatter/SteganographyDecodeOperation.hpp"
#include "../include/imageformatter/SteganographyEncodeOperation.hpp"
#include "../include/imageformatter/GrayscaleOperation.hpp"
#include "../third_party/cxxopts/cxxopts.hpp"

int main(int argc, char **argv) {
  cxxopts::Options options(
      "ImageFormatter",
      "This program formats the input picture based on cli arguments");

  options.add_options()("i,input", "Input file", cxxopts::value<std::string>())(
      "o,output", "Output file", cxxopts::value<std::string>())(
      "operations", "Names of operations to perform",
      cxxopts::value<std::vector<std::string>>());

  options.parse_positional({"operations"});

  auto result = options.parse(argc, argv);

  if (!result.contains("input") || !result.contains("output")) {
    throw std::runtime_error(
        "Input or Output files are not mentioned in the argument list");
  }

  Image image(result["input"].as<std::string>());

  const auto &argument_list = result.arguments();

  OperationPipeline pipeline;

  for (size_t operation_index = 0; operation_index < argument_list.size();
       ++operation_index) {
    if (argument_list[operation_index].key() != "operations") {
      continue;
    }

    const std::string &val = argument_list[operation_index].value();

    if (val == "darken") {
      if (operation_index + 1 >= argument_list.size()) {
        throw std::runtime_error("Not enough arguments for darken");
      }

      if (!DarkenOperation::ValidateArguments(
              {argument_list[operation_index + 1].value()})) {
        throw std::runtime_error("Invalid argumet for darken: " +
                                 argument_list[operation_index + 1].value());
      }

      pipeline.AddOperation(
          std::make_unique<DarkenOperation>(std::vector<std::string>{
              argument_list[operation_index + 1].value()}));

      ++operation_index;
    } else if (val == "invert") {
      pipeline.AddOperation(
          std::make_unique<InvertOperation>(std::vector<std::string>()));
    } else if (val == "ref-hor") {
      pipeline.AddOperation(std::make_unique<ReflectHorizontallyOperation>(
          std::vector<std::string>()));
    } else if (val == "ref-ver") {
      pipeline.AddOperation(std::make_unique<ReflectVerticallyOperation>(
          std::vector<std::string>()));
    } else if (val == "rotate") {
      if (operation_index + 1 >= argument_list.size()) {
        throw std::runtime_error("Not enough arguments for rotate");
      }

      if (!RotateOperation::ValidateArguments(
              {argument_list[operation_index + 1].value()})) {
        throw std::runtime_error("Invalid argument for rotate: " +
                                 argument_list[operation_index + 1].value());
      }

      pipeline.AddOperation(
          std::make_unique<RotateOperation>(std::vector<std::string>{
              argument_list[operation_index + 1].value()}));

      ++operation_index;
    } else if (val == "sobel") {
      pipeline.AddOperation(
          std::make_unique<SobelOperatorOperation>(std::vector<std::string>()));
    } else if (val == "encode") {
      if (operation_index + 1 >= argument_list.size()) {
        throw std::runtime_error("Not enough arguments for darken");
      }

      pipeline.AddOperation(std::make_unique<SteganographyEncodeOperation>(
          std::vector<std::string>{
              argument_list[operation_index + 1].value()}));

      ++operation_index;
    } else if (val == "decode") {
      pipeline.AddOperation(std::make_unique<SteganographyDecodeOperation>());
    } else if (val == "grayscale") {
      pipeline.AddOperation(std::make_unique<GrayscaleOperation>());
    } else {
      throw std::runtime_error("Unknown argument: " + val);
    }
  }

  pipeline.Run(image);

  image.CreateOutputImage(result["output"].as<std::string>());

  return 0;
}
