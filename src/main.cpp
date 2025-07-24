#include <cstdlib>
#include <stdexcept>
#include <string>

#include "headers/image.hpp"
#include "headers/cxxopts.hpp"

int main(int argc, char** argv) {
  // TODO: Сделать проверку аргументов сразу
  
  cxxopts::Options options("ImageFormatter", "This program formats the input picture based on cli arguments");

  options.add_options()
    ("i,input", "Input file", cxxopts::value<std::string>())
    ("o,output", "Output file", cxxopts::value<std::string>())
    ("operations", "Names of operations to perform", cxxopts::value<std::vector<std::string>>())
    ;
  
  options.parse_positional({"operations"});

  auto result = options.parse(argc, argv);

  if (!result.contains("input") || !result.contains("output")) {
    throw std::runtime_error("Input or Output files are not mentioned in the argument list");
  }

  Image image(result["input"].as<std::string>());

  std::vector<std::string> operation_list;

  for (const auto& kv : result.arguments()) {
    if (kv.key() != "operations") {
      continue;
    }
    operation_list.emplace_back(kv.value());
  }

  for (size_t operation_list_index = 0; operation_list_index < operation_list.size(); ++operation_list_index) {
    if (operation_list[operation_list_index] == "darken") {
      image.Darken(std::stoi(operation_list[operation_list_index + 1]));
      ++operation_list_index;
    } else if (operation_list[operation_list_index] == "invert") {
      image.Invert();
    } else if (operation_list[operation_list_index] == "ref-hor") {
      image.ReflectHorizontally();
    } else if (operation_list[operation_list_index] == "ref-ver") {
      image.ReflectVertically();
    } else {
      throw std::runtime_error("Unknown operation: " + operation_list[operation_list_index]);
    }
  }

  image.CreateOutputImage(result["output"].as<std::string>());
  
  return 0;
}
