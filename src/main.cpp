#include <cstdlib>
#include <functional>
#include <initializer_list>
#include <stdexcept>
#include <string>
#include <unordered_map>

#include "../third_party/cxxopts/cxxopts.hpp"
#include "../include/imageformatter/Image.hpp"
#include "../include/imageformatter/DarkenOperation.hpp"
#include "../include/imageformatter/OperationPipeline.hpp"

int main(int argc, char** argv) {
  // TODO: Сделать проверку аргументов сразу

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

  const auto& argument_list = result.arguments();

  OperationPipeline pipeline;

  for (size_t operation_index = 0; operation_index < argument_list.size();
       ++operation_index) {
    if (argument_list[operation_index].key() != "operations") {
      continue;
    }

    const std::string& val = argument_list[operation_index].value();

    if (val == "darken") {
      if (operation_index + 1 >= argument_list.size()) {
        throw std::runtime_error("Not enough arguments for darken");
      }

      if (!DarkenOperation::ValidateArguments({argument_list[operation_index + 1].value()})) {
        throw std::runtime_error("Invalid argumet for darken: " + argument_list[operation_index + 1].value());
      }

      pipeline.AddOperation(std::move(std::make_unique<DarkenOperation>(
              std::vector<std::string>{
                argument_list[operation_index + 1].value()
              }))
          );

      ++operation_index;
     } else if (val == "invert") {
      return {};
    } else if (val == "ref-hor") {
      return {};
    } else if (val == "ref-ver") {
      return {};
    } else if (val == "rotate") {
      if (operation_index + 1 >= argument_list.size()) {
        throw std::runtime_error("Not enough arguments for rotate");
      }
      const std::string& val1 =
        argument_list[operation_index + 1].value();
      ++operation_index;
      return {val1};
    } else if (val == "sobel") {
      return {};
    } else {
      throw std::runtime_error("Unknown argument: " + val);
    }
  }

  pipeline.Run(image);

  image.CreateOutputImage(result["output"].as<std::string>());

  return 0;
}

